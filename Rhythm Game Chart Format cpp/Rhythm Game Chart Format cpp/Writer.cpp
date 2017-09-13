// Copyright (c) 2017 Alexander Ong
// See LICENSE in the RhythmGameChart root for license information.

#include "stdafx.h"

#include "Writer.h"
#include <iostream>

using namespace std;

namespace RhythmGameChart
{
    namespace IO
    {
        Writer::Writer() : BaseIO()
        {
            m_encodedVLV.reserve(sizeof(VLV));
        }

        void Writer::VLVEncode(VLV number)
        {
            m_encodedVLV.clear();
      
            bool first = true;
            while (first || number > 0)
            {
                u8 lower7bits;

                lower7bits = static_cast<u8>(number & 0x7F);

                if (!first)
                    lower7bits |= 128;      // Change lsb to a value of 1
                number >>= 7;

                first = false;

                byte data = static_cast<byte>(lower7bits);
                ToLittleEndian(data);

                m_encodedVLV.insert(m_encodedVLV.begin(), data);
            }

#if DEBUG
            std::cout << "Binary of " << number << " is ";
            Helper::PrintBinary(m_encodedVLV);
#endif
        }

        void Writer::JoinTick(VLV tick, vector<byte>& eventData)
        {
            VLVEncode(tick);
            eventData.insert(eventData.begin(), m_encodedVLV.begin(), m_encodedVLV.end());
        }
    }
}
