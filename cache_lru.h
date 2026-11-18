#ifndef CACHE_LRU_H
#define CACHE_LRU_H

#include "cache_config.h"

void find_victim(
    uint32_t set,
    uint8_t *victim_way,
    uint8_t cache_type
);

void update_D_LRU(
    uint32_t set,
    uint32_t accessed_way,
    uint8_t operation
);

void update_I_LRU(
    uint32_t set,
    uint32_t accessed_way,
    uint8_t operation
);

#endif