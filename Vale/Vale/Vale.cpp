#include "StdAfx.h"
#include "Vale.h"
#include "ValeRectangle.h"

Vale::Vale(void)
{
}

Vale::~Vale(void)
{
	releaseAll();
}



void Vale::initialize(HWND hwnd)
{
	ValeGame::initialize(hwnd);

	bLevelLoaded = false;
	bLoadingLevel = false;
	mainMenu.initialize(graphics, input);

	//Initialize Level
	//loadLevel();
	

	return;
}

void Vale::update(float frameTime)
{
	//Check all input - keys and mouse
	checkInput();

	//Update level
	if(bLevelLoaded)
	{
		currLevel.update(frameTime);
	}
	
	mainMenu.update(frameTime);
	if(mainMenu.bShouldLoadLevel && !bLevelLoaded && !bLoadingLevel)
	{
		loadLevel();
		bLoadingLevel = true;
	}
}

void Vale::loadLevel()
{
	currLevel.loadMap("map5.vmp", this);
	currLevel.setGroundImages(graphics);
	currLevel.initialize(this);
	bLevelLoaded = true;
}

void Vale::checkInput()
{
	//Check game state. If not !paused
	if(bLevelLoaded)
	{
		currLevel.checkInput();
	}
}

void Vale::render()
{
	graphics->spriteBegin();

	mainMenu.draw();
	
	if(bLevelLoaded)
	{
		currLevel.draw();

		// find what tile the mouse is over currently
		int mx, my;
		currLevel.grabMapCoord(&mx, &my, (float)input->getMouseX(), (float)input->getMouseY());
		// show the map coord of whatever tile we have the mouse over
		strcpy(mapCoordXStr,"Map Coord X:");
		itoa(mx, mapCoordXStr+12,10);
		//graphics->DrawTextString(10,70,mapCoordXStr);
		strcpy(mapCoordYStr,"Map Coord Y:");
		itoa(my, mapCoordYStr+12,10);
		//graphics->DrawTextString(10,100,mapCoordYStr);

	}
	strcpy(fpsStr, "FPS: ");
	itoa(static_cast<int>(fps), fpsStr+5, 10);
	//graphics->DrawTextString(10, 10, fpsStr);
	strcpy(mouseXStr,"Mouse X:");
	itoa(static_cast<int>(input->getMouseX()),mouseXStr+8,10);
	//graphics->DrawTextString(10,40,mouseXStr);
	strcpy(mouseYStr,"Mouse Y:");
	itoa(static_cast<int>(input->getMouseY()),mouseYStr+8,10);
	//graphics->DrawTextString(200,40,mouseYStr);

	graphics->spriteEnd();
}

void Vale::releaseAll()
{
	ValeGame::releaseAll();
	return;
}

void Vale::resetAll()
{
	ValeGame::resetAll();
	return;
}
