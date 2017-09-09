Rhythm Game Chart
=================

This is a new rhythm game chart file. It is binary, and has song meta-data embeded within it. This repo contains a WIP specfication, and several projects in development for reading and writing the format.

Various development goals are:

- Faster than .chart
- More compact note data than midi/chart
- Extendable for different game requirements.
- Setup so that the format can be partially read for increased parsing speeds in some situations(such as song loading/scanning).
- Embeded metadata so the song.ini no longer needs to exist.


Currently there are various games/tools that should eventually support the format:

 - MoonScraper
 - OpenRhythm
 - Clone Hero