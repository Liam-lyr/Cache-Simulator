#ifndef _BASE
#define _BASE

//////////////////

#define QUICK_MODE

//////////////////

#include <iostream>
#include <bitset>
#include <string>

//////////////////

#ifdef QUICK_MODE
#define MAX_CACHE_LINE_NUM 65536
#endif

#ifndef QUICK_MODE
#define MAX_CACHE_LINE_NUM 268435456 // max array size gcc supports (2^28)
#endif

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
    LRU
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
    unsigned int long i_cache_size;      // entire cache size (KB)
    unsigned int long i_cache_line_size; // cache line size (byte)
    unsigned int long i_cache_set_num;   // number of sets
    unsigned int long i_cache_line_num;  // number of cache lines
    Associativity t_assoc;               // associativity type
    ReplacementPolicy t_replace;         // replacement policy type
    WritePolicy t_write;                 // write policy type

    /** Properties for this cache **/
    /** will be calculated **/
    unsigned short bit_cache_line_size;  // cache line size (bit)
    unsigned short bit_cache_block_size; // cache block size (bit)
    unsigned short bit_cache_set_num;    // number of sets (bit)
    unsigned short bit_cache_tag;        // tag (bit)

protected:
    void printCacheProperties();
};

//////////////////

/** Properties of analyzer **/
class AnalyzerProperties
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

protected:
    void printAnalyzerProperties();
};

//////////////////

/** Cache body **/
class CacheBody
{
protected:
    std::bitset<64> *cacheBody;  // cache body
    unsigned long *LRU_priority; // LRU priority for each line (mod in a set)
};

//////////////////

/** Class registration **/

class InputUtilities
{
public:
    void getInput(int argc, char *argv[]);

private:
    void getCacheLineSize();
    void getCacheSize();
    void getMappingMethod();
    void getLineCountEachSet();
    void getReplacePolicy();
    void getWritePolicy();

protected:
    /** File reading properties **/
    std::string s_config_dir;   // configuration file direction
    std::string s_trace_dir;  // trace file direction
    std::string s_output_dir; // output file direction
public:
    void printInputProperties();
};

//////////////////

/** Entire cache **/
class Cache : public CacheProperties, public AnalyzerProperties, public CacheBody
{
    friend class InputUtilities; // InputUtilities should be able to access cache

public:
    Cache();
    ~Cache();
    void printCacheInfo();
    void printCacheBody();
};

#endif // _BASE