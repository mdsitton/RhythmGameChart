// Copyright (c) 2017 Alexander Ong
// See LICENSE in the RhythmGameChart root for license information.

#include "stdafx.h"

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
