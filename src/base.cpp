#include <iostream>
#include "base.h"

using namespace std;

void CacheProperties::printCacheProperties()
{
    cout << "Cache size: " << i_cache_size << "KB" << endl;
    cout << "Cache line size: " << i_cache_line_size << "B" << endl;
    cout << "Cache set number: " << i_cache_set_num << endl;
    cout << "Cache line number: " << i_cache_line_num << endl;

    cout << "Type of association: " << t_assoc << endl;
    cout << "Type of replacement: " << t_replace << endl;
    cout << "Type of write policy: " << t_write << endl;

    cout << "Cache line size in bits: " << bit_cache_line_size << endl;
    cout << "Cache block size in bits: " << bit_cache_block_size << endl;
    cout << "Cache set number in bits: " << bit_cache_set_num << endl;
    cout << "Cache tag in bits: " << bit_cache_tag << endl;
}

void AnalyzerProperties::printAnalyzerProperties()
{
    cout << "Number of cache access: " << i_num_cache_access << endl;
    cout << "Number of cache load: " << i_num_cache_load << endl;
    cout << "Number of cache store: " << i_num_cache_store << endl;

    cout << "Number of cache hit: " << i_num_cache_hit << endl;
    cout << "Number of cache load hit: " << i_num_cache_load_hit << endl;
    cout << "Number of cache store hit: " << i_num_cache_store_hit << endl;

    cout << "Average cache hit rate: " << d_ave_rate << endl;
    cout << "Average cache load hit rate: " << d_load_rate << endl;
    cout << "Average cache store hit rate: " << d_store_rate << endl;

    cout << "Current access line: " << current_access_line << endl;
    cout << "Current access set: " << current_access_set << endl;
}

void InputUtilities::printInputProperties()
{
    cout << "Configuration file direction: " << s_config_dir << endl;
    cout << "Trace file direction: " << s_trace_dir << endl;
    cout << "Output file direction: " << s_output_dir << endl;
}

Cache::Cache()
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

    cacheBody = new bitset<64>[MAX_CACHE_LINE_NUM]
    { 0 };
    LRU_priority = new unsigned long[MAX_CACHE_LINE_NUM]{0};

    current_access_line = 0;
    current_access_set = 0;
}

Cache::~Cache()
{
    delete[] cacheBody;
    delete[] LRU_priority;
}

void Cache::printCacheInfo()
{
    cout << "===============================================" << endl
         << "===================Cache Info==================" << endl;

    printCacheProperties();
    cout << "===============================================" << endl;
    printAnalyzerProperties();
    cout << "===============================================" << endl;
    printCacheBody();
    cout << "===============================================" << endl;
}

void Cache::printCacheBody()
{
    for (int i = 0; i < i_cache_line_num; i++)
    {
        cout << "Cache line " << i << ": " << cacheBody[i] << endl;
    }
}