#include "stdafx.h"
#include "IO Util.h"

namespace RhythmGameChart
{
    namespace IO
    {
        bool Helper::SystemIsLittleEndian()
        {
            int n = 1;
            return (*(char *)&n == 1);
        }

        void Helper::ReverseBits(byte& data)
        {
            // TODO
        }
    }
}