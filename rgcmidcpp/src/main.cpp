// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the rhcmidcpp root for license information.

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <memory>

#include "midireader/midi_chart.hpp"
#include "binutils.hpp"

int main()
{
    
    RGCCPP::Midi::Song song("songs");

    song.load();

    std::cout << "Song division: " << song.get_divison() << " ticks" << std::endl;
    std::cout << "Track count: " << song.get_tracks()->size() << std::endl;
    
    // Test 32-bit unsigned int round trip.
    {
        std::ofstream outFile("bintest.bin", std::ios_base::binary | std::ios_base::out);

        uint32_t ref_value = 128;
        
        write_type<uint32_t>(outFile, ref_value);

        outFile.close();

        std::ifstream inFile("bintest.bin", std::ios_base::binary);

        auto data = read_type<uint32_t>(inFile);

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


        char ref_value[] = "We are number one!";
        uint32_t ref_var_len = 12345678;

        size_t len = strlen(ref_value);

        // write string length
        write_vlv(outFile, len);

        // write string contents.
        write_type<char>(outFile, ref_value, len);
        
        // Test a longer var len which will use all 4 bytes.
        write_vlv(outFile, ref_var_len);

        outFile.close();

        std::ifstream inFile("bintest_str.bin", std::ios_base::binary);


        // read string length
        size_t inLen = read_vlv(inFile);

        // allocate storage for string
        auto inValue = std::make_unique<char[]>(inLen + 1);
        inValue[inLen] = '\0';
        read_type<char>(inFile, inValue.get(), inLen);

        // Test a longer var len which will use all 4 bytes.
        size_t inLongVarLen = read_vlv(inFile);

        if (strcmp(ref_value, inValue.get()) == 0 && inLen == len && inLongVarLen == ref_var_len)
        {
            std::cout << "SUCCESS!" << std::endl;
        }
        else
        {
            std::cout << "FAILED!" << std::endl;
        }
        
        inFile.close();
    }


    return 0;
}
