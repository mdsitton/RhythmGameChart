#ifdef _WIN32
#include "stdafx.h"
#endif

#include "Reader.h"

#include <fstream>

namespace RhythmGameChart
{
    namespace IO
    {
        VLV Reader::VLVDecode(std::ifstream& ifs)
        {
            return VLV();
        }
    }
}
