#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/libsnowflakeid.h"

#define DATACENTER_ID_VALUE     5
#define WORKER_ID_VALUE         1
#define OFFSET_TIME_MS_VALUE    0

#ifdef _WIN32
static inline uint64_t get_current_time_ms(void) {
    static const ULONGLONG epoch_offset_us = 116444736000000000ULL;
    FILETIME               filetime;
    ULARGE_INTEGER         x;

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
    GetSystemTimePreciseAsFileTime(&filetime);
#else
    GetSystemTimeAsFileTime(&filetime);
#endif
    x.LowPart = filetime.dwLowDateTime;
    x.HighPart = filetime.dwHighDateTime;

    return ((x.QuadPart - epoch_offset_us) / 10000);
}
#else

static inline uint64_t get_current_time_ms(void) {
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);

    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

#endif

int main(const int argc, const char *const *argv) {
    enum e_snowflakeid_init_status     status_out;
    struct s_snowflakeid_generator_ctx *ctx = snowflakeid_initialize(
            DATACENTER_ID_VALUE,
            WORKER_ID_VALUE,
            OFFSET_TIME_MS_VALUE,
            &status_out);

    if (ctx == NULL) {
        printf("Can't create CTX: %d\n", status_out);
        return (-1);
    }

    if (argc > 2 && strcmp(argv[1], "PERFORMANCE") == 0) {
        int max_value = (int) strtol(argv[2], NULL, 10);

        printf("START PERFORMANCE TEST: %d\n", max_value);

        uint64_t *id_array = malloc(sizeof(uint64_t) * max_value + 1);
        memset(id_array, 0, sizeof(uint64_t) * max_value + 1);

        uint64_t time_start = get_current_time_ms();
        for (int idx        = 0; idx < max_value; ++idx) {
            id_array[idx] = snowflakeid_next_value(ctx);
        }
        uint64_t time_stop  = get_current_time_ms();
        printf("END PERFORMANCE TEST  : %" PRIu64 "ms\n", time_stop - time_start);

        free(id_array);
    } else if (argc > 1 && strcmp(argv[1], "SINGLE") == 0) {
        printf("libsnowflakeid version %s (%d)\n", snowflakeid_get_version_as_str(), snowflakeid_get_version_as_int());

        uint64_t snowflakeid = snowflakeid_next_value(ctx);
        printf("SINGLE: %" PRIu64 " (timestamp_ms: %" PRIu64 ", datacenter_id: %" PRIu8 ", worker_id: %" PRIu8 ", inc: %" PRIu16 ")\n",
               snowflakeid,
               SNOWFLAGEID_GET_TIMESTAMP(snowflakeid),
               SNOWFLAGEID_GET_DATACENTER_ID(snowflakeid),
               SNOWFLAGEID_GET_WORKER_ID(snowflakeid),
               SNOWFLAGEID_GET_SEQUENCE(snowflakeid));
    }

    snowflakeid_destroy(ctx);
    return (0);
}
