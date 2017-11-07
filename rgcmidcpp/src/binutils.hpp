// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the RhythmGameChart root for license information.

#pragma once
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <string_view>

// This is an older version of the code used in openrhythm with backported
// bugfixes because it's a bit simpler to extend/understand. Way slower than
// my code in openrhythm.

// Read and write strings
// The data format here is <vlv length> <string data>
std::string read_string(std::istream &stream);
void write_string(std::ostream &stream, std::string_view str);

// Templates to read from a file different length values
template<typename T, bool swapEndian = true>
T read_type(std::istream &file)
{
    T output;
    auto size = sizeof(T);
    int offset;
    for (size_t i = 0; i < size; i++) {
        if (swapEndian)
        {
            offset = static_cast<int>((size-1) - i); // endianness lol ?
        }
        else
        {
            offset = i;
        }
        file.read(reinterpret_cast<char*>(&output)+offset, 1);
    }
    return output;
}

template<typename T, bool swapEndian = true>
T read_type(std::istream &file, size_t size)
{
    T output = 0;
    if (sizeof(output) < size)
    {
        throw std::runtime_error("Size greater than container type");
    }
    else
    {
        int offset;
        for (size_t i = 0; i < size; i++)
        {
            if (swapEndian)
            {
                offset = static_cast<int>((size-1) - i); // endianness lol ?
            }
            else
            {
                offset = i;
            }
            file.read(reinterpret_cast<char*>(&output)+offset, 1);
        }
    }
    return output;
}

template<typename T, bool swapEndian = true>
void read_type(std::istream &file, T *output, size_t length)
{
    auto size = sizeof(T);
    int offset;
    for (size_t j = 0; j < length; j++)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (swapEndian)
            {
                offset = static_cast<int>(((size-1) - i) + (size * j)); // endianness lol ?
            }
            else
            {
                offset = i + (size * j);
            }
            file.read(reinterpret_cast<char*>(output) + offset, 1);
        }
    }
}

// We implement peek with read_type above to reduce code duplication.
template<typename T, bool swapEndian = true>
T peek_type(std::istream &file)
{
    // We need to emulate a peek because we need several bytes of data.
    std::streampos startingPos = file.tellg();
    T output = read_type<T, swapEndian>(file);
    file.seekg(startingPos);
    return output;

}

template<typename T, bool swapEndian = true>
T peek_type(std::istream &file, size_t size)
{
    // We need to emulate a peek because we need several bytes of data.
    std::streampos startingPos = file.tellg();
    T output = read_type<T, swapEndian>(file, size);
    file.seekg(startingPos);
    return output;
}

template<typename T, bool swapEndian = true>
void peek_type(std::istream &file, T *output, size_t length)
{
    // We need to emulate a peek because we need several bytes of data.
    std::streampos startingPos = file.tellg();
    read_type<T, swapEndian>(file, output, length);
    file.seekg(startingPos);
}


// Templates to write to different lengths values to files.

// write `source` to `file` in big endian
template<typename T, bool swapEndian = true>
void write_type(std::ostream &file, T source)
{
    auto size = sizeof(T);
    int offset;

    char* sourcePtr = reinterpret_cast<char*>(&source);

    for (size_t i = 0; i < size; i++)
    {
        if (swapEndian)
        {
            offset = static_cast<int>((size-1) - i); // endianness lol ?
        }
        else
        {
            offset = i;
        }
        file << *(sourcePtr+offset);
    }
}

// write only `size` bytes from `source` to `file` in big endian
template<typename T, bool swapEndian = true>
void write_type(std::ostream &file, T source, size_t size)
{
    if (size > sizeof(T))
    {
        throw std::runtime_error("Size greater than container type");
    }
    else
    {
        int offset;
        char* sourcePtr = reinterpret_cast<char*>(&source);

        for (size_t i = 0; i < size; i++)
        {
            if (swapEndian)
            {
                offset = static_cast<int>((size-1) - i); // endianness lol ?
            }
            else
            {
                offset = i;
            }
            file << *(sourcePtr+offset);
        }

    }
}

// Write `length` number of `T` from `source` in big endian
// Primarally used for writing strings.
template<typename T, bool swapEndian = true>
void write_type(std::ostream &file, const T *source, size_t length)
{
    auto size = sizeof(T);
    int offset;
    char* sourcePtr = const_cast<char*>(source);

    for (size_t j = 0; j < length; j++)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (swapEndian)
            {
                offset = static_cast<int>(((size-1) - i) + (size * j)); // endianness lol ?
            }
            else
            {
                offset = i + (size * j);
            }
            file << *(sourcePtr + offset);
        }
    }
}

// Convert string to bytes for creation of things like BOM's
template<typename T, bool swapEndian = false>
T str_to_bin(std::string_view str)
{
    T output = 0;
    int offset = 0;
    size_t size = sizeof(T);

    if (size < str.length())
    {
        throw std::runtime_error("Size greater than container type");
    }

    for (size_t i = 0; i < str.length(); i++)
    {
        if (swapEndian)
        {
            offset = static_cast<int>((size-1) - i); // endianness lol ?
        }
        else
        {
            offset = i;
        }
        output |= str[offset] << ((size-1-i)*8);
    }
    return output;
}

// Read and write to VariableLengthValues

// This is an earlier varient of my varlen reading code I wrote for openrhythm w/ bugfixes.
// It is slower but supports reading and writing, and i'm lazy so here we go.

template<typename T>
T from_vlv(std::vector<uint8_t>& varLen)
{
    T value = 0;

    // Check that the size of the varlen fits into the max number of bytes we have available
    if (varLen.size() > ((sizeof(T)*8)/7))
    {
        throw std::runtime_error("Variable Length Value to long for type!");
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

template<typename T>
std::vector<uint8_t> to_vlv(T value)
{
    uint8_t scratch;
    int byteCount = 0;

    std::vector<uint8_t> output;

    do {
        scratch = value & 0x7F;
        if (byteCount != 0 ) {
            scratch |= 0x80;
        }
        output.push_back(scratch);
        value >>= 7;
        byteCount++;

    } while(value != 0 && byteCount < ((sizeof(T)*8)/7));

    std::reverse(output.begin(), output.end());

    return output;
}

template<typename T>
T read_vlv(std::istream &stream)
{
    std::vector<uint8_t> varLen;
    uint8_t c;
    do {
        c = read_type<uint8_t>(stream);
        varLen.push_back(c);
    } while(c & 0x80 && varLen.size() < ((sizeof(T)*8)/7));

    return from_vlv<T>(varLen);
}

template<typename T>
void write_vlv(std::ostream &stream, T value)
{
    std::vector<uint8_t> varLen = to_vlv<T>(value);

    for(auto &byte : varLen)
    {
        stream << byte;
    }
}
