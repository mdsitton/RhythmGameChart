// Copyright (c) 2017 Alexander Ong
// See LICENSE in the RhythmGameChart root for license information.

#pragma once
#include "BaseIO.h"

#include <vector>
namespace RhythmGameChart
{
    namespace IO
    {
        class Writer : BaseIO
        {
        public:
            Writer();
        protected:
            // Converts a number into variable length value byte data and stores it in the m_encodedVLV vector
            void VLVEncode(VLV number);

            // Adds tick data in front of the provided byte data
            void JoinTick(VLV tick, std::vector<byte>& eventData);

        private:
            std::vector<byte> m_encodedVLV;
            bool m_isLittleEndian;
        };
    }
}

