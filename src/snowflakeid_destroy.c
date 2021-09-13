#include <stdlib.h>
#include "libsnowflakeid.h"

void snowflakeid_destroy(s_snowflakeid_generator_ctx *const ctx) {
    if (ctx != NULL) {
        pthread_mutex_destroy(&ctx->internal_lock);
        free(ctx);
    }
}
