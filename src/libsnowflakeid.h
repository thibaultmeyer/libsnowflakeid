#ifndef LIBSNOWFLAKEID_H
# define LIBSNOWFLAKEID_H

# ifdef __cplusplus
extern "C"
{
# endif

# include <stdint.h>

typedef struct s_snowflakeid_generator_ctx s_snowflakeid_generator_ctx;

/**
 * Snowflake ID generator context.
 */
struct s_snowflakeid_generator_ctx {
    uint64_t last_timestamp;
    uint16_t instance_id;
    uint16_t sequence_number;
};

/**
 * Initialize Snowflake ID generator context.
 *
 * @return Initialized Snowflake ID generator context
 */
struct s_snowflakeid_generator_ctx *snowflakeid_initialize(uint8_t instance_id);

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
