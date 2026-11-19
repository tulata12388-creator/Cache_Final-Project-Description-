#include "cache_output.h"
#include "cache_state.h"

#include <stdio.h>
#include <inttypes.h>

void print_statistics(void)
{
    uint32_t d_total = d_cache_hits + d_cache_misses;
    uint32_t i_total = i_cache_hits + i_cache_misses;
    double d_hit_ratio = 0;
    double i_hit_ratio = 0;

    if (d_total > 0)
    {
        d_hit_ratio = ((double)d_cache_hits / d_total) * 100;
    }
    if (i_total > 0)
    {
        i_hit_ratio = ((double)i_cache_hits / i_total) * 100;
    }

    printf("*** L1 DATA CACHE STATISTICS ***\n");
    printf("Number of cache reads: %u\n", (unsigned)d_cache_read);
    printf("Number of cache writes: %u\n", (unsigned)d_cache_write);
    printf("Number of cache hits: %u\n", (unsigned)d_cache_hits);
    printf("Number of cache misses: %u\n", (unsigned)d_cache_misses);
    printf("Cache hit ratio: %.4f%%\n", d_hit_ratio);

    printf("\n*** L1 INSTRUCTION CACHE STATISTICS ***\n");
    printf("Number of cache reads: %u\n", (unsigned)i_cache_read);
    printf("Number of cache writes: %u\n", (unsigned)i_cache_write);
    printf("Number of cache hits: %u\n", (unsigned)i_cache_hits);
    printf("Number of cache misses: %u\n", (unsigned)i_cache_misses);
    printf("Cache hit ratio: %.4f%%\n", i_hit_ratio);
}
void print_cache(void)
{
    uint8_t d_found = 0;
    uint8_t i_found = 0;

    // =========================================
    // IN L1 DATA CACHE
    // =========================================
    printf("==================== L1 DATA CACHE ====================\n");

    printf("%-8s %-8s %-12s %-12s %-8s %-8s %-12s\n",
           "Set", "Way", "Line Addr", "Tag", "LRU", "Dirty", "Dirty Addr");

    for (uint32_t set = 0; set < SET_CNT; set++)
    {
        for (uint32_t way = 0; way < D_CACHE_WAY_CNT; way++)
        {
            if (d_cache[set][way].valid)
            {
                d_found = 1;

                // Khôi phục địa chỉ đầu cache line từ tag và set
                uint32_t line_address =
                    (d_cache[set][way].tag << 20) | (set << 6);

                // 0x%08x (10 ký tự) + 2 spaces = 12 ký tự (khớp %-12s)
                // 0x%03x (5 ký tự)  + 7 spaces = 12 ký tự (khớp %-12s)
                printf("%-8" PRIu32 " %-8" PRIu32
                       " 0x%08" PRIx32 "   0x%03" PRIx32 "        "
                       "%-8u %-8u ",
                       set,
                       way,
                       line_address,
                       d_cache[set][way].tag,
                       d_cache[set][way].lru,
                       d_cache[set][way].dirty);

                if (d_cache[set][way].dirty)
                {
                    printf("0x%08" PRIx32,
                           d_cache[set][way].dirty_address);
                }
                else
                {
                    printf("%-12s", "-");
                }

                putchar('\n');
            }
        }
    }

    if (!d_found)
    {
        printf("L1 Data Cache is empty.\n");
    }

    // =========================================
    // IN L1 INSTRUCTION CACHE
    // =========================================
    printf("\n================= L1 INSTRUCTION CACHE ================\n");

    printf("%-8s %-8s %-12s %-12s %-8s\n",
           "Set", "Way", "Line Addr", "Tag", "LRU");

    for (uint32_t set = 0; set < SET_CNT; set++)
    {
        for (uint32_t way = 0; way < I_CACHE_WAY_CNT; way++)
        {
            if (i_cache[set][way].valid)
            {
                i_found = 1;

                // Khôi phục địa chỉ đầu cache line từ tag và set
                uint32_t line_address =
                    (i_cache[set][way].tag << 20) | (set << 6);

                printf("%-8" PRIu32 " %-8" PRIu32
                       " 0x%08" PRIx32 "   0x%03" PRIx32 "        "
                       "%-8u\n",
                       set,
                       way,
                       line_address,
                       i_cache[set][way].tag,
                       i_cache[set][way].lru);
            }
        }
    }

    if (!i_found)
    {
        printf("L1 Instruction Cache is empty.\n");
    }

    printf("=======================================================\n");
}