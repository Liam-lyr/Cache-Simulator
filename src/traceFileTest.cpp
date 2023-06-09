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

inline void setTag(bitset<64> &dstCacheItem, const bitset<48> &srcAddr, const unsigned long tagLen)
{
    for (unsigned long i = 0; i < tagLen; ++i)
    {
        dstCacheItem[60 - i] = srcAddr[ADDRESS_WIDTH - 1 - i];
    }
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

    // unsigned long long instrucCount = 0;
    while (getline(traceFile, instrucLine))
    {
        if(instrucLine == "#eof")
        {
            break;
        }
        instrucAddr = instrucLine.substr(0, 15); // instruction address
        instrucLine = instrucLine.substr(16);    // remove instruction address
        try
        {
            // cout << "line " << instrucCount << endl;
            // ++instrucCount;
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

    traceFile.close();

    updateResult(cache);
}

bool Analyzer::runOneInstruction(Cache &cache, const string &instrucAddr, const string &instrucLine)
{
    bool isRead = false; // Read or Write
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

    bitset<48> dataAddr = hexStr2Bit(instrucLine.substr(2));

    dataHit = isHit(cache, dataAddr);

    // data read hit
    try
    {
        if (dataHit && isRead)
        {
            i_num_cache_access++;
            i_num_cache_read++;
            i_num_cache_hit++;
            i_num_cache_read_hit++;
            if (cache.t_replace == LRU)
            {
                lruHit(cache);
            }
        }
        // data write hit
        else if (dataHit && !isRead)
        {
            i_num_cache_access++;
            i_num_cache_write++;
            i_num_cache_hit++;
            i_num_cache_write_hit++;
            if (cache.t_write == WRITE_THROUGH)
            {
                writeMem(cache, dataAddr);
            }
            else if (cache.t_write == WRITE_BACK)
            {
                cache.cacheBody[current_access_line].set(61); // dirty bit = 1
            }
            else
            {
                throw("Unknown write policy! At " + instrucAddr);
                return false;
            }

            if (cache.t_replace == LRU)
            {
                lruHit(cache);
            }
        }
        // data read miss
        else if (!dataHit && isRead)
        {
            i_num_cache_access++;
            i_num_cache_read++;
            readMem(cache, dataAddr);
        }
        // data write miss
        else if (!dataHit && !isRead)
        {
            i_num_cache_access++;
            i_num_cache_write++;
            // not write allocate
            if (cache.t_write == WRITE_THROUGH)
            {
                writeMem(cache, dataAddr);
            }
            // write allocate
            else if (cache.t_write == WRITE_BACK)
            {
                readMem(cache, dataAddr);
                cache.cacheBody[current_access_line].set(61); // dirty bit = 1
            }
            else
            {
                throw("Unknown write policy! At " + instrucAddr);
                return false;
            }
        }
        else
        {
            throw("Unknown error! At " + instrucAddr);
            return false;
        }
    }

    catch (const char *msg)
    {
        cerr << msg << " At instruction" << instrucAddr << endl;
        exit(1);
    }
    catch (exception &e)
    {
        cerr << e.what() << " At instruction" << instrucAddr << endl;
        exit(1);
    }
    catch (...)
    {
        cerr << "Unknown error! At instruction " << instrucAddr << endl;
        exit(1);
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

        // check tag same && valid = 1, if not equal then miss
        ret = cache.cacheBody[current_access_line].test(63);
        ret = checkSameSubBitSet(addr, cache.cacheBody[current_access_line], cache.bit_block_offset_width + cache.bit_cache_line_offset_width, ADDRESS_WIDTH, 61 - cache.bit_cache_tag_width, 61);
    }
    else if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        // travel all lines to check tag
        for (unsigned long i = 0; i < cache.i_cache_line_num; i++)
        {
            // check tag same && valid = 1, if not equal then miss
            ret = cache.cacheBody[i].test(63);
            ret = checkSameSubBitSet(addr, cache.cacheBody[i], cache.bit_block_offset_width, ADDRESS_WIDTH, 61 - cache.bit_cache_tag_width, 61);
            if (ret == true)
            {
                current_access_line = i;
                assert(current_access_line < cache.i_cache_line_num);
                break;
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
            // check tag same && valid = 1, if not equal then miss
            ret = cache.cacheBody[i].test(63);
            ret = checkSameSubBitSet(addr, cache.cacheBody[i], cache.bit_block_offset_width + cache.bit_cache_set_offset_width, ADDRESS_WIDTH, 61 - cache.bit_cache_tag_width, 61);
            if (ret == true)
            {
                current_access_line = i;
                assert(current_access_line < cache.i_cache_line_num);
                break;
            }
        }
    }
    else
    {
        throw("Unknown associativity!");
        return false;
    }

    return ret;
}

void Analyzer::readMem(Cache &cache, const bitset<48> &addr)
{
    i_num_mem_read++;
    if (cache.t_assoc == DIRECT_MAPPED)
    {
        // valid = 0, spare line
        if (!cache.cacheBody[current_access_line].test(63))
        {
            cache.cacheBody[current_access_line].set(63);   // valid = 1
            cache.cacheBody[current_access_line].reset(61); // dirty = 0
            setTag(cache.cacheBody[current_access_line], addr, cache.bit_cache_tag_width);
            return;
        }
        // valid = 1, no spare line
        replace(cache, addr);
    }
    else if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        for (unsigned long i = 0; i < cache.i_cache_line_num; i++)
        {
            // valid = 0, spare line
            if (!cache.cacheBody[i].test(63))
            {
                cache.cacheBody[i].set(63);   // valid = 1
                cache.cacheBody[i].reset(61); // dirty = 0
                setTag(cache.cacheBody[i], addr, cache.bit_cache_tag_width);
                current_access_line = i;
                if (cache.t_replace == LRU)
                {
                    lruMiss_SpareLine(cache);
                }
                return;
            }
        }
        // no spare line
        replace(cache, addr);
    }
    else if (cache.t_assoc == SET_ASSOCIATIVE)
    {
        // travel all lines in this set to check valid
        unsigned long base = current_access_set * cache.i_cache_set_line_num;
        for (unsigned long i = base; i < base + cache.i_cache_set_line_num; i++)
        {
            // valid = 0, spare line
            if (!cache.cacheBody[i].test(63))
            {
                cache.cacheBody[i].set(63);   // valid = 1
                cache.cacheBody[i].reset(61); // dirty = 0
                setTag(cache.cacheBody[i], addr, cache.bit_cache_tag_width);
                current_access_line = i;
                if (cache.t_replace == LRU)
                {
                    lruMiss_SpareLine(cache);
                }
                return;
            }
        }
        // no spare line
        replace(cache, addr);
    }
    else
    {
        throw("Unknown associativity!");
        return;
    }
}

inline void Analyzer::writeMem(const Cache &cache, const bitset<48> &addr)
{
    i_num_mem_write++;
}

void Analyzer::replace(Cache &cache, const bitset<48> &addr)
{
    if (cache.t_assoc == DIRECT_MAPPED)
    {
        // dirty = 1, therefore WRITE_BACK (WRITE_ALLOCATE), need to write back to memory when being replaced
        if (cache.cacheBody[current_access_line].test(61))
        {
            writeMem(cache, addr);
        }
        cache.cacheBody[current_access_line].set(63);   // valid = 1
        cache.cacheBody[current_access_line].reset(61); // dirty = 0
        setTag(cache.cacheBody[current_access_line], addr, cache.bit_cache_tag_width);
    }
    else if (cache.t_assoc == FULLY_ASSOCIATIVE || cache.t_assoc == SET_ASSOCIATIVE)
    {
        if (cache.t_replace == LRU)
        {
            lruMiss_NoSpareLine(cache);
        }
        else if (cache.t_replace == RANDOM)
        {
            randomMiss(cache);
        }
        else
        {
            throw("Unknown replacement policy!");
            return;
        }
        // dirty = 1, therefore WRITE_BACK (WRITE_ALLOCATE), need to write back to memory when being replaced
        if (cache.cacheBody[current_access_line].test(61))
        {
            writeMem(cache, addr);
        }
        cache.cacheBody[current_access_line].set(63);   // valid = 1
        cache.cacheBody[current_access_line].reset(61); // dirty = 0
        setTag(cache.cacheBody[current_access_line], addr, cache.bit_cache_tag_width);
    }
    else
    {
        throw("Unknown associativity!");
        return;
    }
}