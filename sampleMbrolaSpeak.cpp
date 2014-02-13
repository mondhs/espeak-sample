/* 
   TITLE: labai paprasta eSpeak C++ naudojimo pavizdys speak_lib.h
   AUTHOR:Mindaugas Greibus
   LICENSE: GPLv2
*/


#include <./src/speak_lib.h>

int main(int argc, char* argv[] ) {
	char text[32] = {"1. Labas. Šauk. Laikas. Taika"};//Tariamas tekstas
	espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0 ); // paruošiamas eSpeak variklis
	espeak_SetVoiceByName("mb-lt1");// nustatomas Mbrola lietuvių 1 garas
	unsigned int size = 0; // saičius kiek tariamų simbolių yra masyve
	while(text[size]!='\0') size++;//suskaičiuojam pilnu perinkimu tariamus simbolius ==strlen(text)
	unsigned int flags=espeakCHARS_AUTO | espeakENDPAUSE;//Auto: utf8 ar 8bit, 
	espeak_Synth( text, size+1, 0,POS_CHARACTER,0, flags, NULL, NULL );// grafema->fonema->garsas transformacija
	espeak_Synchronize( ); //Lukterėti kol pasibaigs viską ištarti.
	return 0;
}
