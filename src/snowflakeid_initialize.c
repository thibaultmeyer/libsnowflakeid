#include <stdlib.h>
#include "libsnowflakeid.h"

struct s_snowflakeid_generator_ctx *snowflakeid_initialize(const uint8_t instance_id) {
    struct s_snowflakeid_generator_ctx *ctx = malloc(sizeof(struct s_snowflakeid_generator_ctx));
    if (ctx != NULL) {
        ctx->last_timestamp  = 0;
        ctx->instance_id     = instance_id;
        ctx->sequence_number = 0;
    }

    return (ctx);
}
