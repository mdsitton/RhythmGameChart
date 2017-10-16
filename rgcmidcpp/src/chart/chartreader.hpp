#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <utility>


struct SectionData
{
	using LineData = std::pair<std::string_view, std::string_view>;
	std::string name;
	std::vector<LineData> data;

	SectionData(std::string_view str)
    :name(str)
	{
	}
};

struct SongSection
{
	std::string name;
	std::string artist;
	std::string album;
	std::string year;
	std::string charter;
	std::string genre;
	int resolution;
	float offset;
	using LineData = std::pair<std::string, std::string>;
	std::vector<LineData> unused;
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
    SongSection m_metadata;

};