#include <iostream>

#include "cacheUtils.h"
#include "analyzerUtils.h"

using namespace std;

inline string CacheProperties::assocTranslate(unsigned int assoc) const
{
    switch (assoc)
    {
    case 0:
        return "Fully associative";
    case 1:
        return "Direct mapped";
    default:
        return "N-way set associative";
    }
}

inline string CacheProperties::replaceTranslate(unsigned int replace) const
{
    switch (replace)
    {
    case 0:
        return "Random";
    case 1:
        return "LRU";
    default:
        return "none";
    }
}

inline string CacheProperties::writeTranslate(unsigned int write) const
{
    switch (write)
    {
    case 0:
        return "Write back & Write allocate";
    default:
        return "Write through & Not write allocate";
    }
}

void CacheProperties::printCacheProperties() const
{
    cout << "Cache size: " << i_cache_size << " KB" << endl;
    cout << "Cache block size: " << i_cache_block_size << " B (i.e. line size ignoring tag & flags)" << endl;
    cout << "Cache line size: " << i_cache_line_size << " B (i.e. line size including tag & flags)(round to upper bound)" << endl;
    cout << "Cache line number of each set: " << i_cache_set_line_num << endl;
    cout << "Cache set number: " << i_cache_set_num << endl;
    cout << "Cache line number: " << i_cache_line_num << endl;

    cout << "Type of association: " << assocTranslate(t_assoc) << endl;
    cout << "Type of replacement: " << replaceTranslate(t_replace) << endl;
    cout << "Type of write policy: " << writeTranslate(t_write) << endl;

    cout << "Miss penalty: " << i_miss_penalty << endl;

    cout << "Cache line size: " << bit_cache_line_size << " bit (i.e. line size including tag & flags)" << endl;
    cout << "Bits for line offset in address: " << bit_cache_line_offset_width << " bit" << endl;
    cout << "Bits for in-block offset in address: " << bit_block_offset_width << " bit" << endl;
    cout << "Bits for set offset in address: " << bit_cache_set_offset_width << " bit" << endl;
    cout << "Bits for tag in address: " << bit_cache_tag_width << " bit" << endl;
}

void InputUtilities::printInputProperties() const
{
    cout << "Configuration file direction: " << s_config_dir << endl;
    cout << "Trace file direction: " << s_trace_dir << endl;
    cout << "Output file direction: " << s_output_dir << endl;
}

Cache::Cache()
{
    i_cache_size = 0;
    i_cache_block_size = 0;
    i_cache_line_size = 0;
    i_cache_set_line_num = 0;
    i_cache_set_num = 0;
    i_cache_line_num = 0;

    t_assoc = FULLY_ASSOCIATIVE;
    t_replace = RANDOM;
    t_write = WRITE_THROUGH;

    i_miss_penalty = 0;

    bit_cache_line_offset_width = 0;
    bit_block_offset_width = 0;
    bit_cache_set_offset_width = 0;
    bit_cache_tag_width = 0;
    bit_cache_line_size = 0;

    cacheBody = new bitset<64>[MAX_CACHE_LINE_NUM]
    { 0 };
    LRU_priority = new unsigned long[MAX_CACHE_LINE_NUM]{0};
}

Cache::~Cache()
{
    delete[] cacheBody;
    delete[] LRU_priority;
}

Analyzer::Analyzer()
{
    i_num_cache_access = 0;
    i_num_cache_read = 0;
    i_num_cache_write = 0;

    i_num_cache_hit = 0;
    i_num_cache_read_hit = 0;
    i_num_cache_write_hit = 0;

    d_ave_rate = 0;
    d_read_rate = 0;
    d_write_rate = 0;

    current_access_line = 0;
    current_access_set = 0;
}

void Analyzer::printAnalyzerProperties() const
{
    cout << "Number of cache access: " << i_num_cache_access << endl;
    cout << "Number of cache read: " << i_num_cache_read << endl;
    cout << "Number of cache write: " << i_num_cache_write << endl;

    cout << "Number of cache hit: " << i_num_cache_hit << endl;
    cout << "Number of cache read hit: " << i_num_cache_read_hit << endl;
    cout << "Number of cache write hit: " << i_num_cache_write_hit << endl;

    cout << "Average cache hit rate: " << d_ave_rate << endl;
    cout << "Average cache read hit rate: " << d_read_rate << endl;
    cout << "Average cache write hit rate: " << d_write_rate << endl;

    cout << "Current access line: " << current_access_line << endl;
    cout << "Current access set: " << current_access_set << endl;
}

void Cache::printCacheInfo() const
{
    cout << "===============================================" << endl
         << "===================Cache Info==================" << endl;

    printCacheProperties();
    cout << "===============================================" << endl;
    // printCacheBody();
    // cout << "===============================================" << endl;
}

void Cache::printCacheBody() const
{
    cout << "===============================================" << endl
         << "===================Cache Body==================" << endl;
    for (int i = 0; i < i_cache_line_num; i++)
    {
        cout << "Cache line " << i << ": " << cacheBody[i] << endl;
    }
    cout << "===============================================" << endl;
}