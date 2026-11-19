#ifndef CACHE_STATE_H
#define CACHE_STATE_H

#include "cache_config.h"

// Khai bao bien toan cuc
extern uint8_t mode;

// Bo dem rieng cho L1 Data Cache
extern uint32_t d_cache_read;
extern uint32_t d_cache_write;
extern uint32_t d_cache_hits;
extern uint32_t d_cache_misses;

// Bo dem rieng cho L1 Instruction Cache
extern uint32_t i_cache_read;
extern uint32_t i_cache_write;
extern uint32_t i_cache_hits;
extern uint32_t i_cache_misses;

extern D_line d_cache[SET_CNT][D_CACHE_WAY_CNT];
extern I_line i_cache[SET_CNT][I_CACHE_WAY_CNT];

#endif