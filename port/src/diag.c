#include "diag.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <switch.h>

__attribute__((used)) const char build_id[] = "POP2-BUILD " POP2_BUILD_ID;
const char *last_call = "start";

#if defined(DIAGNOSTICS)
static int log_fd = -1;
static int link_fd = -1;
#ifdef DIAGNOSTICS
static volatile uint32_t log_lines;
#endif
static void log_out(const char *s, size_t n) {
    if (log_fd >= 0 && write(log_fd, s, n) < 0) { close(log_fd); log_fd = -1; }
    if (link_fd >= 0 && write(STDOUT_FILENO, s, n) != (ssize_t)n) {
        link_fd = -1;
        static const char m[] = "log: nxlink write failed, nxlink off for this run\n";
        if (log_fd >= 0) (void)!write(log_fd, m, sizeof m - 1);
    }
}

#define LOG_RING (64 * 1024)
static char log_ring[LOG_RING];
static volatile uint32_t ring_head, ring_tail; static uint32_t ring_dropped;
static Mutex ring_lock, flush_lock;
static int ring_ready;
static void ring_put(const char *s, int n) {
    if (n <= 0) return;
    if (!ring_ready) { log_out(s, (size_t)n); return; }
    mutexLock(&ring_lock);
    if (ring_head - ring_tail + (uint32_t)n > LOG_RING) ring_dropped++;
    else {
        uint32_t o = ring_head & (LOG_RING - 1), first = LOG_RING - o;
        if ((uint32_t)n <= first) memcpy(log_ring + o, s, (size_t)n);
        else { memcpy(log_ring + o, s, first); memcpy(log_ring, s + first, (size_t)n - first); }
        ring_head += (uint32_t)n;
    }
    mutexUnlock(&ring_lock);
}
void log_flush(void) {
    if (!ring_ready) return;
    mutexLock(&flush_lock);
    mutexLock(&ring_lock);
    uint32_t t = ring_tail, h = ring_head, dropped = ring_dropped; ring_dropped = 0;
    mutexUnlock(&ring_lock);
    while (t != h) {
        uint32_t o = t & (LOG_RING - 1), n = h - t;
        if (o + n > LOG_RING) n = LOG_RING - o;
        log_out(log_ring + o, n); t += n;
    }
    if (dropped) { char b[64]; int n = snprintf(b, sizeof b, "log: %u lines dropped (ring full)\n", (unsigned)dropped); log_out(b, (size_t)n); }
    if (log_fd >= 0) fsync(log_fd);
    mutexLock(&ring_lock);
    ring_tail = t;
    mutexUnlock(&ring_lock);
    mutexUnlock(&flush_lock);
}
#ifdef DIAGNOSTICS
void say(const char *fmt, ...) {
    char line[640]; va_list ap; va_start(ap, fmt);
    int n = vsnprintf(line, sizeof line, fmt, ap); va_end(ap);
    if (n > (int)sizeof line - 1) n = sizeof line - 1;
    if (n > 0) { ring_put(line, n); log_lines++; }
}
#endif
#ifdef DIAGNOSTICS
void diag_alive(void) { log_lines++; }
#endif
void diag_open(void) {
    mkdir(DATA_DIR, 0777);
    log_fd = open(DATA_DIR "/debug.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (R_SUCCEEDED(socketInitializeDefault())) { link_fd = nxlinkStdio(); if (link_fd < 0) socketExit(); }
    if (link_fd >= 0) fcntl(STDOUT_FILENO, F_SETFL, fcntl(STDOUT_FILENO, F_GETFL, 0) | O_NONBLOCK);
    say("%s\n", build_id);
    mutexInit(&ring_lock); mutexInit(&flush_lock); ring_ready = 1;
    say("log: ring, written once a second by the watchdog thread; nxlink %s\n", link_fd >= 0 ? "on" : "off");
}
#ifdef DIAGNOSTICS
void mem_log(const char *where) {
    u64 total = 0, used = 0;
    svcGetInfo(&total, InfoType_TotalMemorySize, CUR_PROCESS_HANDLE, 0);
    svcGetInfo(&used, InfoType_UsedMemorySize, CUR_PROCESS_HANDLE, 0);
    say("mem[%s]: used=%u KB total=%u KB applet=%d\n", where, (unsigned)(used / 1024), (unsigned)(total / 1024), (int)appletGetAppletType());
}

void clock_log(void) {
    static const struct { PcvModuleId id; const char *name; } m[] = { { PcvModuleId_CpuBus, "cpu" }, { PcvModuleId_GPU, "gpu" }, { PcvModuleId_EMC, "emc" } };
    u32 hz[3] = { 0, 0, 0 };
    if (R_SUCCEEDED(clkrstInitialize())) {
        for (int i = 0; i < 3; i++) { ClkrstSession s; if (R_SUCCEEDED(clkrstOpenSession(&s, m[i].id, 3))) { clkrstGetClockRate(&s, &hz[i]); clkrstCloseSession(&s); } }
        clkrstExit();
    }
    say("clocks: %s=%u MHz %s=%u MHz %s=%u MHz\n", m[0].name, (unsigned)(hz[0] / 1000000), m[1].name, (unsigned)(hz[1] / 1000000), m[2].name, (unsigned)(hz[2] / 1000000));
}

#define WD_HANG_MS 300000
#define WD_HEARTBEAT_MS 60000
static int wd_fd = -1;
static void wd_write(const char *fmt, ...) {
    char b[200]; va_list ap; va_start(ap, fmt);
    int n = vsnprintf(b, sizeof b, fmt, ap); va_end(ap);
    if (n > (int)sizeof b - 1) n = sizeof b - 1;
    if (wd_fd >= 0 && n > 0) { (void)!write(wd_fd, b, (size_t)n); fsync(wd_fd); }
}
static void watchdog_thread(void *arg) {
    (void)arg;
    wd_fd = open(DATA_DIR "/watchdog.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    wd_write("watchdog: running, trap after %d s without a log line\n", WD_HANG_MS / 1000);
    uint32_t last = log_lines; int idle_ms = 0, beat_ms = 0;
    for (;;) {
        svcSleepThread(1000000000ll);
        uint32_t now = log_lines;
        if (now != last) { last = now; idle_ms = 0; } else idle_ms += 1000;
        if ((beat_ms += 1000) >= WD_HEARTBEAT_MS) { beat_ms = 0; wd_write("watchdog: alive, %u log lines, quiet %d s\n", (unsigned)now, idle_ms / 1000); }
        log_flush();
        if (idle_ms >= WD_HANG_MS) {
            wd_write("watchdog: HANG, %d s without a log line, last_call=%s: trapping for a crash report\n", idle_ms / 1000, last_call);
            if (wd_fd >= 0) close(wd_fd);
            __builtin_trap();
        }
    }
}
void watchdog_start(void) {
    static Thread th;
    Result rc = threadCreate(&th, watchdog_thread, NULL, NULL, 0x10000, 0x2B, -2);
    if (R_SUCCEEDED(rc)) rc = threadStart(&th);
    say("watchdog: thread rc 0x%x priority 0x2B\n", (unsigned)rc);
}
#endif
#else
void diag_open(void) {}
#endif

void __real__exit(int code);
void __wrap__exit(int code) {
    say("exit  _exit(%d) from %p last_call=%s\n", code, __builtin_return_address(0), last_call);
    log_flush();
#if defined(DIAGNOSTICS)
    if (log_fd >= 0) close(log_fd);
#endif
    appletUnlockExit();
    __real__exit(code);
}
void abort(void) {
    say("exit  abort() from %p last_call=%s\n", __builtin_return_address(0), last_call);
    log_flush();
    appletUnlockExit();
    __real__exit(1);
    for (;;) ;
}
