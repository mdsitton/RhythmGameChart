#include "stdafx.h"
#include "Writer.h"

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

                m_encodedVLV.push_back(data);
            }
        }

        void Writer::JoinTick(VLV tick, vector<byte>& eventData)
        {
            VLVEncode(tick);
            eventData.insert(eventData.begin(), m_encodedVLV.begin(), m_encodedVLV.end());
        }
    }
}
