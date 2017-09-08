#include "midi_chart.hpp"

#include <map>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>


/////////////////////////////////////
// Track Class
/////////////////////////////////////

Track::Track(Song* song)
: m_song(song)
{
}

void Track::add_note(NoteType type, double time, int32_t pulseTime, bool on)
{
    static std::vector<std::pair<NoteType, int>> activeNotes;

    if (on)
    {
        int index = m_notes.size();
        activeNotes.emplace_back(type, index);
        m_notes.push_back({type, time, pulseTime});
    }
    else
    {
        auto findFunc = [&](const auto& element)
        {
            return element.first == type;
        };
        auto item = std::find_if(activeNotes.begin(), activeNotes.end(), findFunc);
        if (item != activeNotes.end())
        {

            auto &note = m_notes[item->second];

            int tailCutoff = std::ceil(m_song->get_divison()/3.0);
            int pulseLength = pulseTime - note.pulseTimeStart;


            if (pulseLength <= tailCutoff)
            {
                note.length = 0.0;
                note.pulseTimeEnd = note.pulseTimeStart;
            }
            else
            {
                note.length = time - note.time;
                note.pulseTimeEnd = pulseTime;
            }
            activeNotes.erase(item);
        }
    }
}

void Track::set_event(EventType type, double time, bool on)
{
    static std::vector<std::pair<EventType, int>> activeEvents;

    if (on)
    {
        int index = m_events.size();
        activeEvents.emplace_back(type, index);
        m_events.push_back({type, time, 0.0});
    }
    else
    {
        auto findFunc = [&](const auto& element)
        {
            return element.first == type;
        };
        auto item = std::find_if(activeEvents.begin(), activeEvents.end(), findFunc);
        if (item != activeEvents.end())
        {
            auto &event = m_events[item->second];
            event.length = time - event.time;
            activeEvents.erase(item);
        }
    }
}

std::vector<Event> &Track::get_events()
{
    return m_events;
}

std::vector<TrackNote*> &Track::get_notes_in_frame(double start, double end)
{   
    static std::vector<TrackNote*> notes;
    // The idea here is to reuse the same vector each frame to reduce allocations.
    notes.clear();
    for ( auto &note : m_notes)
    {
        if (note.time >= start && note.time <= end)
        {
            notes.emplace_back(&note);
        }
    }
    return notes;
}

std::vector<TrackNote> &Track::get_notes()
{
    return m_notes;
}

// Midi note values for guitar track.
const uint8_t expert_green = 0x60;
const uint8_t expert_red = 0x61;
const uint8_t expert_yellow = 0x62;
const uint8_t expert_blue = 0x63;
const uint8_t expert_orange = 0x64;
const uint8_t hard_green = 0x54;
const uint8_t hard_red = 0x55;
const uint8_t hard_yellow = 0x56;
const uint8_t hard_blue = 0x57;
const uint8_t hard_orange = 0x58;
const uint8_t medium_green = 0x48;
const uint8_t medium_red = 0x49;
const uint8_t medium_yellow = 0x4a;
const uint8_t medium_blue = 0x4b;
const uint8_t medium_orange = 0x4c;
const uint8_t easy_green = 0x3c;
const uint8_t easy_red = 0x3d;
const uint8_t easy_yellow = 0x3e;
const uint8_t easy_blue = 0x3f;
const uint8_t easy_orange = 0x40;
const uint8_t event_solo = 0x67;
const uint8_t event_drive = 0x74;

/////////////////////////////////////
// Conversion functions for taking data from the midi into internal formats.
/////////////////////////////////////

// Convert midi notes into a note type
NoteType midi_to_note_type(Difficulty diff, int midiNote)
{
    // Difficulty based notes
    switch(diff)
    {
        case Difficulty::Expert:
            switch(midiNote)
            {
                case expert_green: return NoteType::Green;
                case expert_red: return NoteType::Red;
                case expert_yellow: return NoteType::Yellow;
                case expert_blue: return NoteType::Blue;
                case expert_orange: return NoteType::Orange;
                default: break;
            }
            break;

        case Difficulty::Hard:
            switch(midiNote)
            {
                case hard_green: return NoteType::Green;
                case hard_red: return NoteType::Red;
                case hard_yellow: return NoteType::Yellow;
                case hard_blue: return NoteType::Blue;
                case hard_orange: return NoteType::Orange;
                default: break;
            }
            break;

        case Difficulty::Medium:
            switch(midiNote)
            {
                case medium_green: return NoteType::Green;
                case medium_red: return NoteType::Red;
                case medium_yellow: return NoteType::Yellow;
                case medium_blue: return NoteType::Blue;
                case medium_orange: return NoteType::Orange;
                default: break;
            }
            break;

        case Difficulty::Easy:
            switch(midiNote)
            {
                case easy_green: return NoteType::Green;
                case easy_red: return NoteType::Red;
                case easy_yellow: return NoteType::Yellow;
                case easy_blue: return NoteType::Blue;
                case easy_orange: return NoteType::Orange;
                default: break;
            }
            break;

        default: break;
    }
    
    // Global note types
    switch(midiNote)
    {
        case event_solo: return NoteType::Solo;
        case event_drive: return NoteType::Drive;
        default: return NoteType::NONE;
    }
}

const std::string diff_type_to_name(Difficulty diff)
{
    switch(diff)
    {
        case Difficulty::Expert: return "Expert";
        case Difficulty::Hard: return "Hard";
        case Difficulty::Medium: return "Medium";
        case Difficulty::Easy: return "Easy";
        default: return "";
    }
}

const std::string track_type_to_name(TrackType type)
{
    switch(type)
    {
        case TrackType::Guitar: return "Guitar";
        case TrackType::Bass: return "Bass";
        case TrackType::Drums: return "Drums";
        case TrackType::Keys: return "Keys";
        case TrackType::Vocals: return "Vocals";
        default: return "None";
    }
}

static const std::map<std::string, TrackType> midiTrackTypeMap {
    {"PART GUITAR", TrackType::Guitar},
    {"PART BASS", TrackType::Bass},
    {"PART DRUMS", TrackType::Drums},
    {"PART KEYS", TrackType::Keys},
    {"PART VOCALS", TrackType::Vocals},
    {"", TrackType::NONE}
};

const TrackType get_track_type(std::string trackName)
{
    try
    {
        return midiTrackTypeMap.at(trackName);
    }
    catch (std::out_of_range &err)
    {
        err;
        return TrackType::NONE;
    }
}

Song::Song(std::string songPath)
:m_midi(songPath+"/notes.mid")
{

}

bool Song::load()
{

    SmfTrack* midiTrack = nullptr;

    // for now we will only load a single track until we get the rgc writer finished.
    for (SmfTrack& track : m_midi.get_tracks())
    {
        TrackType type = get_track_type(track.name);
        if (type == TrackType::Guitar)
        {
            midiTrack = &track;
        }
    }

    // If we didnt find a track skip it.
    if (midiTrack == nullptr)
    {
        return false;
    }

    std::vector<Difficulty> diffs = {
        Difficulty::Expert,
        Difficulty::Hard,
        Difficulty::Medium,
        Difficulty::Easy
    };

    for (Difficulty diff: diffs)
    {
        m_tracks.emplace_back(this);
        
        auto &track = m_tracks.back();

        for (auto &midiEvent : midiTrack->midiEvents)
        {
            // Handle velocity = 0 to turn notes off
            if (midiEvent.message == NoteOn && midiEvent.data2 == 0)
            {
                midiEvent.message = NoteOff;
            }

            if (midiEvent.message == NoteOn || midiEvent.message == NoteOff)
            {
                // To reduce code duplication use the same codepath for notes on/off but convert event types to bool.
                bool noteOn = false;
                if (midiEvent.message == NoteOn)
                {
                    noteOn = true;
                }

                NoteType note = midi_to_note_type(diff, midiEvent.data1);

                double time = m_midi.pulsetime_to_abstime(midiEvent.info.pulseTime);

                switch (note)
                {
                    // Event markers
                    case NoteType::Solo:
                        track.set_event(EventType::solo, time, noteOn);
                        break;
                    case NoteType::Drive:
                        track.set_event(EventType::drive, time, noteOn);
                        break;

                    // Track Notes
                    case NoteType::Green:
                    case NoteType::Red:
                    case NoteType::Yellow:
                    case NoteType::Blue:
                    case NoteType::Orange:
                        track.add_note(note, time, midiEvent.info.pulseTime, noteOn);
                        break;
                    default: break;
                }
            }
        }
    }
    return true;

}


int16_t Song::get_divison()
{
    return m_midi.get_header()->division;
}