#include <stdlib.h>
#include "libsnowflakeid.h"

struct s_snowflakeid_generator_ctx *snowflakeid_initialize(const uint8_t datacenter_id,
                                                           const uint8_t worker_id,
                                                           enum e_snowflakeid_init_status *status_out) {
    // Verify datacenter_id and worker_id
    if (datacenter_id > 31 || worker_id > 31) {
        if (status_out != NULL) {
            *status_out = SNOWFLAKEID_INIT_ERROR_INVALID_ARGUMENT;
        }

        return (NULL);
    }

    // Allocate a new ctx instance
    struct s_snowflakeid_generator_ctx *ctx = malloc(sizeof(struct s_snowflakeid_generator_ctx));
    if (ctx == NULL) {
        if (status_out != NULL) {
            *status_out = SNOWFLAKEID_INIT_ERROR_MEMORY_ALLOCATION;
        }

        return (NULL);
    }

    // Initialize internal lock
#ifdef USE_WINDOWS_MUTEX
    ctx->internal_lock = CreateMutex(NULL, FALSE, NULL);
    if (ctx->internal_lock == NULL) {
#else
    if (pthread_mutex_init(&ctx->internal_lock, NULL) != 0) {
#endif
        free(ctx);

        if (status_out != NULL) {
            *status_out = SNOWFLAKEID_INIT_ERROR_LOCK_INITIALIZATIO;
        }

        return (NULL);
    }

    // Configure ctx
    ctx->last_time_ms    = 0;
    ctx->datacenter_id   = datacenter_id;
    ctx->worker_id       = worker_id;
    ctx->sequence_number = 0;

    // Success
    if (status_out != NULL) {
        *status_out = SNOWFLAKEID_INIT_SUCCESS;
    }

    return (ctx);
}
