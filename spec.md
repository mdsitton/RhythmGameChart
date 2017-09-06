Overall filestructure:

    `<header> <metadata> <timed global song events> <Instrument Track 1> <Instrument Track N...>`


Header:

    Byte order mark:
        - This could be a 4 byte ascii sequence that would be tied back to the name of the data format we decide on.
            - or maybe not i dont really know a lot about BOM's

    Version Number
        - We should have a version number in the header incase we need to ammend the format due to issues or functionality improvments.
            - Also backwards compatibility should be taken into consideration for new format versions.

    Track Offsets:
        - An offset table will store file offsets from the start of the metadata section for all tracks in the file.
        - Metadata will always be offset 0 so there really isnt much reason to include it in the table.


Metadata:

    The general idea of this section is to move song.ini infomation into the chart format.
    This will be a key, value list storing different properties about the song and instrument tracks.


    We need some method to express a config he
    We should have a sub-section for each track in the song as well as a global section.

    Ideas for global properties:
        song_name
        artist
        cover - Triggers the "famous by" text in-game
        charter
        setlist - This is a more generically named version of the icon property in a song.ini
        difficulty - This is full band average difficulty
        preview_start - time in milliseconds of the start of the song preview
        preview_length - length of preview in milliseconds
            * another option for preview timing is to just have 1 type with 2 values start/end time or start/length
        audio_offset - Audio offset for easier syncing of audio to chart without re-encoding of lossy audio formats.
        song_length - Not really sure how useful this would be... so this is a maybe.
        loading_text

    Instrument/track specific properties:
        difficulty - Average difficulty of this track
        audio_file - Explicity defines the audio file attached to this track.
            * One issue is how to handle drum tracks where there are multiple tracks for different drum types that should be attached to a specific note lane.
        audio_level
        drum_kit_type - This isn't really very useful unless we want to have multiple drum kit audio sounds for freestyle sections and then to have a default one.



Instrument tracks:

    `<track header> <timed event data> <difficulty 1 data> <difficulty N... data>`
    
    This works similarly to midi:
    <var len delta tick since last event> <event>

    The first byte in any event serves two purposes.
        - Event type
        - Event length

    This leaves us with 15 event types and a max of 15 bytes per event whice seems like a reasonable limitation to me.

    0x1 note on/off toggle
        - Multiple chords would use the most significant bit for note continuation.
        - 0x13 0x81 0x82 0x03
      * The original idea with the continuation bit was not having to store the number of notes in an event.
        - This could still be useful if we need to store any data in a note event after the notes... Otherwise it can probably be removed.
    0x2 note immediate
        - Multiple chords same as above
        - 0x21 0x02
      * Note there are a couple issues up in the air here:
        - We can either use note value + note length for notes or the above method.
        - Problem with note + length is extended sustains where you have the first notes in a chord starting and ending at different times.
            - This could be solved by just having notes in those case use a seperate event, however I would like chords to work consistantly including es.

    0x3 force strum - deactivates any active force hopo or force tap
    0x4 force hopo  - deactivates any active force strum or force tap
    0x5 force tap - deactivates and active force strum or force hopo
      * We could have this as a stack or just a normal state which is set and reset by each event.
        - After thinking about it its probably best to just have it be a normal state variable and forgo using a stack
        - I doubt it will save us anything, and will probably cost us needing to have a byte to denote push/pop on the stack.

    0x6 - solo start/end - this could work as a toggle solo so we only need one event type

