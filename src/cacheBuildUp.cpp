#include <iostream>

#include "cacheUtils.h"

using namespace std;

inline void Cache::clearCacheBody()
{
    for (unsigned long long i = 0; i < i_cache_line_num; i++)
    {
        cacheBody[i].reset();
        cacheBody[i][63] = 1;
        LRU_priority[i] = 0;
    }
}

void Cache::cacheBuildUp()
{
    clearCacheBody();
}