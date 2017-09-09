#include "midi_chart.hpp"

#include <map>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>


namespace RGCCPP::Midi
{
    /////////////////////////////////////
    // TempoEvents Class methods
    /////////////////////////////////////

    void TempoEvents::add_tempo_event(int qnLength, double time)
    {
        // If this is the first change in tempo/timesignature create an event with the other half of state
        if (m_tempo.size() == 0)
        {
            m_tempo.push_back({0, 0, 0, qnLength, time});
        }
        else
        {
            // If the previous event is located at the same time as the current one overwrite the data to reflect the current state.
            auto &eventLast = m_tempo.back();
            if (eventLast.time == time)
            {
                eventLast.qnLength = qnLength;
            }
            else
            {
                // Otherwise we copy the previous change and update the data to reflect the new state.
                RGCCPP::TempoEvent tempoEvent = eventLast;
                tempoEvent.time = time;
                tempoEvent.qnLength = qnLength;
                m_tempo.push_back(tempoEvent);
            }
        }
    }

    void TempoEvents::add_time_sig_event(int numerator, int denominator, int compoundFactor, double time)
    {
        // If this is the first change in tempo/timesignature create an event with the other half of state
        if (m_tempo.size() == 0)
        {
            m_tempo.push_back({numerator, denominator, compoundFactor, 0, time});
        }
        else
        {
            // If the previous event is located at the same time as the current one overwrite the data to reflect the current state.
            auto &eventLast = m_tempo.back();
            if (eventLast.time == time)
            {
                eventLast.numerator = numerator;
                eventLast.denominator = denominator;
                eventLast.qnScaleFactor = compoundFactor;
            }
            else
            {
                // Otherwise we copy the previous change and update the data to reflect the new state.
                RGCCPP::TempoEvent tempoEvent = eventLast;
                tempoEvent.time = time;
                tempoEvent.numerator = numerator;
                tempoEvent.denominator = denominator;
                tempoEvent.qnScaleFactor = compoundFactor;

                m_tempo.push_back(tempoEvent);
            }
        }
    }

    std::vector<RGCCPP::TempoEvent*> &TempoEvents::get_events(double start, double end)
    {
        static std::vector<RGCCPP::TempoEvent*> events;
        events.clear();
        for (auto &tempo : m_tempo)
        {
            if (tempo.time >= start && tempo.time <= end)
            {
                events.push_back(&tempo);
            }
        }
        return events;
    }

    std::vector<RGCCPP::TempoEvent> &TempoEvents::get_events()
    {
        return m_tempo;
    }

    /////////////////////////////////////
    // Track Class
    /////////////////////////////////////

    Track::Track(Song* song)
    : m_song(song)
    {
    }

    void Track::add_note(RGCCPP::NoteType type, double time, int32_t pulseTime, bool on)
    {
        static std::vector<std::pair<RGCCPP::NoteType, int>> activeNotes;

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
    RGCCPP::NoteType midi_to_note_type(RGCCPP::Difficulty diff, int midiNote)
    {
        // Difficulty based notes
        switch(diff)
        {
            case RGCCPP::Difficulty::Expert:
                switch(midiNote)
                {
                    case expert_green: return RGCCPP::NoteType::Green;
                    case expert_red: return RGCCPP::NoteType::Red;
                    case expert_yellow: return RGCCPP::NoteType::Yellow;
                    case expert_blue: return RGCCPP::NoteType::Blue;
                    case expert_orange: return RGCCPP::NoteType::Orange;
                    default: break;
                }
                break;

            case RGCCPP::Difficulty::Hard:
                switch(midiNote)
                {
                    case hard_green: return RGCCPP::NoteType::Green;
                    case hard_red: return RGCCPP::NoteType::Red;
                    case hard_yellow: return RGCCPP::NoteType::Yellow;
                    case hard_blue: return RGCCPP::NoteType::Blue;
                    case hard_orange: return RGCCPP::NoteType::Orange;
                    default: break;
                }
                break;

            case RGCCPP::Difficulty::Medium:
                switch(midiNote)
                {
                    case medium_green: return RGCCPP::NoteType::Green;
                    case medium_red: return RGCCPP::NoteType::Red;
                    case medium_yellow: return RGCCPP::NoteType::Yellow;
                    case medium_blue: return RGCCPP::NoteType::Blue;
                    case medium_orange: return RGCCPP::NoteType::Orange;
                    default: break;
                }
                break;

            case RGCCPP::Difficulty::Easy:
                switch(midiNote)
                {
                    case easy_green: return RGCCPP::NoteType::Green;
                    case easy_red: return RGCCPP::NoteType::Red;
                    case easy_yellow: return RGCCPP::NoteType::Yellow;
                    case easy_blue: return RGCCPP::NoteType::Blue;
                    case easy_orange: return RGCCPP::NoteType::Orange;
                    default: break;
                }
                break;

            default: break;
        }
        
        // Global note types
        switch(midiNote)
        {
            case event_solo: return RGCCPP::NoteType::Solo;
            case event_drive: return RGCCPP::NoteType::Drive;
            default: return RGCCPP::NoteType::NONE;
        }
    }

    const std::string diff_type_to_name(RGCCPP::Difficulty diff)
    {
        switch(diff)
        {
            case RGCCPP::Difficulty::Expert: return "Expert";
            case RGCCPP::Difficulty::Hard: return "Hard";
            case RGCCPP::Difficulty::Medium: return "Medium";
            case RGCCPP::Difficulty::Easy: return "Easy";
            default: return "";
        }
    }

    const std::string track_type_to_name(RGCCPP::TrackType type)
    {
        switch(type)
        {
            case RGCCPP::TrackType::Guitar: return "Guitar";
            case RGCCPP::TrackType::Bass: return "Bass";
            case RGCCPP::TrackType::Drums: return "Drums";
            case RGCCPP::TrackType::Keys: return "Keys";
            case RGCCPP::TrackType::Vocals: return "Vocals";
            default: return "None";
        }
    }

    static const std::map<std::string, RGCCPP::TrackType> TrackTypeMap {
        {"PART GUITAR", RGCCPP::TrackType::Guitar},
        {"PART BASS", RGCCPP::TrackType::Bass},
        {"PART DRUMS", RGCCPP::TrackType::Drums},
        {"PART KEYS", RGCCPP::TrackType::Keys},
        {"PART VOCALS", RGCCPP::TrackType::Vocals},
        {"", RGCCPP::TrackType::NONE}
    };

    const RGCCPP::TrackType get_track_type(std::string trackName)
    {
        try
        {
            return TrackTypeMap.at(trackName);
        }
        catch (std::out_of_range &err)
        {
            err;
            return RGCCPP::TrackType::NONE;
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
            RGCCPP::TrackType type = get_track_type(track.name);
            if (type == RGCCPP::TrackType::Guitar)
            {
                midiTrack = &track;
            }

            // find the longest midi track
            if (m_length < midiTrack->endTime)
            {
                m_length = m_midi.pulsetime_to_abstime(midiTrack->endTime);
            }

        }

        // If we didnt find a track skip it.
        if (midiTrack == nullptr)
        {
            return false;
        }

        // Load tempo and time signature data
        const TempoTrack &tempoTrack = *m_midi.get_tempo_track();

        int32_t lastQnLength;

        for (auto &eventOrder : tempoTrack.tempoOrdering)
        {

            if (eventOrder.type == TtOrderType::TimeSignature)
            {
                auto &ts = tempoTrack.timeSignature[eventOrder.index];
                m_tempoEvents.add_time_sig_event(ts.numerator, ts.denominator, ts.thirtySecondPQN/8.0, m_midi.pulsetime_to_abstime(ts.info.info.pulseTime));
            }
            else if (eventOrder.type == TtOrderType::Tempo)
            {
                auto &tempo = tempoTrack.tempo[eventOrder.index];
                lastQnLength = tempo.qnLength;
                m_tempoEvents.add_tempo_event(tempo.qnLength, tempo.absTime);
            } 
        }

        m_tempoEvents.add_tempo_event(lastQnLength, m_length); // add final tempo change.

        std::vector<RGCCPP::Difficulty> diffs = {
            RGCCPP::Difficulty::Expert,
            RGCCPP::Difficulty::Hard,
            RGCCPP::Difficulty::Medium,
            RGCCPP::Difficulty::Easy
        };

        for (RGCCPP::Difficulty diff: diffs)
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

                    RGCCPP::NoteType note = midi_to_note_type(diff, midiEvent.data1);

                    double time = m_midi.pulsetime_to_abstime(midiEvent.info.pulseTime);

                    switch (note)
                    {
                        // Event markers
                        case RGCCPP::NoteType::Solo:
                            track.set_event(EventType::solo, time, noteOn);
                            break;
                        case RGCCPP::NoteType::Drive:
                            track.set_event(EventType::drive, time, noteOn);
                            break;

                        // Track Notes
                        case RGCCPP::NoteType::Green:
                        case RGCCPP::NoteType::Red:
                        case RGCCPP::NoteType::Yellow:
                        case RGCCPP::NoteType::Blue:
                        case RGCCPP::NoteType::Orange:
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
}