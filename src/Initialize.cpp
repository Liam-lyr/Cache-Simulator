#include <iostream>
#include "base.h"

using namespace std;

void Initialize()
{
    i_cache_size = 0;
    i_cache_line_size = 0;
    i_cache_set_num = 0;
    i_cache_line_num = 0;

    t_assoc = FULLY_ASSOCIATIVE;
    t_replace = RANDOM;
    t_write = WRITE_THROUGH;

    bit_cache_line_size = 0;
    bit_cache_block_size = 0;
    bit_cache_set_num = 0;
    bit_cache_tag = 0;

    i_num_cache_access = 0;
    i_num_cache_load = 0;
    i_num_cache_store = 0;

    i_num_cache_hit = 0;
    i_num_cache_load_hit = 0;
    i_num_cache_store_hit = 0;

    d_ave_rate = 0;
    d_load_rate = 0;
    d_store_rate = 0;

    std::bitset<64> cacheBody[MAX_CACHE_LINE_NUM];  // definition of cache body
    unsigned long LRU_priority[MAX_CACHE_LINE_NUM]; // definition of LRU priority for each line (mod in a set)
    current_access_line = 0;
    current_access_set = 0;

    for (int i = 0; i < i_cache_line_num; i++)
    {
        cacheBody[i].reset();
        if (t_replace == LRU)
            cacheBody[i] = 0;
    }
}