// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the rhcmidcpp root for license information.

#include <iostream>
#include <fstream>
#include <cstdint>

#include "midireader/midi_chart.hpp"
#include "binutils.hpp"

int main()
{
    
    RGCCPP::Midi::Song song("songs");

    song.load();

    std::cout << "Song division: " << song.get_divison() << " ticks" << std::endl;
    std::cout << "Track count: " << song.get_tracks()->size() << std::endl;
    

    std::ofstream outFile("bintest.bin", std::ios_base::binary | std::ios_base::out);
    
    write_type<uint32_t>(outFile, 128);

    outFile.close();

    std::ifstream inFile("bintest.bin", std::ios_base::binary);

    auto data = read_type<uint32_t>(inFile);

    if (data == 128)
    {
        std::cout << "SUCCESS!" << std::endl;
    }
    else
    {
        std::cout << "FAILED!" << std::endl;
    }
    
    inFile.close();

    return 0;
}
