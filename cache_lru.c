#include "cache_lru.h"
#include "cache_state.h"

void find_victim(uint32_t set, uint8_t *victim_way, uint8_t cache_type)
{
    uint8_t way_cnt;
    if (cache_type == 0) 
    {
        way_cnt = D_CACHE_WAY_CNT;
    } 
    else 
    {
        way_cnt = I_CACHE_WAY_CNT;
    }

    uint8_t max_lru = 0;
    *victim_way = 0;

    for (int way = 0; way < way_cnt; way++)
    {
        uint8_t current_lru;
        if (cache_type == 0) {
            current_lru = d_cache[set][way].lru;
        } else {
            current_lru = i_cache[set][way].lru;
        }

        if (current_lru > max_lru) {
            max_lru = current_lru;
            *victim_way = way;
        }
    }
}
// CÁC HÀM CHO CHÍNH SÁCH LRU

// LRU cho instruction cache
void update_D_LRU(uint32_t set, uint32_t accessed_way, uint8_t operation)
{
    uint8_t old_lru = d_cache[set][accessed_way].valid ? d_cache[set][accessed_way].lru : D_CACHE_WAY_CNT;
    if (operation == 0) 
    {
        for (int way = 0; way < D_CACHE_WAY_CNT; way++)
        {
            if (way == accessed_way)    
            {
                continue; // Không cần cập nhật LRU cho way được truy cập
            }
            if (d_cache[set][way].valid && 
                d_cache[set][way].lru < old_lru)
            {
                d_cache[set][way].lru++;
            }
        }
        d_cache[set][accessed_way].lru = 0; // Đặt LRU của way được truy cập về 0
    }
    else if (operation == 1) 
    {
        for (int way = 0; way < D_CACHE_WAY_CNT; way++)
        {
            if (way == accessed_way)    
            {
                continue; // Không cần cập nhật LRU cho way được truy cập
            }
            if (d_cache[set][way].valid && 
                d_cache[set][way].lru > old_lru)
            {
                d_cache[set][way].lru--;
            }
        }
    }
}
// LRU cho instruction cache
void update_I_LRU(uint32_t set, uint32_t accessed_way, uint8_t operation)
{
    uint8_t old_lru = i_cache[set][accessed_way].valid ? i_cache[set][accessed_way].lru : I_CACHE_WAY_CNT;
    if (operation == 0) 
    {
        for (int way = 0; way < I_CACHE_WAY_CNT; way++)
        {
            if (way == accessed_way)    
            {
                continue; // Không cần cập nhật LRU cho way được truy cập
            }
            if (i_cache[set][way].valid && 
                i_cache[set][way].lru < old_lru)
            {
                i_cache[set][way].lru++;
            }
        }
        i_cache[set][accessed_way].lru = 0; // Đặt LRU của way được truy cập về 0
    }
    else if (operation == 1) 
    {
        for (int way = 0; way < I_CACHE_WAY_CNT; way++)
        {
            if (way == accessed_way)    
            {
                continue; // Không cần cập nhật LRU cho way được truy cập
            }
            if (i_cache[set][way].valid && 
                i_cache[set][way].lru > old_lru)
            {
                i_cache[set][way].lru--;
            }
        }
    }
}