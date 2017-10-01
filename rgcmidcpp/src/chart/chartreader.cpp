#include "chartreader.hpp"

#include <iostream>
#include <fstream>
#include <utility>

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
        start = str.size();
    }
    for (size_t i = start; i >= 0; --i)
    {
        auto &chr = str[i];
        bool match = false;
        for (auto &item : collection)
        {
            if (chr == item || chr == '\0')
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

std::string_view strip(std::string_view str)
{
    size_t first = find_first_not_in(str, " \t");
    size_t last = find_last_not_in(str, " \t");
    return str.substr(first, last-first);
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

            section->data.emplace_back(strip(line.substr(0, split-1)), strip(line.substr(split + 1, line.size() - split)));
        }
        else if (line[0] == '[' && line[(line.size())-1] == ']')
        {
            activeSection = true;
            m_sections.emplace_back(line.substr(1, line.size()-2));
            section = &m_sections.back();
        }
    }

    // for (auto &section : m_sections)
    // {
    //     std::cout << section.name << std::endl;
    //     for (auto &line : section.data)
    //     {
    //         std::cout << line.dataLeft << " | " << line.dataRight << "\n";
    //     }
    // }

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

    // resize view to only cover a single line.
    view.remove_suffix(view.size() - end);

    if (newBufPos == m_buffer.size())
    {
        m_eob = true;
    }

    m_bufferCharPos = newBufPos;

    return std::move(view);
}