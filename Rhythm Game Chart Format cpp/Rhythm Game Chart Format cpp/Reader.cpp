#include "stdafx.h"
#include "Reader.h"

#include <fstream>

namespace RhythmGameChart
{
    namespace IO
    {
        Reader::Reader()
        {
        }


        Reader::~Reader()
        {
        }

        VLV Reader::VLVDecode(std::ifstream& ifs)
        {
            return VLV();
        }
    }
}
