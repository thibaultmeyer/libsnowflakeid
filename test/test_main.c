#include <inttypes.h>
#include <stdio.h>
#include "../src/libsnowflakeid.h"

#define MAX_ITERATION   100000

int main(void) {
    struct s_snowflakeid_generator_ctx *ctx = snowflakeid_initialize(5, 1);
    if (ctx == NULL) {
        printf("Can't create CTX\n");
        return (-1);
    }

    uint64_t ids[MAX_ITERATION] = {0};
    for (int idx                = 0; idx < MAX_ITERATION; ++idx) {
        ids[idx] = snowflakeid_next_value(ctx);
    }
    for (int idx                = 0; idx < MAX_ITERATION; ++idx) {
        printf("%" PRIu64 "\n", ids[idx]);
    }

    snowflakeid_destroy(ctx);
    return (0);
}
