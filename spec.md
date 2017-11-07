# Rhythm Game Chart Specification

## Overall filestructure:

`<header> <metadata> <timed global song events> <Instrument Track 1> <Instrument Track N...>`

RGC is has a little endian byte order unlike midi for example which is big endian.

All sections are 8 byte aligned and padded.

## Header

The header is 16 bytes with only the first 12 bytes currently in use. The final 4 bytes are reserved for future use.


Byte order mark
- 32-bit unsigned int
- The byte order mark interprited as ascii as visible in BE order: RGCf
  - This means the actual written LE version of the BOM is fCGR.
  - This is not an ascii string, the BOM is however chosen to view as ascii characters..
  - The bom in BE hex is: 0x52 0x47 0x43 0x66 

Version Number
- We should have a version number in the header incase we need to ammend the format due to issues or functionality improvments.
  - 16-bit unsigned int
  - Also backwards compatibility should be taken into consideration for new format versions.

Ticks per quarter note.
- 16-bit unsigned int 
- This value gives context of how to parser all of the events in the event tracks
  - I considered putting it in the metadata section but decided not to because you shouldnt need to parse the metadata section in order to parse the tracks.
- Track events use these ticks to define location and length of events.

Chart Version
- 16-bit unsigned int
- This should be incremented each time the file is changed.

Size of offset table
- 16-bit unsigned int.
- The number of bytes in the offset table.
- This will always be in multiples of 8 because of it being 8-byte aligned and padded.
- Metadata section will always start at the position 16+offsetSize

Number of items in the track offset table.
- 8-bit unsigned int.

3 bytes unused in header.

### Track Offset Table:
- An offset table will store file offsets from the start of the metadata section for all tracks in the file.
  - Metadata will always be offset 0 but it is included anyways.
  - These will be 64-bit offsets encoded to a vlv. This gives us a max of 9 vlv bytes for offsets.


## Metadata

The general idea of this section is to move song.ini infomation into the chart format.
This will be a key, value list storing different properties about the song and instrument tracks.

We need some method to express a config table
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
- `vox_gender` Maybe could be useful?

Instrument/track specific properties:
- `difficulty` Average difficulty of this track
- `audio_file` Explicity defines the audio file attached to this track.
  - One issue is how to handle drum tracks where there are multiple tracks for different drum types that should be attached to a specific note lane.
- `audio_level`
- `drum_kit_type` This isn't really very useful unless we want to have multiple drum kit audio sounds for freestyle sections and then to have a default one.


## Global Events

`<track header> <timed event data>`

This track is primarally for storing tempo and time signature changes.

This track cannot be empty and should always contain atleast one time signature and one tempo event.

Like midi if there is no tempo events or no time signature events starting at time=0 it will default to:

- 120 BPM tempo equivelent
- 4/4 time signature

tempo events are structured as follows:

`<deltaTicks> <µs per quarter note>`

Type info:
- `<deltaTicks>` - vlv
- `<µspqn>` - 32bit unsigned int

timeSignature events are structured as follows:

`<deltaTicks> <numerator> <denominator>`

Type info:

- `<deltaTicks>` - vlv
- `<numerator>` - 8bit unsigned int
- `<denominator>` - 8bit unsigned int (This is not 2^den like midi is)


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

<length> is a variable length tick count


##### Mod

*WIP*

Mod types:

`0x1` force strum - deactivates any active force hopo or force tap

`0x2` force hopo - deactivates any active force strum or force tap

`0x3` force tap - deactivates and active force strum or force hopo

`0x4` solo start/end - this could work as a toggle solo so we only need one event type



## Variable Length Values

A variable length value or VLV is a multibyte encoded unsigned integer value. A VLV uses the most significant bit of each byte as a continuation flag. VLV's are limited in size only by the container in which they are derived to/from.

Some issues to note with VLV's related to size. There MUST not be any unused extra bits in a VLV, every bit must map into the container. Essentially any spare bits will cause the decoded value to be shifted by that number of unused bits trunicating the higher order bits. This is why the vlv's used in midi files are limited to 4 bytes with 28-bits of information.

Ex:
`33025` `0b00000000000000001000000100000001` in vlv takes 3 bytes:
`0b10000001 0b10000001 0b00000001`
