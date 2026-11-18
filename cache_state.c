#include "cache_state.h"

uint8_t mode;

uint32_t cache_read;
uint32_t cache_write;
uint32_t cache_hits;
uint32_t cache_misses;

D_line d_cache[SET_CNT][D_CACHE_WAY_CNT];
I_line i_cache[SET_CNT][I_CACHE_WAY_CNT];