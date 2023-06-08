#include <iostream>
#include <bitset>
#include <string>
#include "cacheUtils.h"
#include "analyzerUtils.h"

using namespace std;

inline bitset<48> hexStr2Bit(const string &hexStr)
{
    return bitset<48>(stoll(hexStr, nullptr, 16));
}

// (end, start], bit wise so low bit is on the right
inline bitset<48> subBitSet(const bitset<48> &ori, unsigned short start, unsigned short end)
{
    unsigned long long mask = (1ULL << (end - start)) - 1;
    return bitset<48>((ori.to_ullong() >> start) & mask);
}

inline bitset<48> subBitSet(const bitset<64> &ori, unsigned short start, unsigned short end)
{
    unsigned long long mask = (1ULL << (end - start)) - 1;
    return bitset<48>((ori.to_ullong() >> start) & mask);
}

// (end, start], bit wise so low bit is on the right
inline bool checkSameSubBitSet(const bitset<48> &addr, const bitset<64> &cacheItem, unsigned short addrStart, unsigned short addrEnd, unsigned short itemStart, unsigned short itemEnd)
{
    return (subBitSet(addr, addrStart, addrEnd) == subBitSet(cacheItem, itemStart, itemEnd));
}

class Test
{
public:
    void testAnalyzer(int argc, char *argv[]);
};

void Test::testAnalyzer(int argc, char *argv[])
{
    Cache cache;
    InputUtilities inputUtil;
    Analyzer analyzer;

    inputUtil.setCacheProperties(argc, argv, cache);
    inputUtil.printInputProperties();
    cache.printCacheInfo();
    analyzer.printAnalyzerProperties();
    string traceDir = inputUtil.getTraceDir();
    string outputDir = inputUtil.getOutputDir();
    cache.cacheInit();

    string hexStr;
    cin >> hexStr;
    bitset<48> addr = hexStr2Bit(hexStr);
    cout << addr << endl;

    ////////////////////////

    unsigned long &current_access_set = analyzer.current_access_set;
    unsigned long &current_access_line = analyzer.current_access_line;
    bool ret = false;

    current_access_set = subBitSet(addr, cache.bit_block_offset_width, cache.bit_block_offset_width + cache.bit_cache_set_offset_width).to_ulong();
    assert(current_access_set < cache.i_cache_set_num);

    bitset<64> testCacheItem = bitset<64>("1100111111111111111110010010111001001111111111111111100100101110");

    // travel all lines in this set to check tag
    unsigned long base = current_access_set * cache.i_cache_set_line_num;

    if (testCacheItem[62] == 1) // check hit bit, if 1 then check tag
    {
        ret = checkSameSubBitSet(addr, testCacheItem, cache.bit_block_offset_width + cache.bit_cache_set_offset_width, ADDRESS_WIDTH, 61 - cache.bit_cache_tag_width, 61);
        if (ret == true)
        {
            // current_access_line = ;
            assert(current_access_line < cache.i_cache_line_num);
            assert(testCacheItem[63] == 1); // valid bit must be 1
            // break;
        }
    }

    cout << current_access_line << endl
         << current_access_set << endl
         << ret << endl;
}