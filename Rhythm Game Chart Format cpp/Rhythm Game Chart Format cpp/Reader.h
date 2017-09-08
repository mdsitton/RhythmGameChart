#pragma once
#include "BaseIO.h"

#include <iostream>

namespace RhythmGameChart
{
    namespace IO
    {
        class Reader : BaseIO
        {
        protected:
            VLV VLVDecode(std::ifstream& ifs);
        private:
            bool m_isLittleEndian;
        };
    }
}

