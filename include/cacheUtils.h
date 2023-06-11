#ifndef _CACHE
#define _CACHE

//////////////////

#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include <limits>
#include <cassert>
#include <climits>

#include "analyzerUtils.h"

//////////////////

#define MAX_CACHE_LINE_NUM 65536  // 2^16
#define MAX_CACHE_LINE_SIZE 65536 // 2^16

#define ADDRESS_WIDTH 48

//////////////////

#ifndef STRUCT_TYPE
#define STRUCT_TYPE
// associality
enum Associativity
{
    FULLY_ASSOCIATIVE,
    DIRECT_MAPPED,
    SET_ASSOCIATIVE
};
// replaceemnt policy
enum ReplacementPolicy
{
    RANDOM,
    LRU,
    none // for direct mapped cache
};
// write policy
enum WritePolicy
{
    WRITE_THROUGH, // hit - write through. miss - not write allocate
    WRITE_BACK     // hit - write back.    miss - write allocate
};

#endif // STRUCT_TYPE

//////////////////

/** Properties for this cache **/
/** get from command line**/
class CacheProperties
{
protected:
    unsigned long i_cache_size;         // entire cache size (KB)
    unsigned long i_cache_block_size;   // cache block size (Byte) (i.e. line size but ignoring tag & flags)
    unsigned long i_cache_line_size;    // cache line size (Byte) (i.e. line size including tag & flags, round to upper bound)
    unsigned long i_cache_set_line_num; // number of lines in each set (E)
    unsigned long i_cache_set_num;      // number of sets (S)
    unsigned long i_cache_line_num;     // number of cache lines (S * E)
    Associativity t_assoc;              // associativity type
    ReplacementPolicy t_replace;        // replacement policy type
    WritePolicy t_write;                // write policy type
    unsigned int i_miss_penalty;        // miss penalty (cycle)

    /** Properties for this cache **/
    /** will be calculated **/
    unsigned short bit_cache_line_offset_width; // bits for line offset in address (bit)
    unsigned short bit_block_offset_width;      // bits for in-block size in address (bit) (b)
    unsigned short bit_cache_set_offset_width;  // bits for set offset in address (bit) (s)
    unsigned short bit_cache_tag_width;         // bits for tag in address (bit)
    unsigned long long bit_cache_line_size;     // cache line size (bit) (i.e. line size including flags)

protected:
    void printCacheProperties() const;
    inline std::string assocTranslate(unsigned int) const;
    inline std::string replaceTranslate(unsigned int) const;
    inline std::string writeTranslate(unsigned int) const;
};

//////////////////

/** Cache body **/
class CacheBody
{
protected:
    std::bitset<64> *cacheBody; // cache body, a line will be divided into multiple cache items, each 1 byte
};

//////////////////

/** Class registration **/

class Cache;

class InputUtilities
{
public:
    void setCacheProperties(int argc, char *argv[], Cache &cache);

private:
    void setCacheBlockSize(Cache &, std::ifstream &) const;
    void setCacheSize(Cache &, std::ifstream &) const;
    void setAssociativity(Cache &, std::ifstream &) const;
    void setReplacePolicy(Cache &, std::ifstream &) const;
    void setWritePolicy(Cache &, std::ifstream &) const;
    void setMissPenalty(Cache &, std::ifstream &) const;
    void readConfig(int argc, char *argv[], Cache &cache);

protected:
    /** File reading properties **/
    std::string s_config_dir; // configuration file direction
    std::string s_trace_dir;  // trace file direction
    std::string s_output_dir; // output file direction
public:
    void printInputProperties() const;
    std::string getConfigDir() const { return s_config_dir; }
    std::string getTraceDir() const { return s_trace_dir; }
    std::string getOutputDir() const { return s_output_dir; }
};

//////////////////

class Analyzer;
/** Entire cache **/
class Cache : public CacheProperties, public CacheBody
{
    friend class InputUtilities; // InputUtilities should be able to access cache
    friend class Analyzer;       // Analyzer should be able to access cache

public:
    Cache();
    ~Cache();
    void printCacheInfo() const;
    void printCacheBody() const;

public:
    void cacheInit();

private:
    inline void clearCacheBody();
};

#endif // _CACHE