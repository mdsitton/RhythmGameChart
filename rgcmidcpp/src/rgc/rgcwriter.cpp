
#include "rgcwriter.hpp"

#include <fstream>
#include <stdexcept>

#include "binutils.hpp"


namespace RGCCPP::RGC
{

    RgcWriter::RgcWriter(std::string filePath, RgcFileData* fileData)
    :m_filePath(filePath), m_fileData(fileData),
    m_rgcFile(filePath, std::ios_base::binary | std::ios_base::out)
    {
        if (m_rgcFile)
        {
            write_file();
        }
        else
        {
            throw std::runtime_error("Failed to write rgc file.");
        }
    }

    RgcWriter::~RgcWriter()
    {
        if (m_rgcFile)
        {
            m_rgcFile.close();
        }
    }

    void RgcWriter::write_file()
    {

    }
}