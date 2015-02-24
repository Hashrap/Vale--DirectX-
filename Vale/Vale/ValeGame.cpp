#include "StdAfx.h"
#include "ValeGame.h"


ValeGame::ValeGame(void)
{
	paused = false;
	graphics = NULL;
	input = new Input();        // initialize keyboard input immediately
	initialized = false;
	fps = 0.0f;
}


ValeGame::~ValeGame(void)
{
	deleteAll();
}

LRESULT ValeGame::handleMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);        //tell Windows to kill this program
			return 0;
        case WM_KEYDOWN: case WM_SYSKEYDOWN:    // key down
            input->keyDown(wParam);
            return 0;
        case WM_KEYUP: case WM_SYSKEYUP:        // key up
            input->keyUp(wParam);
            return 0;
        case WM_CHAR:                           // character entered
            input->keyIn(wParam);
            return 0;
        case WM_MOUSEMOVE:                      // mouse moved
            input->mouseIn(lParam);
            return 0;
        case WM_INPUT:                          // raw mouse data in
            input->mouseRawIn(lParam);
            return 0;
		case WM_LBUTTONDOWN:                    // left mouse button down
                input->setMouseLButton(true);
                input->mouseIn(lParam);             // mouse position
                return 0;
        case WM_LBUTTONUP:                      // left mouse button up
            input->setMouseLButton(false);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_MBUTTONDOWN:                    // middle mouse button down
            input->setMouseMButton(true);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_MBUTTONUP:                      // middle mouse button up
            input->setMouseMButton(false);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_RBUTTONDOWN:                    // right mouse button down
            input->setMouseRButton(true);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_RBUTTONUP:                      // right mouse button up
            input->setMouseRButton(false);
            input->mouseIn(lParam);             // mouse position
            return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ValeGame::initialize(HWND hwnd)
{
	hWnd = hwnd;

	graphics = new ValeGraphics();
	graphics->initialize(hWnd, GAME_WIDTH, GAME_HEIGHT, false);

	// input
	input->initialize(hWnd, false);

	// init sound system
    audio = new Audio();
    if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')  // if sound files defined
    {
        if( FAILED( result = audio->initialize() ) )
        {
            if( result == HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) )
                throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system: media file not found."));
            else
                throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system."));
        }
    }


	// timer
	if(QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initilizing high resolution timer."));
	QueryPerformanceCounter(&timeStart);

	

	initialized = true;
}

//=============================================================================
// Toggle window or fullscreen mode
//=============================================================================
void ValeGame::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
    releaseAll();                   // free all user created surfaces
    graphics->changeDisplayMode(mode);
    resetAll();                     // recreate surfaces
}

void ValeGame::run(HWND hwnd)
{
	if(graphics == NULL)
		return;

	QueryPerformanceCounter(&timeEnd);
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timerFreq.QuadPart);

	if(frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime)*1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return;
	}

	if(frameTime > 0.0)
		fps = (fps*0.99f) + (0.01f/frameTime);

	if(frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;

	timeStart = timeEnd;

	if(!paused)
		update(frameTime);

	renderGame();
	
	setDisplayMode(graphicsNS::WINDOW);
	input->clear(inputNS::KEYS_PRESSED);
}

void ValeGame::handleLostGraphicsDevice()
{
	result = graphics->getDeviceState();
	if(FAILED(result))
	{
		if(result == D3DERR_DEVICELOST)
		{
			Sleep(100);
			return;
		}
		else if(result == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			result = graphics->reset();
			if(FAILED(result))
				return;
			resetAll();
		}
		else
			return;
	}
}

void ValeGame::renderGame()
{
	if(SUCCEEDED(graphics->beginScene()))
	{
		render();		
		graphics->endScene();
	}
	handleLostGraphicsDevice();

	graphics->showBackBuffer();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void ValeGame::releaseAll() {}

//=============================================================================
// Recreate all surfaces and reset all entities.
//=============================================================================
void ValeGame::resetAll() {}

//=============================================================================
// Delete all reserved memory
//=============================================================================
void ValeGame::deleteAll()
{
    releaseAll();               // call onLostDevice() for every graphics item
	SAFE_DELETE(graphics);
	SAFE_DELETE(audio);
	SAFE_DELETE(input);
    initialized = false;
}