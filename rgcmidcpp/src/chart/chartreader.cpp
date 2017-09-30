#include "chartreader.hpp"

#include <iostream>
#include <fstream>
#include <utility>


std::string_view strip(std::string_view str)
{
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last-first+1);
}

ChartReader::ChartReader(std::string path)
:m_path(path), m_bufferCharPos(0), m_eob(false)
{
    // Open the file in binary mode 
    std::ifstream chartFile(m_path, std::ios::binary | std::ios::ate);
    if (chartFile)
    {
        auto end = chartFile.tellg();
        std::cout << end << std::endl;
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
        // TODO - strip each line here.
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
            section->data.emplace_back(line);
        }
        else if (line[0] == '[' && line[(line.size())-1] == ']')
        {
            activeSection = true;
            m_sections.emplace_back(line.substr(1, line.size()-2));
            section = &m_sections.back();

        }
    }
    for (auto &section : m_sections)
    {
        std::cout << section.name << std::endl;

        for (auto &line : section.data)
        {
            std::cout << line << "\n";
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

    // resize view to only cover a single line.
    view.remove_suffix(view.size() - end);

    if (newBufPos == m_buffer.size())
    {
        m_eob = true;
    }

    m_bufferCharPos = newBufPos;

    return std::move(view);
}