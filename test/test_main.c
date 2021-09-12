#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "../src/libsnowflakeid.h"

static inline uint64_t get_current_time_ms(void) {
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);

    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int main(const int argc, const char *const *argv) {
    struct s_snowflakeid_generator_ctx *ctx = snowflakeid_initialize(5, 1);
    if (ctx == NULL) {
        printf("Can't create CTX\n");
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
               (uint64_t) (snowflakeid >> TIMESTAMP_SHIFT),
               (uint8_t) ((snowflakeid & 0x3E0000) >> DATACENTER_ID_SHIFT),
               (uint8_t) ((snowflakeid & 0x1F000) >> WORKER_ID_SHIFT),
               (uint16_t) (snowflakeid & 0xFFF));
    }

    snowflakeid_destroy(ctx);
    return (0);
}
