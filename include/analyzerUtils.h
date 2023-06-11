#ifndef _ANALYZER_UTILS
#define _ANALYZER_UTILS

#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <cassert>
#include <limits>

#include "cacheUtils.h"

class Cache;

//////////////////

/** Cache behaviour analyzer **/
class Analyzer
{
    friend class Test;

protected:
    unsigned long i_num_cache_access; // total cache access count
    unsigned long i_num_cache_read;   // total cahce read count
    unsigned long i_num_cache_write;  // total cache write count

    unsigned long i_num_cache_hit;       // total cache hit count
    unsigned long i_num_cache_read_hit;  // total cache read hit count
    unsigned long i_num_cache_write_hit; // total cache write hit count

    unsigned long i_num_mem_read;  // total memory write count
    unsigned long i_num_mem_write; // total memory read count

    double d_ave_rate;   // average cache hit rate
    double d_read_rate;  // average cache read hit rate
    double d_write_rate; // average cache write hit rate

    unsigned long long i_total_cache_access_cycles; // total cache access cycles
    long double d_ave_cache_access_cycles;          // average cache access cycles

protected:
    unsigned long current_access_line; // cache line currently accesssing
    unsigned long current_access_set;  // cache set currently accessing
    unsigned long *LRU_priority;       // LRU priority for each line (mod in a set)

public:
    Analyzer();
    ~Analyzer();
    void printAnalyzerProperties() const;
    void traceFileTest(Cache &, const std::string &traceFileDir);
    void outputToFile(const std::string &outputFileDir) const;

protected:
    bool runOneInstruction(Cache &, const std::string &instrucAddr, const std::string &instrucLine);
    bool isHit(Cache &, const std::bitset<48> &addr);
    void lruHit(Cache &);
    void lruMiss_SpareLine(Cache &);
    void lruMiss_NoSpareLine(Cache &);
    void randomMiss(Cache &);
    void readMem(Cache &, const std::bitset<48> &addr);
    inline void writeMem(const Cache &, const std::bitset<48> &addr);
    void replace(Cache &, const std::bitset<48> &addr);
    void updateResult(const Cache &);
};

//////////////////

#endif // _ANALYZER_UTILS