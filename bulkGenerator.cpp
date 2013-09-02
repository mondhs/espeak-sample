/**
 * TITLE: bulk wav generator 
 * and phoneme transcription(in audacity format).
 * AUTHOR: Mondhs Bus
 * LICENSE: GPLv2
 */
#include <string.h>
#include <malloc.h>
#include <speak_lib.h>


//internal espeak stuff
espeak_POSITION_TYPE position_type;
espeak_AUDIO_OUTPUT output;
char *path=NULL;
void* user_data;
unsigned int Size, position=0,end_position=0, synth_flags=espeakCHARS_AUTO, *unique_identifier;


// important settings as without this it would not get notification on phoneme activity from espeak
int Options=espeakINITIALIZE_PHONEME_EVENTS;
unsigned int samples_total = 0;
unsigned int previous_position=0;
const char *previous_word;
int samplerate;
int gapBetweenWords = 20;

//parameters used by this code
char Voice[] = {"mb-lt1"};
char fileName[] = {"/tmp/audacityLabelSpeak.phn.txt"};
char wavefile[] = {"stdout"};
FILE *f_events = NULL;
FILE *f_wavfile = NULL;




//functions:
const char *WordToString(unsigned int word);
static void Write4Bytes(FILE *f, int value);
int OpenWavFile(char *path, int rate);
static void CloseWavFile();




/**
 * Most important peace of functionality. 
 * Callback function is invoked from espeak lib and store 
 * audio and phoneme level transcriptions to files 
 */
int WavSynthCallback(short *wav, int numsamples, espeak_EVENT *events)
{//====================================================================
	int type;
	//some wav activity.
	if(wav == NULL){
		CloseWavFile();
		return(0);
	}
	
	//Open wave file if it is not open
	if(f_wavfile == NULL){
			if(OpenWavFile(wavefile, samplerate) != 0){
				return(1);
			}
	}
	//Write wav file
	if(numsamples > 0){
		samples_total += numsamples;
		fwrite(wav,numsamples*2,1,f_wavfile);
	}
	return(0);
}

int TxtSynthCallback(short *wav, int numsamples, espeak_EVENT *events)
{//====================================================================
	int type;
		
	//write phoneme transcribtion to file
	f_events = stdout;
	while((type = events->type) != 0){
		if(events->type == espeakEVENT_SAMPLERATE){
			samplerate = events->id.number;
		}else if(type==espeakEVENT_PHONEME){
			const char *word = WordToString(events->id.number);
			fprintf(f_events,"%4.3f\t%4.3f\t%s\n", (float)previous_position/1000, (float)events->audio_position/1000,word );  
			previous_position=events->audio_position;
			previous_word=word;
		}
		events++;
	}
	
	return(0);
}

/**
 * Main 
 */
int main(int argc, char* argv[] ){
	char GENERATION_TYPE_TXT[] = "txt";
	char GENERATION_TYPE_WAV[] = "wav";
	char generation_type[3];
	if(argc == 2 ){
		char *aGenerationType = argv[1];
		strcpy(generation_type, aGenerationType);
	}else{
		printf("argument required:  [wav|txt]");
		return 1;
	}
	f_events = fopen(fileName,"w");
	fclose(f_events);
	
    output = AUDIO_OUTPUT_SYNCHRONOUS;
    espeak_Initialize(output, 0, path, Options ); 
    espeak_SetParameter(espeakWORDGAP,gapBetweenWords,0);
    
    
    //set call back
    if(strcmp(GENERATION_TYPE_WAV,generation_type) == 0){
		
		espeak_SetSynthCallback(WavSynthCallback);
	}else if(strcmp(GENERATION_TYPE_TXT,generation_type) == 0){
		espeak_SetSynthCallback(TxtSynthCallback);
	}
    
    espeak_SetVoiceByName(Voice);
    //Saying
	int max = 1000;
	char *p_text = (char *)malloc(max);

	
	int ix = 0;
	while(!feof(stdin)){
		p_text[ix++] = fgetc(stdin);
		if(ix >= (max-1)){
			max += 1000;
			p_text = (char *)realloc(p_text,max);
			printf("\n:loop %s\n", p_text); 
		}
		
	}
	if(ix > 0){
		p_text[ix-1] = 0;
		espeak_Synth(p_text,ix+1,0,POS_CHARACTER,0,synth_flags,NULL,NULL);
	}

    //Done
    //printf("\n:Done\n"); 
    return 0;
}

////////////////////////////////////////////////////////////////////////////
// Static functions, copy/pasted from espeak. Should be refactored to reuse.
////////////////////////////////////////////////////////////////////////////

const char *WordToString(unsigned int word)
{//========================================
// Convert a phoneme mnemonic word into a string
	int ix;
	static char buf[5];
	 
	for(ix=0; ix<3; ix++){
		buf[ix] = word >> (ix*8);
	}
	buf[4] = 0;
	return(buf);
}

static void Write4Bytes(FILE *f, int value)
{//=================================
// Write 4 bytes to a file, least significant first
	int ix;

	for(ix=0; ix<4; ix++)
	{
		fputc(value & 0xff,f);
		value = value >> 8;
	}
}

int OpenWavFile(char *path, int rate)
//===================================
{
	static unsigned char wave_hdr[44] = {
		'R','I','F','F',0x24,0xf0,0xff,0x7f,'W','A','V','E','f','m','t',' ',
		0x10,0,0,0,1,0,1,0,  9,0x3d,0,0,0x12,0x7a,0,0,
		2,0,0x10,0,'d','a','t','a',  0x00,0xf0,0xff,0x7f};

	if(path == NULL)
		return(2);

	if(path[0] == 0)
		return(0);

	if(strcmp(path,"stdout")==0)
		f_wavfile = stdout;
	else
		f_wavfile = fopen(path,"wb");

	if(f_wavfile == NULL)
	{
		fprintf(stderr,"Can't write to: '%s'\n",path);
		return(1);
	}


	fwrite(wave_hdr,1,24,f_wavfile);
	Write4Bytes(f_wavfile,rate);
	Write4Bytes(f_wavfile,rate * 2);
	fwrite(&wave_hdr[32],1,12,f_wavfile);
	return(0);
}   //  end of OpenWavFile



static void CloseWavFile()
//========================
{
	unsigned int pos;

	if((f_wavfile==NULL) || (f_wavfile == stdout))
		return;

	fflush(f_wavfile);
	pos = ftell(f_wavfile);

	fseek(f_wavfile,4,SEEK_SET);
	Write4Bytes(f_wavfile,pos - 8);

	fseek(f_wavfile,40,SEEK_SET);
	Write4Bytes(f_wavfile,pos - 44);

	fclose(f_wavfile);
	f_wavfile = NULL;

} // end of CloseWavFile
