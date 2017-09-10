#ifdef _WIN32
#include "stdafx.h"
#endif

#include "BaseIO.h"

namespace RhythmGameChart
{
    namespace IO
    {
        BaseIO::BaseIO()
        {
            m_isLittleEndian = Helper::SystemIsLittleEndian();
        }


        BaseIO::~BaseIO()
        {
        }
    }
}
