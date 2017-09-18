// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the RhythmGameChart root for license information.

#pragma once

#include <cstdint>

namespace RGCCPP::RGC
{
	// these are all 4 character identifiers for various sections of the fileformat.
	// I'm sticking to 4 character to keep the format better aligned.
	const std::string header_file_identifier = "RGCf";
	const std::string metadata_identifier = "CMet";
	const std::string global_event_track_identifier = "CGlo";
	const std::string track_identifier = "CTrk";

    // This is basically the data structures which the rgc reader/writer use in order to pass
    // data back and forth so a file can be read then written.
    struct RgcFileData
    {
        uint8_t version;
        uint8_t tracks;
        uint16_t division;
    };
}