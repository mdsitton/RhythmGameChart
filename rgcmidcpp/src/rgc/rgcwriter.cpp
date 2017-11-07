
#include "rgcwriter.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

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

        // There is an offset table that comes next for the header but we cannot continue until
        // we know the track offsets.

        std::vector<uint32_t> trackOffsets;

        // Now we start constructing the body of the file the metadata section is first,
        // however we have not yet finished fleshing out the binary format of this so for now
        // it will just be a basic stub.
        trackOffsets.push_back(m_trackStream.tellp());

        write_type<char, true>(m_trackStream,
            metadata_identifier.c_str(),
            metadata_identifier.length());

        write_type<uint32_t, true>(m_trackStream, 0); // this is a stub for now.


        trackOffsets.push_back(m_trackStream.tellp());

        write_type<char, true>(m_trackStream,
            global_event_track_identifier.c_str(),
            global_event_track_identifier.length());

        write_type<uint32_t, true>(m_trackStream, 0); // this is a stub for now.

        // write section offsets.
        for (auto &offset : trackOffsets)
        {
            write_vlv<uint64_t>(m_offsetTableStream, offset);
        }
        std::cout << m_offsetTableStream.str().length() << std::endl;

        // Create the rgc bom "RGCf" for the file.
        uint32_t rgcBom = str_to_bin<uint32_t, true>(header_file_identifier);

        write_type<uint32_t, true>(m_headerStream, rgcBom);
        write_type<uint8_t, true>(m_headerStream, m_fileData->version);
        write_type<uint16_t, true>(m_headerStream, m_fileData->division);
        write_type<uint8_t, true>(m_headerStream, trackOffsets.size());
        std::cout << m_headerStream.str().length() << std::endl;


        // write out the header and body.
        m_rgcFile << m_headerStream.rdbuf();
        m_rgcFile << m_offsetTableStream.rdbuf();
        m_rgcFile << m_trackStream.rdbuf();
    }
}