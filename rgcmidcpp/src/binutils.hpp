// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the rhcmidcpp root for license information.

#pragma once
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include <stdexcept>

// This is an older version of the code used in openrhythm with backported
// bugfixes because it's a bit simpler to extend/understand. Way slower than
// my code in openrhythm.

// Read and write to VariableLengthValues
std::vector<uint8_t> to_vlv(uint32_t value);
uint32_t from_vlv(std::vector<uint8_t>& varLen);
uint32_t read_vlv(std::istream &stream);
void write_vlv(std::ostream &stream, uint32_t value);

// Templates to read from a file different length values
template<typename T>
T read_type(std::istream &file)
{
    T output;
    auto size = sizeof(T);
    int offset;
    for (size_t i = 0; i < size; i++) {
        offset = static_cast<int>((size-1) - i); // endianness lol ?
        file.read(reinterpret_cast<char*>(&output)+offset, 1);
    }
    return output;
}

template<typename T>
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
            offset = static_cast<int>((size-1) - i); // endianness lol ?
            file.read(reinterpret_cast<char*>(&output)+offset, 1);
        }
    }
    return output;
}

template<typename T>
void read_type(std::istream &file, T *output, size_t length)
{
    auto size = sizeof(T);
    int offset;
    for (size_t j = 0; j < length; j++)
    {
        for (size_t i = 0; i < size; i++)
        {
            offset = static_cast<int>(((size-1) - i) + (size * j)); // endianness lol ?
            file.read(reinterpret_cast<char*>(output) + offset, 1);
        }
    }
}

// We implement peek with read_type above to reduce code duplication.
template<typename T>
T peek_type(std::istream &file)
{
    // We need to emulate a peek because we need several bytes of data.
    std::streampos startingPos = file.tellg();
    T output = read_type<T>(file);
    file.seekg(startingPos);
    return output;

}

template<typename T>
T peek_type(std::istream &file, size_t size)
{
    // We need to emulate a peek because we need several bytes of data.
    std::streampos startingPos = file.tellg();
    T output = read_type<T>(file, size);
    file.seekg(startingPos);
    return output;
}

template<typename T>
void peek_type(std::istream &file, T *output, size_t length)
{
    // We need to emulate a peek because we need several bytes of data.
    std::streampos startingPos = file.tellg();
    read_type<T>(file, output, length);
    file.seekg(startingPos);
}


// Templates to write to different lengths values to files.

// write `source` to `file` in big endian
template<typename T>
void write_type(std::ostream &file, T source)
{
    auto size = sizeof(T);
    int offset;

    char* sourcePtr = reinterpret_cast<char*>(&source);

    for (size_t i = 0; i < size; i++)
    {
        offset = static_cast<int>((size-1) - i); // endianness lol ?
        file << *(sourcePtr+offset);
    }
}

// write only `size` bytes from `source` to `file` in big endian
template<typename T>
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
            offset = static_cast<int>((size-1) - i); // endianness lol ?
            file << *(sourcePtr+offset);
        }

    }
}

// Write `length` number of `T` from `source` in big endian
// Primarally used for writing strings.
template<typename T>
void write_type(std::ostream &file, T *source, size_t length)
{
    auto size = sizeof(T);
    int offset;
    char* sourcePtr = reinterpret_cast<char*>(source);

    for (size_t j = 0; j < length; j++)
    {
        for (size_t i = 0; i < size; i++)
        {
            offset = static_cast<int>(((size-1) - i) + (size * j)); // endianness lol ?
            file << *(sourcePtr + offset);
        }
    }
}
