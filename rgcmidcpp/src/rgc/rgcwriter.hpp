// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the RhythmGameChart root for license information.

#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "rgctypes.hpp"

namespace RGCCPP::RGC
{

    class RgcWriter
    {
    public:
        RgcWriter(std::string filePath, RgcFileData* fileData);
        ~RgcWriter();
    private:
        std::string m_filePath;
        std::ofstream m_rgcFile;
        RgcFileData* m_fileData;
        std::stringstream m_headerStream;
        std::stringstream m_offsetTableStream;
        std::stringstream m_trackStream;


        void write_file();

    };
}