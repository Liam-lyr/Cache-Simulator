#include <iostream>
#include <string>
#include <fstream>

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

void Analyzer::traceFileTest(Cache &cache, const string &traceFileDir)
{
    ifstream traceFile(traceFileDir);
    if (!traceFile.is_open())
    {
        cerr << "Cannot open trace file!" << endl;
        exit(1);
    }

    // read each instruction line and run it
    string instrucLine, instrucAddr;
    bool isSucc;
    while (getline(traceFile, instrucLine))
    {
        instrucAddr = instrucLine.substr(0, 15); // instruction address
        instrucLine = instrucLine.substr(16);    // remove instruction address
        try
        {
            isSucc = runOneInstruction(cache, instrucAddr, instrucLine);
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
            exit(1);
        }
        catch (const char *msg)
        {
            cerr << msg << endl;
            exit(1);
        }
        catch (...)
        {
            cerr << "Unknown error!" << endl;
            exit(1);
        }
    }
}

bool Analyzer::runOneInstruction(Cache &cache, const string &instrucAddr, const string &instrucLine)
{
    bool isRead = false; // Read or
    bool dataHit = false;

    switch (instrucLine[0])
    {
    case 'R':
        isRead = true;
        break;
    case 'W':
        isRead = false;
        break;
    default:
        throw("Invalid instruction! At " + instrucAddr);
        return false;
    }

    bitset<48> dataAddr = hexStr2Bit(instrucLine);

    /* data behavior */

    dataHit = isHit(cache, dataAddr);

    // data read hit
    if (dataHit && isRead)
    {
        i_num_cache_access++;
        i_num_cache_read++;
        i_num_cache_hit++;
        i_num_cache_read_hit++;
        if (cache.t_replace == LRU)
        {
            lruHitProcess(cache);
        }
    }
    // data write hit
    else if (dataHit && !isRead)
    {
        i_num_cache_access++;
        i_num_cache_write++;
        i_num_cache_hit++;
        i_num_cache_write_hit++;
        cache.cacheBody[current_access_line][61] = 1; // dirty bit set to 1 cuz simulator only, no matter the real write policy
        if (cache.t_replace == LRU)
        {
            lruHitProcess(cache);
        }
    }
    // data read miss
    else if (!dataHit && isRead)
    {
        i_num_cache_access++;
        i_num_cache_read++;
        readMem(cache, dataAddr);
        if (cache.t_replace == LRU)
        {
            lruMissWithEmptyLine(cache);
        }
    }
    // data write miss
    else if (!dataHit && !isRead)
    {
        i_num_cache_access++;
        i_num_cache_write++;
        readMem(cache, dataAddr);
        cache.cacheBody[current_access_line][61] = 1; // dirty bit set to 1 cuz simulator only, no matter the real write policy
        if (cache.t_replace == LRU)
        {
            lruMissWithEmptyLine(cache);
        }
    }
    else
    {
        throw("Unknown error! At " + instrucAddr);
        return false;
    }

    return true;
}

bool Analyzer::isHit(Cache &cache, const bitset<48> &addr)
{
    bool ret = false;
    if (cache.t_assoc == DIRECT_MAPPED)
    {
        // get current access line
        current_access_line = subBitSet(addr, cache.bit_block_offset_width, cache.bit_block_offset_width + cache.bit_cache_line_offset_width).to_ulong();
        assert(current_access_line < cache.i_cache_line_num);
        assert(cache.cacheBody[current_access_line][63] == 1); // valid bit must be 1

        if (cache.cacheBody[current_access_line][62] == 1) // check hit bit, if 1 then check tag
        {
            // check tag same, if not equal then miss
            ret = checkSameSubBitSet(addr, cache.cacheBody[current_access_line], cache.bit_block_offset_width + cache.bit_cache_line_offset_width, ADDRESS_WIDTH, 61 - cache.bit_cache_tag_width, 61);
        }
    }
    else if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        // travel all lines to check tag
        for (unsigned long i = 0; i < cache.i_cache_line_num; i++)
        {
            if (cache.cacheBody[i][62] == 1) // check hit bit, if 1 then check tag
            {
                ret = checkSameSubBitSet(addr, cache.cacheBody[i], cache.bit_block_offset_width, ADDRESS_WIDTH, 61 - cache.bit_cache_tag_width, 61);
                if (ret == true)
                {
                    current_access_line = i;
                    assert(current_access_line < cache.i_cache_line_num);
                    assert(cache.cacheBody[current_access_line][63] == 1); // valid bit must be 1
                    break;
                }
            }
        }
    }
    else if (cache.t_assoc == SET_ASSOCIATIVE)
    {
        // get current access set
        current_access_set = subBitSet(addr, cache.bit_block_offset_width, cache.bit_block_offset_width + cache.bit_cache_set_offset_width).to_ulong();
        assert(current_access_set < cache.i_cache_set_num);

        // travel all lines in this set to check tag
        unsigned long base = current_access_set * cache.i_cache_set_line_num;
        for (unsigned long i = base; i < base + cache.i_cache_set_line_num; i++)
        {
            if (cache.cacheBody[i][62] == 1) // check hit bit, if 1 then check tag
            {
                ret = checkSameSubBitSet(addr, cache.cacheBody[i], cache.bit_block_offset_width + cache.bit_cache_set_offset_width, ADDRESS_WIDTH, 61 - cache.bit_cache_tag_width, 61);
                if (ret == true)
                {
                    current_access_line = i;
                    assert(current_access_line < cache.i_cache_line_num);
                    assert(cache.cacheBody[current_access_line][63] == 1); // valid bit must be 1
                    break;
                }
            }
        }
    }
    else
    {
        throw("Unknown error!");
        return false;
    }

    return ret;
}