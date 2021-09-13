#include <stdlib.h>
#include "libsnowflakeid.h"

struct s_snowflakeid_generator_ctx *snowflakeid_initialize(const uint8_t datacenter_id,
                                                           const uint8_t worker_id) {
    if (datacenter_id > 32 || worker_id > 32) {
        return (NULL);
    }

    struct s_snowflakeid_generator_ctx *ctx = malloc(sizeof(struct s_snowflakeid_generator_ctx));
    if (ctx != NULL) {
        ctx->last_time_ms    = 0;
        ctx->datacenter_id   = datacenter_id;
        ctx->worker_id       = worker_id;
        ctx->sequence_number = 0;

        if (pthread_mutex_init(&ctx->internal_lock, PTHREAD_MUTEX_NORMAL) != 0) {
            free (ctx);
            return (NULL);
        }
    }

    return (ctx);
}
