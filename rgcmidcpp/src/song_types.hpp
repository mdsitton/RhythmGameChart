// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the rhcmidcpp root for license information.

#include <cstdint>

namespace RGCCPP
{
    enum class Difficulty
    {
        NONE,
        Easy,
        Medium,
        Hard,
        Expert
    };

    enum class TrackType
    {
        NONE,
        Guitar,
        Bass,
        Drums,
        Keys,
        Vocals,
    };

    enum class NoteType
    {
        NONE,
        Green,
        Red,
        Yellow,
        Blue,
        Orange,
        Solo,
        Drive,
        Freestyle
    };

    enum class EventType
    {
        NONE,
        solo,
        drive,
        freestyle,
    };

    struct TrackNote
    {
        NoteType type;
        double time;
        int32_t pulseTimeStart;
        int32_t pulseTimeEnd;
        double length;
        int objNoteID;
        int objTailID;
        bool played;
    };

    struct TempoEvent
    {
        int numerator;
        int denominator;
        int qnScaleFactor;
        int qnLength;
        double time;
    };

    // Events are for things that have a position/length with no special data accociated with them
    // So drive/solo/freestyle are some examples.
    struct Event
    {
        EventType type;
        double time;
        double length;
    };
}
