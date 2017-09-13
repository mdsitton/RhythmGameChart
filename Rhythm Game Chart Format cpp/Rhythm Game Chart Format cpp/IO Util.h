#pragma once

#define DEBUG true

#include <string>
// Copyright (c) 2017 Alexander Ong
// See LICENSE in the RhythmGameChart root for license information.

#include <vector>
namespace RhythmGameChart
{
    namespace IO
    {
        typedef unsigned long VLV;
        typedef unsigned char byte;
        typedef unsigned short u8;

        // Header constants
        const u8 VERSION = 0x01;
        const std::string FILE_ID = "RhythmGameChart";

        const std::string INSTRUMENT_ID = "RGCI";
        const std::string TRACK_ID = "RGCTrk";

        // Event constants
        namespace Event
        {
            const u8 Note = 0xFF;       // Feel free to change these values these later, idgaf
            const u8 Text = 0xF1;
        }

        namespace Helper
        {
            bool SystemIsLittleEndian();
            void ReverseBits(byte& data);
            void PrintBinary(std::vector<byte> bytes);
        }
    }
}