#ifndef CACHE_CORE_H
#define CACHE_CORE_H

#include "cache_config.h"

void decode_address(
    uint32_t address,
    uint32_t *tag,
    uint32_t *set
);

void hit_miss_flag(
    uint32_t tag,
    uint32_t set,
    uint8_t *is_hit,
    uint8_t *filled_way_cnt,
    uint8_t *accessed_way,
    uint8_t *empty_way,
    uint8_t cache_type
);

void clear_cache(void);

#endif