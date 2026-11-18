#ifndef CACHE_STATE_H
#define CACHE_STATE_H

#include "cache_config.h"

// Khai bao bien toan cuc
extern uint8_t mode;

extern uint32_t cache_read;
extern uint32_t cache_write;
extern uint32_t cache_hits;
extern uint32_t cache_misses;

extern D_line d_cache[SET_CNT][D_CACHE_WAY_CNT];
extern I_line i_cache[SET_CNT][I_CACHE_WAY_CNT];

#endif