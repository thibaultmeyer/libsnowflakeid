#include <pthread.h>
#include <sys/time.h>
#include "libsnowflakeid.h"

static inline uint64_t get_current_time_ms(void) {
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);

    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static inline uint64_t get_next_time_ms(const uint64_t *const not_until_time_ms) {
    uint64_t current_time_ms;

    do {
        current_time_ms = get_current_time_ms();
    } while (*not_until_time_ms == current_time_ms);

    return current_time_ms;
}

uint64_t snowflakeid_next_value(s_snowflakeid_generator_ctx *const ctx) {
    uint64_t current_time_ms = get_current_time_ms();

    pthread_mutex_lock(&ctx->internal_lock);
    if (ctx->last_time_ms == current_time_ms) {
        ctx->sequence_number = (ctx->sequence_number + 1) % SEQUENCE_MAX;
        if (ctx->sequence_number == 0) {
            current_time_ms = get_next_time_ms(&ctx->last_time_ms);
        }
    } else {
        ctx->sequence_number = 0;
    }
    ctx->last_time_ms        = current_time_ms;
    uint16_t sequence_number = ctx->sequence_number;
    pthread_mutex_unlock(&ctx->internal_lock);

    return ((current_time_ms << TIMESTAMP_SHIFT)
            | (ctx->datacenter_id << DATACENTER_ID_SHIFT)
            | (ctx->worker_id << WORKER_ID_SHIFT)
            | sequence_number);
}
