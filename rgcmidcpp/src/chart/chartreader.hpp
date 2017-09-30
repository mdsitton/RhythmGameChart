#pragma once

#include <string>
#include <string_view>

// class StringBuffer
// {
// public:
//  StringBuffer(std::string &str, );

//  operator=
// }

class ChartReader
{
public:
    ChartReader(std::string path);
    void read();
private:
    std::string_view next_line(std::string_view str);
    std::string m_path;
    std::string m_buffer;
    size_t m_bufferCharPos;
    bool m_eob;
};