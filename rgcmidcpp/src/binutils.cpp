// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the RhythmGameChart root for license information.

#include "binutils.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>

std::string read_string(std::istream &stream)
{
    auto length = read_vlv<uint32_t>(stream);

    // allocate storage for string
    auto tempCharArr = std::make_unique<char[]>(length + 1);
    tempCharArr[length] = '\0';

    read_type<char, false>(stream, tempCharArr.get(), length);

    return {tempCharArr.get()};
}

void write_string(std::ostream &stream, std::string_view str)
{

    write_vlv<uint32_t>(stream, str.length());
    write_type<char, false>(stream, str.data(), str.length());
}