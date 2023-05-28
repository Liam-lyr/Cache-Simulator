#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <exception>
#include <limits>
#include "base.h"

using namespace std;

void InputUtilities::setCacheSize(Cache &cache, ifstream &configFile) const
{
    string line;
    unsigned long temp;
    getline(configFile, line);

    temp = (unsigned long)(stol(line));

    if (temp < 1 || temp >= ULONG_MAX || (temp & (~temp + 1)) != temp)
    {
        throw("Cache size is not valid!");
    }
    cache.i_cache_size = temp;
}

void InputUtilities::setCacheLineSize(Cache &cache, ifstream &configFile) const
{
    string line;
    unsigned long temp;
    getline(configFile, line);

    temp = (unsigned long)(stol(line));

    if (temp < 1 || temp >= MAX_CACHE_LINE_SIZE || (temp & (~temp + 1)) != temp)
    {
        throw("Cache line size is not valid!");
    }

    cache.i_cache_line_size = temp;
}

void InputUtilities::setAssociativity(Cache &cache, ifstream &configFile) const
{
    string line;
    unsigned long temp;
    getline(configFile, line);

    temp = (unsigned long)(stol(line));

    switch (temp)
    {
    case 0:
        cache.t_assoc = FULLY_ASSOCIATIVE;
        break;
    case 1:
        cache.t_assoc = DIRECT_MAPPED;
        break;
    default:
        if (temp < 1 || temp >= MAX_CACHE_LINE_NUM || (temp & (~temp + 1)) != temp)
        {
            throw("Associativity is not valid!");
        }
        cache.t_assoc = SET_ASSOCIATIVE;
        cache.i_cache_set_line_num = temp;
    }
}

void InputUtilities::setReplacePolicy(Cache &cache, ifstream &configFile) const
{
    string line;
    unsigned short temp;
    getline(configFile, line);

    temp = (unsigned short)(stoi(line));

    switch (temp)
    {
    case 0:
        cache.t_replace = RANDOM;
        break;
    case 1:
        cache.t_replace = LRU;
        break;
    default:
        throw("Replacement policy is not valid!");
    }
}

void InputUtilities::setMissPenalty(Cache &cache, ifstream &configFile) const
{
    string line;
    unsigned temp;
    getline(configFile, line);

    temp = (unsigned)(stoi(line));

    if (temp < 1 || temp >= UINT32_MAX)
    {
        throw("Miss penalty is not valid!");
    }

    cache.i_miss_penalty = temp;
}

void InputUtilities::setWritePolicy(Cache &cache, ifstream &configFile) const
{
    string line;
    unsigned short temp;
    getline(configFile, line);

    temp = (unsigned short)(stoi(line));

    switch (temp)
    {
    case 0:
        cache.t_write = WRITE_THROUGH;
        break;
    case 1:
        cache.t_write = WRITE_BACK;
        break;
    default:
        throw("Write policy is not valid!");
    }
}

void InputUtilities::readConfig(int argc, char *argv[], Cache &cache)
{
    // read from command line
    int opt;

    while ((opt = getopt(argc, argv, "c:t:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            s_config_dir = optarg;
            break;
        case 't':
            s_trace_dir = optarg;
            break;
        case 'o':
            s_output_dir = optarg;
            break;
        default:
            cerr << "Invalid option!" << endl;
            exit(1);
        }
    }

    if (s_config_dir.empty() || s_trace_dir.empty())
    {
        cerr << "Missing arguments!" << endl;
        exit(1);
    }

    ifstream configFile(s_config_dir);
    if (!configFile.is_open())
    {
        cerr << "Cannot open config file!" << endl;
        exit(1);
    }
    try
    {
        setCacheLineSize(cache, configFile); // Byte
        setAssociativity(cache, configFile); // 0 - FULLY_ASSOCIATIVE, 1 - DIRECT_MAPPED, 2 - SET_ASSOCIATIVE
        setCacheSize(cache, configFile);     // KB

        if (cache.t_assoc == DIRECT_MAPPED) // If the associativity_way is direct_mapped,the replacement polacy can be none only;
        {
            cache.t_replace = none;
            setMissPenalty(cache, configFile);
            setWritePolicy(cache, configFile);
        }
        else if (cache.t_assoc == FULLY_ASSOCIATIVE || cache.t_assoc == SET_ASSOCIATIVE)
        {
            setReplacePolicy(cache, configFile);
            setMissPenalty(cache, configFile);
            setWritePolicy(cache, configFile);
        }
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    configFile.close();
    if (configFile.is_open())
    {
        cerr << "Cannot close config file!" << endl;
        exit(1);
    }
}

void InputUtilities::setCacheProperties(int argc, char *argv[], Cache &cache)
{
    readConfig(argc, argv, cache);

    // set cache line number
    assert(cache.i_cache_line_size != 0);
    cache.i_cache_line_num = (cache.i_cache_size << 10) / cache.i_cache_line_size;

    // set bit width for cache block size in adddress
    unsigned long temp = cache.i_cache_line_size;
    while (temp)
    {
        cache.bit_block_offset_width++; // cuz input line size dosen't include flags
        temp >>= 1;
    }
    cache.bit_block_offset_width--;

    if (cache.t_assoc == DIRECT_MAPPED)
    {
        // tag + line_offset + block_offset
        temp = cache.i_cache_line_num;
        while (temp)
        {
            cache.bit_cache_line_offset_width++;
            temp >>= 1;
        }
        cache.bit_cache_line_offset_width--;
        cache.bit_cache_set_offset_width = 0;
    }
    else if (cache.t_assoc == FULLY_ASSOCIATIVE)
    {
        // tag + block_offset
        cache.bit_cache_set_offset_width = 0;
        cache.bit_cache_line_offset_width = 0;
    }
    else if (cache.t_assoc == SET_ASSOCIATIVE)
    {
        // tag + set_offset + block_offset
        assert(cache.i_cache_set_line_num != 0);
        cache.i_cache_set_num = cache.i_cache_line_num / cache.i_cache_set_line_num;
        temp = cache.i_cache_set_num;
        while (temp)
        {
            cache.bit_cache_set_offset_width++;
            temp >>= 1;
        }
        cache.bit_cache_set_offset_width--;
        cache.bit_cache_line_offset_width = 0;
    }

    cache.bit_cache_tag_width = ADDRESS_WIDTH - cache.bit_cache_set_offset_width - cache.bit_cache_line_offset_width - cache.bit_block_offset_width;
}