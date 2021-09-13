#ifndef LIBSNOWFLAKEID_H
# define LIBSNOWFLAKEID_H

# ifdef __cplusplus
extern "C"
{
# endif

# define SNOWFLAKEID_WORKER_ID_BITS         5L
# define SNOWFLAKEID_DATACENTER_ID_BITS     5L
# define SNOWFLAKEID_SEQUENCE_BITS          12L
# define SNOWFLAKEID_WORKER_ID_SHIFT        SNOWFLAKEID_SEQUENCE_BITS
# define SNOWFLAKEID_DATACENTER_ID_SHIFT    (SNOWFLAKEID_SEQUENCE_BITS + SNOWFLAKEID_WORKER_ID_BITS)
# define SNOWFLAKEID_TIMESTAMP_SHIFT        (SNOWFLAKEID_SEQUENCE_BITS + SNOWFLAKEID_WORKER_ID_BITS + SNOWFLAKEID_DATACENTER_ID_BITS)
# define SNOWFLAKEID_SEQUENCE_MAX           ((1 << SNOWFLAKEID_SEQUENCE_BITS) - 1)

# include <pthread.h>
# include <stdint.h>

/**
 * Result value used in the function "snowflakeid_initialize".
 */
typedef enum e_snowflakeid_init_status {
    SNOWFLAKEID_INIT_SUCCESS                  = 0,
    SNOWFLAKEID_INIT_ERROR_INVALID_ARGUMENT   = 1,
    SNOWFLAKEID_INIT_ERROR_MEMORY_ALLOCATION  = 2,
    SNOWFLAKEID_INIT_ERROR_LOCK_INITIALIZATIO = 3
} e_snowflakeid_init_status;

/**
 * Snowflake ID generator context.
 */
typedef struct s_snowflakeid_generator_ctx {
    uint64_t        last_time_ms;
    uint8_t         datacenter_id;
    uint8_t         worker_id;
    uint16_t        sequence_number;
    pthread_mutex_t internal_lock;
} s_snowflakeid_generator_ctx;

/**
 * Destroys the given Snowflake ID generator context.
 *
 * @param ctx [IN] The Snowflake ID generator context to destroy
 */
void snowflakeid_destroy(struct s_snowflakeid_generator_ctx *ctx);

/**
 * Initialize Snowflake ID generator context.
 *
 * @param datacenter_id [IN] The Datacenter ID (Max. value = 32)
 * @param worker_id     [IN] The Worker ID (Max. value = 32)
 * @param status_out    [OUT] The operation status (ie: SNOWFLAKEID_INIT_SUCCESS)
 * @return Initialized Snowflake ID generator context, or NULL in case of error
 */
struct s_snowflakeid_generator_ctx *snowflakeid_initialize(uint8_t datacenter_id,
                                                           uint8_t worker_id,
                                                           enum e_snowflakeid_init_status *status_out);

/**
 * Get the next Snowflake ID.
 *
 * @param ctx [IN] The Snowflake ID generator context
 * @return Initialized Snowflake ID generator context
 */
uint64_t snowflakeid_next_value(s_snowflakeid_generator_ctx *ctx);

# ifdef __cplusplus
}
# endif

#endif //LIBSNOWFLAKEID_H
