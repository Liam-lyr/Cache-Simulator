#ifndef _BASE
#define _BASE

//////////////////

#define QUICK_MODE

//////////////////

#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include <limits>

//////////////////

#ifdef QUICK_MODE
#define MAX_CACHE_LINE_NUM 65536   // 2^16
#define MAX_CACHE_LINE_SIZE 262144 // 2^18
#endif

#ifndef QUICK_MODE
#define MAX_CACHE_LINE_NUM 268435456 // max array size gcc supports (2^28)
#define MAX_CACHE_LINE_SIZE ULONG_MAX
#endif

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
    WRITE_BACK,   // hit - write back.    miss - write allocate
    WRITE_THROUGH // hit - write through. miss - not write allocate
};

#endif // STRUCT_TYPE

//////////////////

/** Properties for this cache **/
/** get from command line**/
class CacheProperties
{
protected:
    unsigned long i_cache_size;         // entire cache size (KB)
    unsigned long i_cache_line_size;    // cache line size (Byte) (i.e. block size. ignore flags)
    unsigned long i_cache_set_line_num; // number of lines in each set (E)
    unsigned long i_cache_set_num;      // number of sets (S)
    unsigned long i_cache_line_num;     // number of cache lines (S * E)
    Associativity t_assoc;              // associativity type
    ReplacementPolicy t_replace;        // replacement policy type
    WritePolicy t_write;                // write policy type
    unsigned int i_miss_penalty;        // miss penalty (cycle)

    /** Properties for this cache **/
    /** will be calculated **/
    unsigned short bit_cache_line_offset_width; // bits for in-line offset in address (bit)
    unsigned short bit_block_offset_width;      // bits for in-block size in address (bit) (b)
    unsigned short bit_cache_set_offset_width;  // bits for in-set offset in address (bit) (s)
    unsigned short bit_cache_tag_width;         // bits for tag in address (bit)

protected:
    void printCacheProperties() const;
    inline std::string assocTranslate(unsigned int) const;
    inline std::string replaceTranslate(unsigned int) const;
    inline std::string writeTranslate(unsigned int) const;
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
    void printAnalyzerProperties() const;
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

class Cache;

class InputUtilities
{
public:
    void setCacheProperties(int argc, char *argv[], Cache &cache);

private:
    void setCacheLineSize(Cache &, std::ifstream &) const;
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
};

//////////////////

/** Entire cache **/
class Cache : public CacheProperties, public AnalyzerProperties, public CacheBody
{
    friend class InputUtilities; // InputUtilities should be able to access cache

public:
    Cache();
    ~Cache();
    void printCacheInfo() const;
    void printCacheBody() const;

public:
    void cacheBuildUp();
};

#endif // _BASE