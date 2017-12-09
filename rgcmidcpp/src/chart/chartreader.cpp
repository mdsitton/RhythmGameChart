#include "chartreader.hpp"

#include <iostream>
#include <fstream>
#include <utility>
#include <cstdint>
#include <cstdlib>


template<typename T>
T string_to_uint(std::string_view str)
{
    static_assert(std::is_unsigned<T>(), "Must be unsigned type");

    T output = 0;

    for(char chr: str)
    {
        uint8_t d;
        if ((d = chr - '0') <=9 )
        {
            output = output * 10 + d;
        }
    }
    return output;
}

// Faster reimplementations of string functionality may eventually move into my own string wrapper or something.
size_t find_first(std::string_view str, char chr, size_t start = 0)
{
    for (size_t i = start; i < str.size(); ++i)
    {
        if (str[i] == chr)
        {
            return i;
        }
    }
    return std::string::npos;
}

size_t find_last(std::string_view str, char chr, size_t start = std::string::npos)
{
    if (start == std::string::npos)
    {
        start = str.size()-1;
    }
    for (size_t i = start; i >= 0; --i)
    {
        if (str[i] == chr)
        {
            return i;
        }
    }
    return std::string::npos;
}

size_t find_first_not_in(std::string_view str, std::string_view collection, size_t start = 0)
{
    for (size_t i = start; i < str.size(); ++i)
    {
        auto &chr = str[i];
        bool match = false;
        for (auto &item : collection)
        {
            if (chr == item)
            {
                match = true;
            }
        }
        if (match != true)
        {
            return i;
        }
    }
    return std::string::npos;
}

size_t find_last_not_in(std::string_view str, std::string_view collection, size_t start = std::string::npos)
{
    if (start == std::string::npos)
    {
        start = str.size()-1;
    }
    for (size_t i = start; i >= 0; --i)
    {
        auto &chr = str[i];
        bool match = false;
        for (auto &item : collection)
        {
            if (chr == item)
            {
                match = true;
            }
        }
        if (match != true)
        {
            return i;
        }
    }
    return std::string::npos;
}

std::vector<std::string_view> string_split(std::string_view str, char delimiter)
{
    std::vector<std::string_view> split;

    size_t start = 0;
    size_t end = 0;
    bool running = true;

    while (running)
    {
        end = find_first(str, delimiter, start);
        if (end == std::string::npos)
        {
            end = str.size();
            running = false;
        }
        split.push_back(str.substr(start, end-start));
        start = end + 1;
    }

    return split;
}

std::string_view strip(std::string_view str)
{
    size_t first = find_first_not_in(str, " \t");
    size_t last = find_last_not_in(str, " \t");

    // Check for blank lines.
    if (first == std::string::npos && last == std::string::npos)
    {
        return "";
    }
    else
    {
        return str.substr(first, last-first+1);
    }
}


ChartReader::ChartReader(std::string path)
:m_path(path), m_bufferCharPos(0), m_eob(false)
{
    // Open the file in binary mode 
    std::ifstream chartFile(m_path, std::ios::binary | std::ios::ate);
    if (chartFile)
    {
        auto end = chartFile.tellg();
        chartFile.seekg(0, std::ios::beg);
        m_buffer.resize(end - chartFile.tellg());
        chartFile.read(&m_buffer[0], m_buffer.size());
        chartFile.close();
    }
    else
    {
        throw std::runtime_error("Failed to load midi.");
    }
}

void ChartReader::read()
{
    bool activeSection = false;
    SectionData *section = nullptr;

    while (m_eob == false)
    {
        std::string_view line = strip(next_line(m_buffer));
        if (activeSection)
        {
            // Section Data
            if (line[0] == '{')
            {
                continue;
            }
            else if (line[0] == '}')
            {
                activeSection = false;
                continue;
            }
            size_t split = find_first(line, '=');

            section->data.emplace_back(
                strip(line.substr(0, split-1)), // Line left of =
                strip(line.substr(split + 1, line.size() - split))); // Line Right of =
        }
        else if (line[0] == '[' && line[(line.size())-1] == ']')
        {
            // Section Name
            activeSection = true;
            m_sections.emplace_back(line.substr(1, line.size()-2));
            section = &m_sections.back();
        }
    }

    for (auto &section : m_sections)
    {
        if (section.name == "Song")
        {
            for (auto &line : section.data)
            {
                std::string_view key = line.first;

                if (key == "Name")
                {
                    m_metadata.name = line.second;
                }
                else if (key == "Artist")
                {
                    m_metadata.artist = line.second;
                }
                else if (key == "Year")
                {
                    m_metadata.year = line.second;
                }
                else if (key == "Charter")
                {
                    m_metadata.charter = line.second;
                }
                else if (key == "Genre")
                {
                    m_metadata.genre = line.second;
                }
                else if (key == "Resolution")
                {
                    m_metadata.resolution = string_to_uint<uint32_t>(line.second);
                }
                else if (key == "Offset")
                {
                    m_metadata.offset = atof(line.second.data());
                }
                else
                {
                    // Store any unknown key/values
                    m_metadata.unused.emplace_back(line.first, line.second);
                }
            }
        }
        else
        {

            uint64_t tickTime = 0;
            for (auto &line : section.data)
            {
                tickTime = string_to_uint<uint32_t>(line.first);

                // Skip hand animation and anchors for now
                if (line.second[0] == 'H' || line.second[0] == 'A')
                {
                    continue;
                }

                if (line.second[0] == 'E')
                {
                    auto posFirst = find_first(line.second, '\"')+1;
                    auto posLast = find_last(line.second, '\"');

                    if (posFirst != std::string::npos && posLast != std::string::npos)
                    {
                        auto data = line.second.substr(posFirst, posLast - posFirst);
                        // std::cout << data << std::endl;
                    }
                }

                auto parts = string_split(line.second, ' ');



            }
        }
    }
}

std::string_view ChartReader::next_line(std::string_view str)
{
    size_t start = m_bufferCharPos;
    size_t end = start;
    size_t newBufPos = start;

    std::string_view view = str.substr(start, str.size() - start);
    for (size_t i = 0; i < view.size(); ++i)
    {
        // Unix line endings
        if (view[i] == '\n')
        {
            newBufPos += i + 1;
            end = i;
            break;
        }

        // Windows Line endings.
        if (view[i] == '\r' && view[i+1] == '\n')
        {
            newBufPos += i + 2;
            end = i;
            break;
        }
    }

    if (newBufPos == m_buffer.size())
    {
        m_eob = true;
    }

    m_bufferCharPos = newBufPos;

    return view.substr(0, end);
}