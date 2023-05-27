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
extern unsigned int long i_cache_size;      // entire cache size (KB)
extern unsigned int long i_cache_line_size; // cache line size (byte)
extern unsigned int long i_cache_set_num;   // number of sets
extern unsigned int long i_cache_line_num;  // number of cache lines
extern Associativity t_assoc;               // associativity type
extern ReplacementPolicy t_replace;         // replacement policy type
extern WritePolicy t_write;                 // write policy type

/** Properties for this cache **/
/** will be calculated **/
extern unsigned short bit_cache_line_size;  // cache line size (bit)
extern unsigned short bit_cache_block_size; // cache block size (bit)
extern unsigned short bit_cache_set_num;    // number of sets (bit)
extern unsigned short bit_cache_tag;        // tag (bit)

//////////////////

/** Properties of analizer **/
extern unsigned long i_num_cache_access; // total cache access count
extern unsigned long i_num_cache_load;   // total cahce load count
extern unsigned long i_num_cache_store;  // total cache store count

extern unsigned long i_num_cache_hit;       // total cache hit count
extern unsigned long i_num_cache_load_hit;  // total cache load hit count
extern unsigned long i_num_cache_store_hit; // total cache store hit count

extern double d_ave_rate;   // average cache hit rate
extern double d_load_rate;  // average cache load hit rate
extern double d_store_rate; // average cache store hit rate

//////////////////

/** Cache body **/
extern std::bitset<64> cacheBody[MAX_CACHE_LINE_NUM];  // cache body (declaration)
extern unsigned long LRU_priority[MAX_CACHE_LINE_NUM]; // LRU priority for each line (mod in a set) (declaration)
extern unsigned long current_access_line;              // current access line
extern unsigned long current_access_set;               // current access set

//////////////////

/** File reading properties **/
extern std::string s_conf_direction;   // configuration file direction
extern std::string s_trace_direction;  // trace file direction
extern std::string s_output_direction; // output file direction

//////////////////

/** FUnctions registration **/
void Initialize();
void PrintProperties();

//////////////////

/** Class registration **/

class InputUtilities
{
public:
    void GetInput(int argc, char *argv[]);

private:
    void getCacheLineSize();
    void getCacheSize();
    void getMappingMethod();
    void getLineCountEachSet();
    void getReplacePolicy();
    void getWritePolicy();
};

#endif // _BASE