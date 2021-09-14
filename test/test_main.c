#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/libsnowflakeid.h"

# ifdef _WIN32
static inline uint64_t get_current_time_ms(void) {
    static const ULONGLONG epoch_offset_us = 11644473600000000ULL; // Microseconds betweeen Jan 1,1601 and Jan 1,1970
    FILETIME               filetime; // Representing the number of 100-nanosecond intervals since January 1, 1601 00:00 UTC
    ULARGE_INTEGER         x;

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
    GetSystemTimePreciseAsFileTime(&filetime);
#else
    GetSystemTimeAsFileTime(&filetime);
#endif
    x.LowPart = filetime.dwLowDateTime;
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

int main(const int argc, const char *const *argv) {
    enum e_snowflakeid_init_status     status_out;
    struct s_snowflakeid_generator_ctx *ctx = snowflakeid_initialize(5, 1, &status_out);
    if (ctx == NULL) {
        printf("Can't create CTX: %d\n", status_out);
        return (-1);
    }

    if (argc > 2 && strcmp(argv[1], "PERFORMANCE") == 0) {
        int max_value = atoi(argv[2]);
        printf("START PERFORMANCE TEST: %d\n", max_value);

        uint64_t *id_array = malloc(sizeof(uint64_t) * max_value + 1);
        memset(id_array, 0, sizeof(uint64_t) * max_value + 1);

        uint64_t time_start = get_current_time_ms();
        for (int idx        = 0; idx < max_value; ++idx) {
            id_array[idx] = snowflakeid_next_value(ctx);
        }
        uint64_t time_stop  = get_current_time_ms();

        printf("END PERFORMANCE TEST  : %" PRIu64  "ms\n", time_stop - time_start);

        free(id_array);
    } else if (argc > 1 && strcmp(argv[1], "SINGLE") == 0) {
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
