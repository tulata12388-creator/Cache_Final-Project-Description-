#include "cache_core.h"
#include "cache_state.h"

void decode_address(uint32_t address, uint32_t *tag, uint32_t *set)
{
    // vì 2 cái trường của instruction với data giống nhau nên có thể dùng chung
   *tag = (address >> 20) & 0xFFF; // vì tag có 12 bit nên ta shift 20 bit và lấy 12 bit tiếp theo
   *set = (address >> 6) & 0x3FFF; // vì set có 14 bit nên ta shift 6 bit và lấy 14 bit tiếp theo
}
void hit_miss_flag(
    uint32_t tag, 
  	uint32_t set,
    uint8_t *is_hit, 
    uint8_t *filled_way_cnt, 
    uint8_t *accessed_way,
    uint8_t *empty_way,
    uint8_t cache_type   // 0: D, 1: I
)
{

  	// xác định Hit hay Miss
  	*is_hit = 0;         // biến cờ xác định hit hay miss
  	*filled_way_cnt = 0; // đếm số way đã được fill
  	*accessed_way = 0;   // way được truy cập
  	*empty_way = 0xFF;      // way trống đầu tiên 

    uint8_t way_cnt; 
    if (cache_type == 0) 
    {
        way_cnt = D_CACHE_WAY_CNT;
    } 
    else 
    {
        way_cnt = I_CACHE_WAY_CNT;
    }
    for (int way = 0; way < way_cnt; way++)
    {
        uint8_t valid;
        if (cache_type == 0) {
            valid = d_cache[set][way].valid;
        } else {
            valid = i_cache[set][way].valid;
        }

        if (valid) 
        {
            (*filled_way_cnt)++;

            uint32_t current_tag;
            if (cache_type == 0) {
                current_tag = d_cache[set][way].tag;
            } else {
                current_tag = i_cache[set][way].tag;
            }

            if (current_tag == tag) {
                *is_hit = 1;
                *accessed_way = way;
            }
        } 
      else if (*empty_way == 0xFF) 
        {
            *empty_way = way;
        }
    }
}
void clear_cache(void)
{
    // Xóa toàn bộ L1 Data Cache
    for (uint32_t set = 0; set < SET_CNT; set++)
    {
        for (uint32_t way = 0; way < D_CACHE_WAY_CNT; way++)
        {
            d_cache[set][way].valid         = 0;
            d_cache[set][way].dirty         = 0;
            d_cache[set][way].written_once  = 0;
            d_cache[set][way].tag           = 0;
            d_cache[set][way].lru           = 0;
            d_cache[set][way].dirty_address = 0;
        }
    }

    // Xóa toàn bộ L1 Instruction Cache
    for (uint32_t set = 0; set < SET_CNT; set++)
    {
        for (uint32_t way = 0; way < I_CACHE_WAY_CNT; way++)
        {
            i_cache[set][way].valid         = 0;
            i_cache[set][way].tag           = 0;
            i_cache[set][way].lru           = 0;
        }
    }

    // Reset statistics
    d_cache_read    = 0;
    d_cache_write   = 0;
    d_cache_hits    = 0;
    d_cache_misses  = 0;

    i_cache_read    = 0;
    i_cache_write   = 0;
    i_cache_hits    = 0;
    i_cache_misses  = 0;
}