#pragma once
#include "valegame.h"
#include "Warden.h"
#include "RisenWarrior.h"
#include "ValeTexture.h"
#include "ValeImage.h"
#include "Level.h"
#include "ValeMainMenu.h"
#include <vector>
#include "Risen.h"

class Vale :
	public ValeGame
{
private:	
	Level			currLevel;
	ValeMainMenu	mainMenu;
	bool			bLevelLoaded;
	bool			bLoadingLevel;

public:
	//Constructor
	Vale(void);
	virtual ~Vale(void);
	void initialize(HWND hwnd);

	//Member Functions
	void loadLevel();
	void checkInput();
	void update(float frameTime);
	void ai();
	void collisions();
	void render();
	void releaseAll();
	void resetAll();	
};

