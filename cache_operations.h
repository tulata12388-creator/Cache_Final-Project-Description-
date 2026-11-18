#ifndef CACHE_OPERATIONS_H
#define CACHE_OPERATIONS_H

#include <stdint.h>

void read_data(uint32_t address);
void write_data(uint32_t address);
void fetch_instruction(uint32_t address);
void evict_L2(uint32_t address);

#endif