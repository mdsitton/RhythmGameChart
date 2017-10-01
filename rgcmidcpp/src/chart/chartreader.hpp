#pragma once

#include <vector>
#include <string>
#include <string_view>

// class StringBuffer
// {
// public:
//  StringBuffer(std::string &str, );

//  operator=
// }

struct LineData
{
    std::string_view dataLeft;
    std::string_view dataRight;
    int a;

    LineData(std::string_view lData, std::string_view rData)
    :dataLeft(lData), dataRight(rData)
    {
    }
};

struct SectionData
{
	std::string name;

	std::vector<LineData> data;

	SectionData(std::string_view str)
    :name(str)
	{
	}
};

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

    std::vector<SectionData> m_sections;
};