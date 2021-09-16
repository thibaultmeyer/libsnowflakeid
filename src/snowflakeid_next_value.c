#include "libsnowflakeid.h"

#ifdef _WIN32

static inline uint64_t get_current_time_ms(void) {
    static const ULONGLONG epoch_offset_us = 11644473600000000ULL; // Microseconds betweeen Jan 1,1601 and Jan 1,1970
    FILETIME               filetime; // Representing the number of 100-nanosecond intervals since January 1, 1601 00:00 UTC
    ULARGE_INTEGER         x;

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
    GetSystemTimePreciseAsFileTime(&filetime);
#else
    GetSystemTimeAsFileTime(&filetime);
#endif
    x.LowPart  = filetime.dwLowDateTime;
    x.HighPart = filetime.dwHighDateTime;

    return ((x.QuadPart / 10) - epoch_offset_us) / 1000;
}

#else

static inline uint64_t get_current_time_ms(void) {
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);

    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

#endif

static inline uint64_t get_next_time_ms(const uint64_t *const not_until_time_ms) {
    uint64_t current_time_ms;

    do {
        current_time_ms = get_current_time_ms();
    } while (*not_until_time_ms == current_time_ms);

    return current_time_ms;
}

uint64_t snowflakeid_next_value(s_snowflakeid_generator_ctx *const ctx) {
    uint64_t current_time_ms = get_current_time_ms();

    SNOWFLAKEID_LOCK_WAIT_FOR(ctx->internal_lock);
    if (ctx->last_time_ms == current_time_ms) {
        ctx->sequence_number = (ctx->sequence_number + 1) % SNOWFLAKEID_SEQUENCE_MAX;
        if (ctx->sequence_number == 0) {
            current_time_ms = get_next_time_ms(&ctx->last_time_ms);
        }
    } else {
        ctx->sequence_number = 0;
    }
    ctx->last_time_ms        = current_time_ms;
    uint16_t sequence_number = ctx->sequence_number;
    SNOWFLAKEID_LOCK_RELEASE(ctx->internal_lock);

    return ((current_time_ms << SNOWFLAKEID_TIMESTAMP_SHIFT)
            | (ctx->datacenter_id << SNOWFLAKEID_DATACENTER_ID_SHIFT)
            | (ctx->worker_id << SNOWFLAKEID_WORKER_ID_SHIFT)
            | sequence_number);
}
