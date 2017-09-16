// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the RhythmGameChart root for license information.

#pragma once

#include <cstdint>

namespace RGCCPP::RGC
{
    // This is basically the data structures which the rgc reader/writer use in order to pass
    // data back and forth so a file can be read then written.
    struct RgcFileData
    {
        uint8_t version;
        uint8_t tracks;
        uint16_t division;

    };
}