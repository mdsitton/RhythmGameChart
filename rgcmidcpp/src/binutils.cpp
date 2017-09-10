// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the rhcmidcpp root for license information.

#include "binutils.hpp"

#include <algorithm>

// This is an earlier varient of my varlen reading code I wrote for openrhythm w/ bugfixes.
// It is slower but supports reading and writing, and i'm lazy so here we go.

uint32_t from_vlv(std::vector<uint8_t>& varLen)
{
    uint32_t value = 0;

    if (varLen.size() > 4)
    {
        throw std::runtime_error("Variable Length Value to long!");
        return 0;
    }

    // get the last element of the vector and verify that it doesnt
    // have the contiuation bit set.
    if (((* --varLen.end()) & 0x80) != 0)
    {
        throw std::runtime_error("Invalid Variable Length Value!");
    }

    for(auto &byte : varLen)
    {
        value = (value << 7) | (byte & 0x7F);
    }

    return value;
}

std::vector<uint8_t>&& to_vlv(uint32_t value)
{
    uint8_t scratch;
    int byteCount = 0;

    std::vector<uint8_t> output;

    do {
        scratch = value&0x7F;
        if (byteCount != 0 ) {
            scratch |= 0x80;
        }
        output.push_back(scratch);
        value >>= 7;
        byteCount++;

    } while(value != 0 && byteCount < 4);

    std::reverse(output.begin(), output.end());

    return std::move(output);
}

uint32_t read_vlv(std::istream &stream)
{
    std::vector<uint8_t> varLen;
    uint8_t c;
    do {
        c = read_type<uint8_t>(stream);
        varLen.push_back(c);
    } while(c & 0x80 && varLen.size() < 4);

    return from_vlv(varLen);
}

void write_vlv(std::ostream &stream, uint32_t value)
{
    std::vector<uint8_t> varLen = to_vlv(value);

    for(auto &byte : varLen)
    {
        stream << byte;
    }
}