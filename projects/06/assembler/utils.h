#ifndef _UTILS_H
#define _UTILS_H

#include <string>

std::string int_to_binary(uint16_t value)
{
    std::string ret;
    uint32_t tmp = value;
    for (uint16_t i = 0; i < 16; ++i, tmp = tmp << 1) {
        if (tmp & 0x8000) {
            ret += "1";
        }
        else {
            ret += "0";
        }
    }
    return ret;
}

#endif
