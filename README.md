espeak-sample
=============

# Espeak usage samples 

It is tested on Ubuntu 12.04 no other OS are supported. Distributors are welcome.

Before test you need check out code and build from: https://github.com/mondhs/espeak.

Sample uses header files from source dir(<ESPEAK_SRC_DIR>). Example where source should be located: /home/user/src/espeak/src

# Sample output

Directory output contains some code sample outputs. each output contains original script name and only extension is changed.

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

## sampleMbrolaSpeak.cpp

* Shows how to write simple cpp code to call espeak with Mbrola voice.
* Compile(be sure you replace  <ESPEAK_SRC_DIR>): 
 * g++ -g -I.  -I <ESPEAK_SRC_DIR> sampleSpeak.cpp -lportaudio -lespeak -o sampleSpeak
 * e.g g++ -g -I.  -I ../espeak sampleMbrolaSpeak.cpp -lportaudio -lespeak -o sampleMbrolaSpeak

# Other examples

* ```espeak -v lt -s120 -k20   -f sample.txt```
* ```espeak -v mb-lt1 -s120 -k20   -f sample.txt```

## Code Analysis


* Mbrola Events are triggered: 
 * speak_lib#espeak_Synth()
 * ->speak_lib#sync_espeak_Synth()
 * ->speak_lib#Synthesize(text)
 * ->synthesize#Generate(phoneme_list)
 * ->synth_mbrola#MbrolaGenerate(phoneme_list===plist)
 * ->synth_mbrola#MbrolaTranslate()
 * ->synthesize#DoPhonemeMarker()
* Mbrola phoneme generation:
 * speak_lib#Synthesize(text)
 * ->synthesize#SpeakNextClause(text===p_text)(What is this: phoneme_callback)
 * ->TranslateClause(p_text)
 * ... ->translate#MakePhonemeList(){phlist = phoneme_list}
* tranlator created in translate#LoadVoices()
