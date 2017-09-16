
#include "rgcwriter.hpp"

#include <fstream>
#include <stdexcept>

#include "binutils.hpp"


namespace RGCCPP::RGC
{

	RgcWriter::RgcWriter(std::string filePath)
	:m_filePath(filePath), m_rgcFile(filePath, std::ios_base::binary | std::ios_base::out)
	{
        if (m_rgcFile)
        {
            write_file();
            m_rgcFile.close();
        }
        else
        {
            throw std::runtime_error("Failed to load midi.");
        }

	}
	void RgcWriter::write_file()
	{

	}
}