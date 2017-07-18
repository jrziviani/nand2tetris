#ifndef _SYMBOLTLB_H
#define _SYMBOLTLB_H

#include <cstdint>
#include <unordered_map>
#include <string>
#include <limits>

class symbol_table
{
    using symbols = std::unordered_map<std::string, uint16_t>;
    symbols _table;
    uint16_t _spot;

    public:
    symbol_table() :
        _spot(16)
    {
        _table.insert(std::make_pair("SP"  , 0x0));
        _table.insert(std::make_pair("LCL" , 0x1));
        _table.insert(std::make_pair("ARG" , 0x2));
        _table.insert(std::make_pair("THIS", 0x3));
        _table.insert(std::make_pair("THAT", 0x4));
        _table.insert(std::make_pair("R0"  , 0x0));
        _table.insert(std::make_pair("R1"  , 0x1));
        _table.insert(std::make_pair("R2"  , 0x2));
        _table.insert(std::make_pair("R3"  , 0x3));
        _table.insert(std::make_pair("R4"  , 0x4));
        _table.insert(std::make_pair("R5"  , 0x5));
        _table.insert(std::make_pair("R6"  , 0x6));
        _table.insert(std::make_pair("R7"  , 0x7));
        _table.insert(std::make_pair("R8"  , 0x8));
        _table.insert(std::make_pair("R9"  , 0x9));
        _table.insert(std::make_pair("R10" , 0xa));
        _table.insert(std::make_pair("R11" , 0xb));
        _table.insert(std::make_pair("R12" , 0xc));
        _table.insert(std::make_pair("R13" , 0xd));
        _table.insert(std::make_pair("R14" , 0xe));
        _table.insert(std::make_pair("R15" , 0xf));
        _table.insert(std::make_pair("KBD" , 0x6000));
        _table.insert(std::make_pair("SCREEN" , 0x4000));
    }

    void add(const std::string &key, uint16_t value) noexcept
    {
        _table.insert(make_pair(key, value));
    }

    bool has_key(const std::string &key) const noexcept
    {
        return _table.find(key) != _table.end();
    }

    uint16_t get(const std::string &key) noexcept
    {
        if (has_key(key)) {
            return _table[key];
        }

        return std::numeric_limits<uint16_t>::max();
    }

    uint16_t available_spot() noexcept
    {
        return _spot++;
    }
};

#endif
