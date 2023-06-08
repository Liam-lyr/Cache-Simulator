#include <iostream>
#include <random>
#include <exception>

#include "cacheUtils.h"
#include "analyzerUtils.h"

using namespace std;

void Analyzer::lruHit(Cache &cache)
{
    // nothing to do with DIRECT_MAPPED, cuz replace policy is none

    if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        // increase the age of line whose age is less than current line by 1
        for (unsigned long i = 0; i < cache.i_cache_line_num; ++i)
        {
            // age is less than current line && not spare (valid = 1)
            if (LRU_priority[i] < LRU_priority[current_access_line] && cache.cacheBody[i].test(63))
            {
                LRU_priority[i]++;
            }
        }
    }
    else if (cache.t_assoc == SET_ASSOCIATIVE)
    {
        // increase the age of line whose age is less than current line by 1
        unsigned long base = current_access_set * cache.i_cache_set_line_num;
        for (unsigned long i = base; i < base + cache.i_cache_set_line_num; ++i)
        {
            // age is less than current line && not spare (valid = 1)
            if (LRU_priority[i] < LRU_priority[current_access_line] && cache.cacheBody[i].test(63))
            {
                LRU_priority[i]++;
            }
        }
    }
    else
    {
        cerr << "LRUHitProcess: cache type error! LRU used in Direct-Mapped or association invalid." << endl;
        exit(1);
    }

    LRU_priority[current_access_line] = 0;
}

void Analyzer::lruMiss_SpareLine(Cache &cache)
{
    if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        for (unsigned long i = 0; i < cache.i_cache_line_num; i++)
        {
            // increase all no spare line's age by 1
            if (cache.cacheBody[i].test(63))
            {
                LRU_priority[i]++;
            }
        }
        LRU_priority[current_access_line] = 0;
    }
    else if (cache.t_assoc == SET_ASSOCIATIVE)
    {
        unsigned long base = current_access_set * cache.i_cache_set_line_num;
        for (unsigned long i = base; i < base + cache.i_cache_set_line_num; i++)
        {
            // increase all no spare line's age by 1
            if (cache.cacheBody[i].test(63))
            {
                LRU_priority[i]++;
            }
        }
        LRU_priority[current_access_line] = 0;
    }
    else
    {
        cerr << "LRUMissProcess_SpareLine: cache type error! LRU used in Direct-Mapped or association invalid." << endl;
        exit(1);
    }
}

void Analyzer::lruMiss_NoSpareLine(Cache &cache)
{
    if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        bool found = false;
        // find oldest line to replace
        // no spare line, so all i_cache_line_num lines are not spare (all valid = 1)
        for (unsigned long i = 0; i < cache.i_cache_line_num; i++)
        {
            unsigned long oldestAge = cache.i_cache_line_num - 1;
            if (LRU_priority[i] == oldestAge)
            {
                found = true;
                current_access_line = i;
                LRU_priority[i] = 0;
            }
            else
            {
                LRU_priority[i]++;
            }
        }
        if (found == false)
        {
            cerr << "LRUMissProcess_NoSpareLine: no spare line, but no line has age = i_cache_line_num - 1" << endl;
        }
    }
    else if (cache.t_assoc == SET_ASSOCIATIVE)
    {
        bool found = false;
        unsigned long base = current_access_set * cache.i_cache_set_line_num;
        // find oldest line to replace
        // no spare line, so all i_cache_line_num lines are not spare (all valid = 1)
        for (unsigned long i = base; i < base + cache.i_cache_set_line_num; i++)
        {
            unsigned long oldestAge = cache.i_cache_set_line_num - 1;
            if (LRU_priority[i] == oldestAge)
            {
                found = true;
                current_access_line = i;
                LRU_priority[i] = 0;
            }
            else
            {
                LRU_priority[i]++;
            }
        }
        if (found == false)
        {
            cerr << "LRUMissProcess_NoSpareLine: no spare line, but no line has age = i_cache_set_num - 1" << endl;
        }
    }
    else
    {
        cerr << "LRUMissProcess_NoSpareLine: cache type error! LRU used in Direct-Mapped or association invalid." << endl;
        exit(1);
    }
}

void Analyzer::randomMiss(Cache &cache)
{
    if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        // find a random line to replace
        static uniform_int_distribution<unsigned long> u(0, cache.i_cache_line_num - 1);
        static default_random_engine e;
        current_access_line = u(e);
    }
    else if (cache.t_assoc == SET_ASSOCIATIVE)
    {
        // find a random line to replace
        static uniform_int_distribution<unsigned long> u(0, cache.i_cache_set_line_num - 1);
        static default_random_engine e;
        current_access_line = current_access_set * cache.i_cache_set_line_num + u(e);
    }
    else
    {
        cerr << "RandomMissProcess: cache type error! Random used in Direct-Mapped or association invalid." << endl;
        exit(1);
    }
}