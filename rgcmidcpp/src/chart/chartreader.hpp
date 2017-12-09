#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <cstdint>


struct SectionData
{
    using LineData = std::pair<std::string_view, std::string_view>;
    std::string name;
    std::vector<LineData> data;

    SectionData(std::string_view str)
    :name(str)
    {
    }
};

struct SongSection
{
    std::string name;
    std::string artist;
    std::string album;
    std::string year;
    std::string charter;
    std::string genre;
    int resolution;
    float offset;
    using LineData = std::pair<std::string, std::string>;
    std::vector<LineData> unused;
};

enum EventType
{
    NONE,
    Tempo,
    Anchor,
    TimeSignature,
    Text,
    Note,
    Special,
    HandAnimationPosition,
};

// EventInfo structs are used to properly order events of different types in time.
// It contains everything about an event except for its data.
struct EventInfo
{
    uint64_t tickTime;
    uint32_t deltaTicks;
    EventType type;
    size_t index; // This is the index of an event within its array depending on its type.
};

struct TempoEvent
{
    uint32_t qnLength;
};

struct TimeSignatureEvent
{
    int numerator;
    int denominator;
};

// struct AnchorEvent
// {
//  uint32_t time;
// }

struct TempoMap
{

    std::vector<EventInfo> tempoOrder;
    std::vector<TempoEvent> tempos;
    std::vector<TimeSignatureEvent> timeSignatures;
    // std::vector<AnchorEvent> anchors;
};

struct TextEvent
{
    std::string text;
};

// skip hand events until i can figure them out better
// struct HandEvent
// {
//     int val1;
// }

enum Difficulty
{
    Easy,
    Medium,
    Hard,
    Expert,
};

enum Instrument
{
    Guitar,
    Bass,
    Drums,
    Keys,
};

struct SpecialEvent
{
    int type;
    int length;
};

struct NoteEvent
{
    int type;
    int length;
};

struct TrackEvents
{
    Instrument instrument;
    Difficulty difficulty;
    std::vector<EventInfo> tempoOrder;
    std::vector<NoteEvent> notes;
    std::vector<SpecialEvent> specials;
};

class ChartReader
{
public:
    ChartReader(std::string path);
    void read();
private:
    std::string_view next_line(std::string_view str);
    std::string m_path;
    std::string m_buffer;
    size_t m_bufferCharPos;
    bool m_eob;

    std::vector<SectionData> m_sections;
    SongSection m_metadata;

    TempoMap tempoMap;
    std::vector<TrackEvents> tracks;
};