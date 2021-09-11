#ifndef LIBSNOWFLAKEID_H
# define LIBSNOWFLAKEID_H

# ifdef __cplusplus
extern "C"
{
# endif

# define WORKER_ID_BITS         5L
# define DATACENTER_ID_BITS     5L
# define SEQUENCE_BITS          12L
# define WORKER_ID_SHIFT        SEQUENCE_BITS
# define DATACENTER_ID_SHIFT    (SEQUENCE_BITS + WORKER_ID_BITS)
# define TIMESTAMP_LEFT_SHIFT   (SEQUENCE_BITS + WORKER_ID_BITS + DATACENTER_ID_BITS)
# define SEQUENCE_MAX           ((1 << SEQUENCE_BITS) - 1)

# include <pthread.h>
# include <stdint.h>

typedef struct s_snowflakeid_generator_ctx s_snowflakeid_generator_ctx;

/**
 * Snowflake ID generator context.
 */
struct s_snowflakeid_generator_ctx {
    uint64_t        last_time_ms;
    uint8_t         datacenter_id;
    uint8_t         worker_id;
    uint16_t        sequence_number;
    pthread_mutex_t sequence_number_mutex;
};

/**
 * Destroys the given Snowflake ID generator context.
 *
 * @param ctx The Snowflake ID generator context to destroy
 */
void snowflakeid_destroy(struct s_snowflakeid_generator_ctx *ctx);

/**
 * Initialize Snowflake ID generator context.
 *
 * @param datacenter_id The Datacenter ID (Max. value = 32)
 * @param worker_id The Worker ID (Max. value = 32)
 * @return Initialized Snowflake ID generator context, or NULL in case of error
 */
struct s_snowflakeid_generator_ctx *snowflakeid_initialize(uint8_t datacenter_id, uint8_t worker_id);

/**
 * Get the next Snowflake ID.
 *
 * @param ctx The Snowflake ID generator context
 * @return Initialized Snowflake ID generator context
 */
uint64_t snowflakeid_next_value(s_snowflakeid_generator_ctx *ctx);

# ifdef __cplusplus
}
# endif

#endif //LIBSNOWFLAKEID_H
