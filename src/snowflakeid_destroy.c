#include <stdlib.h>
#include "libsnowflakeid.h"

void snowflakeid_destroy(s_snowflakeid_generator_ctx *const ctx) {
    if (ctx != NULL) {
        SNOWFLAKEID_LOCK_DESTROY(ctx->internal_lock);
        free(ctx);
    }
}
