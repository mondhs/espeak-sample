espeak-sample
=============

# Espeak usage samples 

It is tested on Ubuntu 12.04 no other OS are supported. Distributors are welcome.

Before test you need check out code and build from: https://github.com/mondhs/espeak.

Sample uses header files from source dir(<ESPEAK_SRC_DIR>). Example where source should be located: /home/user/src/espeak/src

# Files

## sampleSpeak.cpp
* Shows how to write simple cpp code to call espeak.
* Sample downloaded: http://djkaos.wordpress.com/2009/12/27/simple-espeak-program-speak_lib/
* Compile(be sure you replace  <ESPEAK_SRC_DIR>): 
 * g++ -g -I.  -I <ESPEAK_SRC_DIR> sampleSpeak.cpp -lportaudio -lespeak -o sampleSpeak
* Run: ./speak

## audacityLabelSpeak.cpp
* Shows how to write callback function.
* It stores generated audio file to wav and phoneme transcriptions. 
* Compile(be sure you replace  <ESPEAK_SRC_DIR>): 
 * g++ -g -I. -I <ESPEAK_SRC_DIR> audacityLabelSpeak.cpp -lportaudio -lespeak -o audacityLabelSpeak
* Run: ./audacityLabelSpeak

## talkingClockEspeak.py
* Shows how to call espeak from python
* Run: ./talkingClockEspeak.py
