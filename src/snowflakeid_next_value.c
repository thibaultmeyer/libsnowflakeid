#include "libsnowflakeid.h"

uint64_t snowflakeid_next_value(s_snowflakeid_generator_ctx *const ctx) {
    ctx->sequence_number = 0;
    return (0);
}
