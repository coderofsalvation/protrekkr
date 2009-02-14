// ------------------------------------------------------
// Protrekkr
// Written by Franck Charlet
// Based on the work of Juan Antonio Arguelles Rius 
// ------------------------------------------------------

#ifndef _REPLAY_H_
#define _REPLAY_H_

// ------------------------------------------------------
// Includes
#include "tb_303.h"
#include "cubic_spline.h"
#include "../sounddriver/include/sounddriver.h"
#include "samples_unpack.h"
#include "ptkreplay.h"
#include "synth.h"

// ------------------------------------------------------
// Constants
#define MAX_TRACKS 16
#define MAX_POLYPHONY 16
#define DEFAULT_POLYPHONY 5
#define MAX_FILTER 23
#define MAX_COMB_FILTERS 10
#define PI 3.1415926535897932384626433832795
#define SMP_PACK_GSM 0
#define SMP_PACK_MP3 1
#define SMP_PACK_TRUESPEECH 2
#define SMP_PACK_NONE 3
#define SMP_PACK_AT3 4
#define PATTERN_MAX_ROWS 128
#define PATTERN_BYTES 6
#define PATTERN_ROW_LEN MAX_TRACKS * PATTERN_BYTES
#define PATTERN_TRACK_LEN PATTERN_MAX_ROWS * PATTERN_BYTES
#define PATTERN_LEN PATTERN_ROW_LEN * PATTERN_MAX_ROWS
#define PATTERN_NBR PATTERN_LEN * 128
#define SAMPLESPERSEC 44100

#define FLANGER_LOPASS_CUTOFF 0.1f
#define FLANGER_LOPASS_RESONANCE 0.4f

#define SMP_LOOP_NONE 0
#define SMP_LOOP_FORWARD 1
#define SMP_LOOP_PINGPONG 2

#define SMP_LOOPING_FORWARD 0
#define SMP_LOOPING_BACKWARD 1

#define SYNTH_WAVE_OFF 0
#define SYNTH_WAVE_CURRENT 1

#define PLAYING_NOSAMPLE 0
#define PLAYING_SAMPLE 1
#define PLAYING_SAMPLE_NOTEOFF 2
#define PLAYING_STOCK 3

// ------------------------------------------------------
// Structures

// SAMPLE COUNTER
struct smpos
{
#if defined(__BIG_ENDIAN__)
    Uint32 first;
    Uint32 last;
#else
    Uint32 last;
    Uint32 first;
#endif
};

union s_access
{
    smpos half;
    int64 absolu;
};

// ------------------------------------------------------
// Variables
extern float decays[MAX_COMB_FILTERS][2];
extern int delays[MAX_COMB_FILTERS];       // delays for the comb filters
extern int counters[MAX_COMB_FILTERS];
extern char num_echoes;
extern unsigned char *RawPatterns;
extern int cPosition;
extern unsigned int lchorus_counter;
extern unsigned int rchorus_counter;
extern unsigned int lchorus_counter2;
extern unsigned int rchorus_counter2;
extern int lchorus_delay;
extern int rchorus_delay;
extern float mas_comp_threshold;
extern float mas_comp_ratio;
extern unsigned char nPatterns;
extern char Songtracks;
extern unsigned char sLength;
extern unsigned char pSequence[256];
extern short patternLines[PATTERN_MAX_ROWS];
extern char nameins[128][20];
extern char Midiprg[128];
extern unsigned char Synthprg[128];
extern char SampleCompression[128];
extern char SampleType[128][16];
extern char SampleName[128][16][64];
extern char Basenote[128][16];
extern Uint32 LoopStart[128][16];
extern Uint32 LoopEnd[128][16];
extern char LoopType[128][16];
extern Uint32 SampleNumSamples[128][16];
extern char Finetune[128][16];
extern float SampleVol[128][16];
extern float FDecay[128][16];
extern short *RawSamples[128][2][16];
extern char SampleChannels[128][16];
extern float TCut[MAX_TRACKS];
extern float ICut[MAX_TRACKS];
extern float TPan[MAX_TRACKS];
extern int FType[MAX_TRACKS];
extern int FRez[MAX_TRACKS];
extern float DThreshold[MAX_TRACKS];
extern float DClamp[MAX_TRACKS];
extern float DSend[MAX_TRACKS]; 
extern int CSend[MAX_TRACKS];
extern char Channels_Polyphony[MAX_TRACKS];
extern char compressor; // 0-->Off 1-->On
extern int c_threshold;
extern int BeatsPerMin;
extern int TicksPerBeat;
extern float mas_vol;

extern int delay_time;
extern float Feedback;
extern int DelayType;
extern float lchorus_feedback;
extern float rchorus_feedback;
extern int shuffle;

extern char CHAN_ACTIVE_STATE[256][16];
extern char CHAN_HISTORY_STATE[256][16];
extern float CCoef[MAX_TRACKS];
extern int CHAN_MIDI_PRG[MAX_TRACKS];

extern char LFO_ON[MAX_TRACKS];
extern float LFO_RATE[MAX_TRACKS];
extern float LFO_AMPL[MAX_TRACKS];

extern char FLANGER_ON[MAX_TRACKS];
extern float FLANGER_AMOUNT[MAX_TRACKS];
extern float FLANGER_DEPHASE[MAX_TRACKS];
extern float FLANGER_RATE[MAX_TRACKS];
extern float FLANGER_AMPL[MAX_TRACKS];
extern float FLANGER_GR[MAX_TRACKS];
extern float FLANGER_FEEDBACK[MAX_TRACKS];
extern int FLANGER_DELAY[MAX_TRACKS];
extern int FLANGER_OFFSET[MAX_TRACKS];

extern float foff2[MAX_TRACKS];
extern float foff1[MAX_TRACKS];

extern int CHAN_MUTE_STATE[MAX_TRACKS]; // 0->Normal 1->Muted
extern char Disclap[MAX_TRACKS];

extern char artist[20];
extern char style[20];

extern char beatsync[128];
extern short beatlines[128];

extern float REVERBFILTER;

extern float CustomVol[128];

#if !defined(__STAND_ALONE__)
extern unsigned int SubCounter;
extern int PosInTick;
extern int plx;
extern int Reserved_Sub_Channels[MAX_TRACKS][MAX_POLYPHONY];
extern int sp_Stage[MAX_TRACKS][MAX_POLYPHONY];
extern int sp_Stage2[MAX_TRACKS][MAX_POLYPHONY];
extern int sp_Stage3[MAX_TRACKS][MAX_POLYPHONY];
extern int L_MaxLevel;
extern int R_MaxLevel;
extern CSynth Synthesizer[MAX_TRACKS][MAX_POLYPHONY];
extern float Player_FD[MAX_TRACKS];
extern char sp_channelsample[MAX_TRACKS][MAX_POLYPHONY];
extern char sp_split[MAX_TRACKS][MAX_POLYPHONY];
#if defined(__PSP__)
extern volatile int Songplaying;
#else
extern int Songplaying;
#endif
extern int left_value;
extern int right_value;
extern SynthParameters PARASynth[128];
extern float ramper[MAX_TRACKS];
extern unsigned char nPatterns;
extern int delay_time;
extern int DelayType;
#endif

// ------------------------------------------------------
// Functions
void Pre_Song_Init(void);
void Sp_Player(void);
void Play_Instrument(int channel, int sub_channel,
                     float note, int sample,
                     float vol, unsigned int offset,
                     int glide, int Play_Selection, int midi_channel);
void ResetFilters(int tr);
void ComputeStereo(int channel);
void GetPlayerValues(float master_coef);
void noteoff303(char strack);
void init_sample_bank(void);
void KillInst(int inst_nbr);
void Post_Song_Init(void);
void ResetSynthParameters(SynthParameters *TSP);
void Free_Samples(void);
void Mas_Compressor_Set_Variables(float treshold, float ratio);
int Get_Free_Sub_Channel(int channel);

#endif
