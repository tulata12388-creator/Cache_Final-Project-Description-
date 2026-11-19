#include "cache_state.h"

uint8_t mode;

// Thong ke L1 Data Cache
uint32_t d_cache_read;
uint32_t d_cache_write;
uint32_t d_cache_hits;
uint32_t d_cache_misses;

// Thong ke L1 Instruction Cache
uint32_t i_cache_read;
uint32_t i_cache_write;
uint32_t i_cache_hits;
uint32_t i_cache_misses;

D_line d_cache[SET_CNT][D_CACHE_WAY_CNT];
I_line i_cache[SET_CNT][I_CACHE_WAY_CNT];