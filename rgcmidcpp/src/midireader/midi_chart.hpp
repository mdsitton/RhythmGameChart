#include "smf.hpp"

#include <cstdint>

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

// Events are for things that have a position/length with no special data accociated with them
// So drive/solo/freestyle are some examples.
struct Event
{
    EventType type;
    double time;
    double length;
};

class Song;

class Track
{
public:

    Track(Song* song);

    void add_note(NoteType type, double time, int32_t pulseTime, bool on);
    std::vector<TrackNote*> &get_notes_in_frame(double start, double end);
    std::vector<TrackNote> &get_notes();

    void set_event(EventType type, double time, bool on);
    std::vector<Event> &get_events();

private:
    Song* m_song;
    std::vector<TrackNote> m_notes;
    std::vector<Event> m_events;
};

class Song
{
public:
    Song(std::string songPath);
    bool load();
    int16_t get_divison();
private:
    SmfReader m_midi;
    std::vector<Track> m_tracks;
};


// Functions are mainly used within the Song class
NoteType midi_to_note_type(Difficulty diff, int midiNote);
const std::string diff_type_to_name(Difficulty diff);
const std::string track_type_to_name(TrackType type);
const TrackType get_track_type(std::string trackName);