// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 constants.h v1.0

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }

//-----------------------------------------------
//                  Constants
//-----------------------------------------------
// window
//const char CLASS_NAME[] = "Spacewar";
//const char GAME_TITLE[] = "Game Engine Part 1";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1280;              // width of game in pixels
const UINT GAME_HEIGHT = 720;               // height of game in pixels
 
// game
const double PI = 3.14159265;
const float FRAME_RATE  = 200.0f;               // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const LPCWSTR WAVE_BANK  = L"audio\\Win\\Wave Bank.xwb";
// SOUND_BANK must be location of .xsb file.
const LPCWSTR SOUND_BANK = L"audio\\Win\\Sound Bank.xsb";

//audio cues
// audio cues
const char STRONG[]		= "stronghit";
const char STRONG2[]    = "stronghit2";
const char PREMATURE[]  = "premature";
const char DAMAGE[]     = "damage";
const char FLEETFEET[]  = "fleetfeet";

const char MELEE1[] = "melee1";
const char MELEE2[] = "melee2";
const char MELEE3[] = "melee3";

const char MEDIUM1[] = "medium1";
const char MEDIUM2[] = "medium2";

const char QS1[] = "qs1";
const char QS2[] = "qs2";
const char QS3[] = "qs3";