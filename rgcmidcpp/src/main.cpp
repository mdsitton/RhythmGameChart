#include "midireader/midi_chart.hpp"

#include <iostream>


int main()
{
    
    RGCCPP::Midi::Song song("songs");

    song.load();

    std::cout << "Song division: " << song.get_divison() << " ticks" << std::endl;
    std::cout << "Track count: " << song.get_tracks()->size() << std::endl;
    
    return 0;
}