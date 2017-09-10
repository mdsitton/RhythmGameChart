#ifdef _WIN32
#include "stdafx.h"
#endif

#include "IO Util.h"
#include <iostream>
#include <bitset>

using namespace std;

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
        void Helper::PrintBinary(std::vector<byte> bytes)
        {
            for (int i = 0; i < bytes.size(); ++i)
                cout << bitset<8>(bytes[i]);

            cout << endl;
        }
    }
}