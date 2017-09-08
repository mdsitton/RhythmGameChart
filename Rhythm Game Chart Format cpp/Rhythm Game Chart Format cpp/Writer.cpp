#include "stdafx.h"
#include "Writer.h"

using namespace std;

namespace RhythmGameChart
{
    namespace IO
    {
        Writer::Writer()
        {
            m_encodedVLV.capacity = sizeof(VLV);
        }

        Writer::~Writer()
        {
        }

        void Writer::VLVEncode(VLV number)
        {
            m_encodedVLV.clear();

            bool first = true;
            while (first || number > 0)
            {
                byte lower7bits;

                lower7bits = (byte)(number & 0x7F);

                if (!first)
                    lower7bits |= 128;      // Change lsb to a value of 1
                number >>= 7;

                first = false;
                m_encodedVLV.push_back(lower7bits);
            }
        }

        void Writer::JoinTick(VLV tick, vector<byte>& eventData)
        {
        }
    }
}
