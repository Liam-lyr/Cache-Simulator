#ifndef _ANALYZER_UTILS
#define _ANALYZER_UTILS

#include <iostream>
#include "cacheUtils.h"

//////////////////

/** Cache behaviour analyzer **/
class Analyzer
{
protected:
    unsigned long i_num_cache_access; // total cache access count
    unsigned long i_num_cache_load;   // total cahce load count
    unsigned long i_num_cache_store;  // total cache store count

    unsigned long i_num_cache_hit;       // total cache hit count
    unsigned long i_num_cache_load_hit;  // total cache load hit count
    unsigned long i_num_cache_store_hit; // total cache store hit count

    double d_ave_rate;   // average cache hit rate
    double d_load_rate;  // average cache load hit rate
    double d_store_rate; // average cache store hit rate

protected:
    unsigned long current_access_line; // current access line
    unsigned long current_access_set;  // current access set

public:
    Analyzer();

public:
    void printAnalyzerProperties() const;
};

//////////////////

#endif // _ANALYZER_UTILS