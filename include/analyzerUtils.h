#ifndef _ANALYZER_UTILS
#define _ANALYZER_UTILS

#include <iostream>
#include <string>
#include <fstream>
#include <bitset>

#include "cacheUtils.h"

class Cache;

//////////////////

/** Cache behaviour analyzer **/
class Analyzer
{
protected:
    unsigned long i_num_cache_access; // total cache access count
    unsigned long i_num_cache_read;   // total cahce read count
    unsigned long i_num_cache_write;  // total cache write count

    unsigned long i_num_cache_hit;       // total cache hit count
    unsigned long i_num_cache_read_hit;  // total cache read hit count
    unsigned long i_num_cache_write_hit; // total cache write hit count

    double d_ave_rate;   // average cache hit rate
    double d_read_rate;  // average cache read hit rate
    double d_write_rate; // average cache write hit rate

protected:
    unsigned long current_access_line; // cache line currently accesssing
    unsigned long current_access_set;  // cache set currently accessing

public:
    Analyzer();
    void printAnalyzerProperties() const;
    void traceFileTest(Cache &, const std::string &traceFileDir);

private:
    bool runOneInstruction(Cache &, const std::string &instrucAddr, const std::string &instrucLine);
    bool isHit(Cache &, const std::bitset<48> &addr);
    void lruHitProcess(Cache &);
    void readMem(Cache &, std::bitset<48> &addr);
    void lruMissWithEmptyLine(Cache &);
    void lruMissWithNonEmptyLine(Cache &);
};

//////////////////

#endif // _ANALYZER_UTILS