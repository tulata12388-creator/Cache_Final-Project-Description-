#ifndef CACHE_CONFIG_H
#define CACHE_CONFIG_H

#include <stdint.h>

#define SET_CNT 16384
#define D_CACHE_WAY_CNT 4
#define I_CACHE_WAY_CNT 2
#define TEXT_FILE_NAME "trace.txt"

typedef struct {
    uint8_t lru;
    uint8_t valid;
    uint8_t dirty;
    uint32_t tag;
    uint32_t dirty_address;
} D_line;

typedef struct {
    uint8_t lru;
    uint8_t valid;
    uint32_t tag;
} I_line;

typedef enum {
    TRACE_LINE_INVALID = -1,
    TRACE_LINE_BLANK = -2,
    TRACE_DATA_READ = 0,
    TRACE_DATA_WRITE = 1,
    TRACE_INSTRUCTION_FETCH = 2,
    TRACE_L2_EVICT = 3,
    TRACE_CLEAR_CACHE = 8,
    TRACE_PRINT_CACHE = 9
} TraceType;

#endif