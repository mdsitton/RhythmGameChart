#pragma once
#include "IO Util.h"

namespace RhythmGameChart
{
    namespace IO
    {
        class BaseIO
        {
        public:
            BaseIO();
            ~BaseIO();
        protected:
            inline void ToLittleEndian(byte &value)
            {
                if (!m_isLittleEndian)
                    Helper::ReverseBits(value);
            }
        private:
            bool m_isLittleEndian;
        };
    }
}

