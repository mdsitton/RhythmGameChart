// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the RhythmGameChart root for license information.

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <memory>

#include "midi/midi_chart.hpp"
#include "binutils.hpp"


template <bool swapEndian>
void test()
{
    // Test 32-bit unsigned int round trip.
    {
        std::ofstream outFile("bintest.bin", std::ios_base::binary | std::ios_base::out);

        uint32_t ref_value = 128;
        
        write_type<uint32_t, swapEndian>(outFile, ref_value);

        outFile.close();

        std::ifstream inFile("bintest.bin", std::ios_base::binary);

        auto data = read_type<uint32_t, swapEndian>(inFile);

        if (data == ref_value)
        {
            std::cout << "SUCCESS!" << std::endl;
        }
        else
        {
            std::cout << "FAILED!" << std::endl;
        }
        
        inFile.close();
    }

    // Test string round trip.
    {
        std::ofstream outFile("bintest_str.bin", std::ios_base::binary | std::ios_base::out);


        std::string ref_val = "We are number one!";
        uint32_t ref_var_len = 12345678;

        // write string
        write_string(outFile, ref_val);

        // Test a longer var len which will use all 4 bytes.
        write_vlv(outFile, ref_var_len);

        outFile.close();

        std::ifstream inFile("bintest_str.bin", std::ios_base::binary);

        // read string
        std::string inValue = read_string(inFile);

        // Test a longer var len which will use all 4 bytes.
        size_t inLongVarLen = read_vlv(inFile);

        if (inValue == ref_val && inLongVarLen == ref_var_len)
        {
            std::cout << "SUCCESS!" << std::endl;
        }
        else
        {
            std::cout << "FAILED!" << std::endl;
        }
        
        inFile.close();
    }
}

int main()
{
    
    RGCCPP::Midi::Song song("songs");

    song.load();

    std::cout << "Song division: " << song.get_divison() << " ticks" << std::endl;
    std::cout << "Track count: " << song.get_tracks()->size() << std::endl;

    test<false>();
    test<true>();

    return 0;
}
