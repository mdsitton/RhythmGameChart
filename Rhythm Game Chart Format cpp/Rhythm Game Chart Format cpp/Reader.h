#pragma once

#include "IO Util.h"
#include <iostream>

namespace RhythmGameChart
{
    namespace IO
    {
        class Reader
        {
        public:
            Reader();
            ~Reader();
        protected:
            VLV VLVDecode(std::ifstream& ifs);
        };
    }
}

