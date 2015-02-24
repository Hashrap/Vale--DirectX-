// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.h v1.0
#include "constants.h"
#include <xact3.h>


class Audio
{
    // properties
  private:
    IXACT3Engine* xactEngine;   // pointer to XACT sound engine
    IXACT3WaveBank* waveBank;   // pointer to XACT wave bank
    IXACT3SoundBank* soundBank; // pointer to XACT sound bank
    XACTINDEX cueI;             // XACT sound index
    void* mapWaveBank;          // call UnmapViewOfFile() to release file
    void* soundBankData;
    bool coInitialized;         // set true if coInitialize is successful

  public:
    // Constructor
    Audio();

    // Destructor
    virtual ~Audio();

    // member functions

    // Initialize Audio
    HRESULT initialize();

    // Perform periodic sound engine tasks.
    void run();

    // Play sound specified by cue from sound bank.
    // If cue does not exist no error occurs, there is simply no sound played.
    void playCue(const char cue[]);

    // Stop a playing sound specified by cue from sound bank.
    // If cue does not exist no error occurs.
    void stopCue(const char cue[]);
};
