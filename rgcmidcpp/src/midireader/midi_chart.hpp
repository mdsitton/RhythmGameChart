#include "smf.hpp"

#include <cstdint>

#include "song_types.hpp"


namespace RGCCPP::Midi
{
    class Song;


    class TempoEvents
    {
    public:
        void add_tempo_event(int ppqn, double time);
        void add_time_sig_event(int numerator, int denominator, int compoundFactor, double time);

        std::vector<RGCCPP::TempoEvent*> &get_events(double start, double end);
        std::vector<RGCCPP::TempoEvent> &get_events();

    private:
        std::vector<RGCCPP::TempoEvent> m_tempo;
    };

    class Track
    {
    public:

        Track(Song* song);

        void add_note(RGCCPP::NoteType type, double time, int32_t pulseTime, bool on);
        std::vector<RGCCPP::TrackNote*> &get_notes_in_frame(double start, double end);
        std::vector<RGCCPP::TrackNote> &get_notes();

        void set_event(RGCCPP::EventType type, double time, bool on);
        std::vector<RGCCPP::Event> &get_events();

    private:
        Song* m_song;
        std::vector<RGCCPP::TrackNote> m_notes;
        std::vector<RGCCPP::Event> m_events;
    };

    class Song
    {
    public:
        Song(std::string songPath);
        bool load();
        std::vector<Track> *get_tracks();
        int16_t get_divison();
    private:
        SmfReader m_midi;
        double m_length;
        std::vector<Track> m_tracks;
        TempoEvents m_tempoEvents;
    };


    // Functions are mainly used within the Song class
    RGCCPP::NoteType midi_to_note_type(RGCCPP::Difficulty diff, int midiNote);
    const std::string diff_type_to_name(RGCCPP::Difficulty diff);
    const std::string track_type_to_name(RGCCPP::TrackType type);
    const RGCCPP::TrackType get_track_type(std::string trackName);
}