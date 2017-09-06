# Chart Format Specification

## Overall filestructure:

`<header> <metadata> <timed global song events> <Instrument Track 1> <Instrument Track N...>`


## Header

Byte order mark
- This could be a 4 byte ascii sequence that would be tied back to the name of the data format we decide on.
  - or maybe not I dont really know a lot about BOM's

Version Number
- We should have a version number in the header incase we need to ammend the format due to issues or functionality improvments.
  - Also backwards compatibility should be taken into consideration for new format versions.

Track Offsets:
- An offset table will store file offsets from the start of the metadata section for all tracks in the file.
  - Metadata will always be offset 0 so there really isnt much reason to include it in the table.


## Metadata

The general idea of this section is to move song.ini infomation into the chart format.
This will be a key, value list storing different properties about the song and instrument tracks.


We need some method to express a config he
We should have a sub-section for each track in the song as well as a global section.

Ideas for global properties:
- `song_name`
- `artist`
- `cover` Triggers the "famous by" text in-game
- `charter`
- `setlist` This is a more generically named version of the icon property in a song.ini
- `difficulty` This is full band average difficulty
- `preview_start` Time in milliseconds of the start of the song preview
- `preview_length` Length of preview in milliseconds
  - another option for preview timing is to just have 1 type with 2 values start/end time or start/length
- `audio_offset` Audio offset for easier syncing of audio to chart without re-encoding of lossy audio formats.
- `song_length` Not really sure how useful this would be... so this is a maybe.
- `loading_text`

Instrument/track specific properties:
- `difficulty` Average difficulty of this track
- `audio_file` Explicity defines the audio file attached to this track.
  - One issue is how to handle drum tracks where there are multiple tracks for different drum types that should be attached to a specific note lane.
- `audio_level`
- `drum_kit_type` This isn't really very useful unless we want to have multiple drum kit audio sounds for freestyle sections and then to have a default one.



## Instrument tracks

`<track header> <timed event data> <difficulty 1 data> <difficulty N... data>`
    
This works similarly to midi:
`<var len delta tick since last event> <event>`

The first byte in any event serves two purposes.
- Event type
- Event length

This leaves us with 2 event types and a max of 127 bytes per event.
  - If we dont restrict to 2 event types we end up having multi-note events that can be longer than the support event length.. not a good situation to have. This is the fix for now unless anyone has any strong objections.

### Event types

`0b00000000` Note event - Note or multiple notes.

`0b10000000` Mod event - events which modify the meaning of a note or set of notes.



#### Track Event descriptions

##### Note
Each note event defines a set of notes which occur at the same instant. There are several different forms of a note event.

`<event byte count and type> <note count> <note 1> <note N...>`
- No Length

`<event byte count and type> <note count> <length> <note 1> <note N...>`
- Single length
  - Note, the reason for having length first is that you are expanding the multi-note event out into single notes internally it's easier to have the length first since you don't need to keep track of the all of the previous notes in the event.

`<event byte count and type> <note count> <note 1> <length> <note N...> <length>`
- Multi-length chords.

These are specified by the two highest bits in note count:

`0b00xxxxxx` Note event with no length.

`0b10xxxxxx` Note event with a constant length for all notes.

`0b11xxxxxx` Note event with diffent lengths for all notes.

`0b01xxxxxx` Undefined.

*Note these are not set in stone we may rearrange this somewhat*


##### Mod

*WIP*

Mod types:

`0x1` force strum - deactivates any active force hopo or force tap

`0x2` force hopo - deactivates any active force strum or force tap

`0x3` force tap - deactivates and active force strum or force hopo

`0x4` solo start/end - this could work as a toggle solo so we only need one event type
