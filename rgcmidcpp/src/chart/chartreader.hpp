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

struct SectionData
{
	std::string name;
	std::vector<std::string> data;
	SectionData(std::string_view str)
	{
		// copy string
		name = str;
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