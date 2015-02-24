#pragma once

//class Level;

#include <Windows.h>
#include <MMSystem.h>
#include <DxErr.h>
#include "ValeGraphics.h"
#include "Input.h"
#include "audio.h"
#include "gameError.h"
#include "constants.h"


class ValeGame
{
protected: // Data members
	HWND hWnd;

	
	ValeGraphics*	graphics;
	Input*			input;
	Audio*			audio;
	HRESULT			result;

	LARGE_INTEGER	timeStart;
	LARGE_INTEGER	timeEnd;
	LARGE_INTEGER	timerFreq;
	float			frameTime;
	float			fps;
	DWORD			sleepTime;

	bool			paused;
	bool			initialized;
	char			fpsStr[20];
	char			mouseXStr[20];
	char			mouseYStr[20];
	char			mapCoordXStr[30];
	char			mapCoordYStr[30];


public: // Member functions
	ValeGame(void);
	virtual ~ValeGame(void);

	LRESULT handleMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
	virtual void initialize(HWND hwnd);
	virtual void run(HWND);
	virtual void releaseAll();
	virtual void resetAll();
	virtual void deleteAll();
	virtual void renderGame();
	virtual void handleLostGraphicsDevice();

	// Set display mode (fullscreen, window or toggle)
    void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	ValeGraphics* getGraphics() {return graphics;}
	Input* getInput()	{return input;}
	void exitGame() {PostMessage(hWnd, WM_DESTROY, 0, 0);}

    Audio* getAudio()       {return audio;}

	// Pure Virtuals
	virtual void update(float frameTime) = 0;
	virtual void render() = 0;
};

