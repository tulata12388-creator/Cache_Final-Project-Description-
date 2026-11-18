#include "cache_operations.h"
#include "cache_core.h"
#include "cache_lru.h"
#include "cache_state.h"

#include <stdio.h>
#include <inttypes.h>

// CÁC HÀM CHO CÁC TRƯỜNG HỢP TRACE KHÁC NHAU
void read_data(uint32_t address)
{
    uint8_t is_hit;
    uint8_t filled_way_cnt;
    uint8_t accessed_way;
    uint8_t empty_way;
    uint32_t tag;
    uint32_t set;
    decode_address(address, &tag, &set);
    hit_miss_flag(tag, set, &is_hit, &filled_way_cnt, &accessed_way, &empty_way, 0);
    
    cache_read++;
    if (is_hit)
    {
        //Hit
		//cập nhật LRU
        update_D_LRU(set, accessed_way, 0);
        cache_hits++;
    }
  	else
    {
        // Miss
        cache_misses++;
      	if (filled_way_cnt == D_CACHE_WAY_CNT)
        {
          // nếu full way 
          // hàm update_D_LRU
          uint8_t victim_way;
          find_victim(set, &victim_way, 0);
          // Encode_address cua victim
        
          if (d_cache[set][victim_way].dirty)
          {
            if (mode == 1)
            {
                printf("Write to L2  0x%08" PRIx32 "\n", d_cache[set][victim_way].dirty_address);
                printf("Read from L2 0x%08" PRIx32 "\n", address); // read from L2
            }
            // reset dirty bit sau khi write back
            d_cache[set][victim_way].dirty = 0;
            d_cache[set][victim_way].tag = tag; // cập nhật tag mới 
            // hàm update_D_LRU
            update_D_LRU(set, victim_way, 0);  
          }
          else
          {
            if (mode == 1)
            {
                printf("Read from L2 0x%08" PRIx32 "\n", address);
            }
            // cập nhật tag mới
            d_cache[set][victim_way].tag = tag;
            // reset dirty bit (LRU = 0) sau khi write back
            d_cache[set][victim_way].dirty = 0;
            // hàm update_D_LRU
            update_D_LRU(set, victim_way, 0);
          }
        }
      	else
        {
          	// ghi vô cái empty line 
            if (mode == 1) 
            {
                printf("Read from L2 0x%08" PRIx32 "\n", address);
            }
            //hàm update_D_LRU 
            update_D_LRU(set, empty_way, 0);
            // cập nhật tag mới
            d_cache[set][empty_way].tag = tag;
            // đánh dấu valid bit;
            d_cache[set][empty_way].valid = 1;
            // đánh dấu dirty bit
            d_cache[set][empty_way].dirty = 0;
          	
            
        }
    }
}
void write_data(uint32_t address)
{
    uint8_t is_hit;
    uint8_t filled_way_cnt;
    uint8_t accessed_way;
    uint8_t empty_way;
    uint32_t tag;
    uint32_t set;
    decode_address(address, &tag, &set);
    hit_miss_flag(tag, set, &is_hit, &filled_way_cnt, &accessed_way, &empty_way, 0);
  
    cache_write++;
    if (is_hit)
    {
        //hit
        cache_hits++;
        d_cache[set][accessed_way].dirty = 1; // đánh dấu dirty bit
      	update_D_LRU(set, accessed_way, 0);
        // có một biến để lưu địa chỉ bị dirty, khi bị evict thì sẽ write back ra L2
        d_cache[set][accessed_way].dirty_address = address; 
    }
  	else
    {
      //Miss
      cache_misses++;
      if (filled_way_cnt == D_CACHE_WAY_CNT)
        {
          // nếu full way 
          // hàm update_D_LRU
          // hàm tìm victim
           uint8_t victim_way;
           find_victim(set, &victim_way, 0);
          // Encode_address cua victim
         if(d_cache[set][victim_way].dirty)
         {
            if (mode == 1) 
            {
                printf("Write to L2  0x%08" PRIx32 "\n", d_cache[set][victim_way].dirty_address);
                printf("Read for Ownership from L2 0x%08" PRIx32 "\n", address);
            }
           d_cache[set][victim_way].tag = tag; // cập nhật tag mới
           d_cache[set][victim_way].dirty_address = address; 
              
		   update_D_LRU(set, victim_way, 0); // reset dirty bit (LRU = 0) sau khi write back 
         }
        else
         {
            if (mode == 1) 
            {
                printf("Read for Ownership from L2 0x%08" PRIx32 "\n", address);
            }
           // dirty = 1
           d_cache[set][victim_way].tag = tag; // cập nhật tag mới
           d_cache[set][victim_way].dirty = 1;
           d_cache[set][victim_way].dirty_address = address;
           update_D_LRU(set, victim_way, 0);
         }
        }
      	else
        {
            if (mode == 1) 
            {
                printf("Read for Ownership from L2 0x%08" PRIx32 "\n", address);
            }
          	// ghi vô cái empty line  
            //hàm update_D_LRU 
          	update_D_LRU(set, empty_way, 0);
            d_cache[set][empty_way].tag = tag; // cập nhật tag mới
            d_cache[set][empty_way].valid = 1; // đánh dấu valid bit;
            //d_cache[set][empty_way].dirty = 1; // đánh dấu dirty bit
            //d_cache[set][empty_way].dirty_address = address; 
          	
            
        }
    }
}
void fetch_instruction(uint32_t address)
{
    uint8_t is_hit;
    uint8_t filled_way_cnt;
    uint8_t accessed_way;
    uint8_t empty_way;
    uint32_t tag;
    uint32_t set;
    decode_address(address, &tag, &set);
    hit_miss_flag(tag, set, &is_hit, &filled_way_cnt, &accessed_way, &empty_way, 1);
    cache_read++;
    if (is_hit)
    {
        //Hit
        cache_hits++;
		//cập nhật LRU
        update_I_LRU(set, accessed_way, 0);
    }
  	else
    {
        // Miss
        cache_misses++;
      	if (filled_way_cnt == I_CACHE_WAY_CNT)
        {
            // nếu full way 
            // hàm update_D_LRU
            uint8_t victim_way;
            find_victim(set, &victim_way, 1);
            // Encode_address cua victim
            if (mode == 1) 
            {
                printf("Read from L2 0x%08" PRIx32 "\n", address);
            }
            
            // cập nhật tag mới
            i_cache[set][victim_way].tag = tag;
            // hàm update_I_LRU
            update_I_LRU(set, victim_way, 0);
        }
      	else
        {
            if (mode == 1) 
            {
                // ghi vô cái empty line 
                printf("Read from L2 0x%08" PRIx32 "\n", address);
            }
          	
            //hàm update_D_LRU 
          	update_I_LRU(set, empty_way, 0);
            // cập nhật tag mới
            i_cache[set][empty_way].tag = tag;
            // đánh dấu valid bit;
            i_cache[set][empty_way].valid = 1;
          	
            
        }
    }
}
void evict_L2(uint32_t address)
{
    uint32_t tag;
    uint32_t set;
    decode_address(address, &tag, &set);
    // tìm victim trong D cache 
    for (int way = 0; way < D_CACHE_WAY_CNT; way++)
    {
        if (d_cache[set][way].valid && d_cache[set][way].tag == tag)
        {
            if (d_cache[set][way].dirty)
            {
                if (mode == 1) 
                {
                    printf("Write to L2  0x%08" PRIx32 "\n", d_cache[set][way].dirty_address);
                }
                
                
            }
            // reset dirty bit và valid bit
            update_D_LRU(set, way, 1); // cập nhật LRU
            d_cache[set][way].dirty = 0;
            d_cache[set][way].valid = 0;
            d_cache[set][way].tag = 0;
            d_cache[set][way].dirty_address = 0;
            break;
        }
    }
    for (int way = 0; way < I_CACHE_WAY_CNT; way++)
    {
        if (i_cache[set][way].valid && i_cache[set][way].tag == tag)
        {
            // reset valid bit
            update_I_LRU(set, way, 1); // cập nhật LRU
            i_cache[set][way].valid = 0;
            i_cache[set][way].tag = 0;
            break;
        }
    }
}