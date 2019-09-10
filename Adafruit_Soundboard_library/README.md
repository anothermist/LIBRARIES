# Adafruit_Soundboard_library

This is a library for the Adafruit Audio FX Sound Boards in UART mode

* [Adafruit Audio FX Mini Sound Board - WAV/OGG Trigger - 2MB Flash](https://www.adafruit.com/products/2342)
* [Adafruit Audio FX Mini Sound Board - WAV/OGG Trigger - 16MB Flash](https://www.adafruit.com/products/2341)
* [Adafruit Audio FX Sound Board + 2x2W Amp - WAV/OGG Trigger - 2MB](https://www.adafruit.com/products/2210)
* [Adafruit Audio FX Sound Board + 2x2W Amp - WAV/OGG Trigger - 16MB](https://www.adafruit.com/products/2217)
* [Adafruit Audio FX Sound Board - WAV/OGG Trigger with 2MB Flash](https://www.adafruit.com/products/2133)
* [Adafruit Audio FX Sound Board - WAV/OGG Trigger with 16MB Flash](https://www.adafruit.com/products/2220)

Check out the links above for our tutorials and wiring diagrams
This sound fx driver uses TTL Serial to communicate

This is a list of the UART strings that will trigger actions on the Sound Board. For more details, see [Adafruit_Soundboard.cpp](Adafruit_Soundboard.cpp) (all commands must be followed by a carriage return character (`\r`) and then a new line character(`\n`), just like Serial.println())

| ASCII Character(s) sent to Sound Board | Description |
| :---: | :---: |
| `L` | [return a list of all the files on the Sound Board over the serial connection](Adafruit_Soundboard.cpp#L94) | 
| `#{n}` | [play song number "{n}"](Adafruit_Soundboard.cpp#L142) | 
| `P{name}` | [play song with a name of "{name}"](Adafruit_Soundboard.cpp#L170) | 
| `+` | [increase the volume](Adafruit_Soundboard.cpp#L195) | 
| `-` | [decrease the volume](Adafruit_Soundboard.cpp#L208) | 
| `=` | [pause playback](Adafruit_Soundboard.cpp#L221) | 
| `>` | [resume playback](Adafruit_Soundboard.cpp#L235) | 
| `q` | [stop playback](Adafruit_Soundboard.cpp#L248) | 
| `t` | [return the current time in the track and the total time of the track in seconds over the serial connection](Adafruit_Soundboard.cpp#L262) | 
| `s` | [return the remaining size of the track and the total size of the track in bytes over the serial connection](Adafruit_Soundboard.cpp#L277) | 


Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
MIT license, all text above must be included in any redistribution

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder Adafruit_Soundboard. Check that the Adafruit_Soundboard folder contains Adafruit_Soundboard.cpp and Adafruit_Soundboard.h

Place the Adafruit_Soundboard library folder your *arduinosketchfolder*/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE

We also have a great tutorial at:
https://learn.adafruit.com/adafruit-audio-fx-sound-board
