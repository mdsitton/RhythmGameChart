#include "chartreader.hpp"

#include <iostream>
#include <fstream>
#include <utility>

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
    while (m_eob == false)
    {
        std::string_view line = next_line(m_buffer);
        size_t lineLength = line.size();
        if (line[0] == '[' && line[lineLength-1] == ']')
        {
            std::cout << line.substr(1, lineLength-2) << std::endl;
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