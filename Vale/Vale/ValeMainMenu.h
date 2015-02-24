#pragma once

#include "Input.h"
#include "ValeGraphics.h"
#include "ValeImage.h"
#include "ValeTexture.h"
#include "ValeButton.h"
#include <fstream>

class ValeMainMenu
{
public:
	ValeMainMenu(void);
	~ValeMainMenu(void);

	void initialize(ValeGraphics *g, Input *input);//, void (*readyToLoadLevel)(void));
	void checkInput();
	void update(float frameTime);
	void draw();

	//void (*CallLoadLevel)(void);

	ValeImage newGameButton;
	ValeImage creditsButton;
	ValeImage exitButton;
	ValeImage creditsImage;
	ValeImage mainImage;
	
	ValeTexture ngButtonTexture;
	ValeTexture cButtonTexture;
	ValeTexture eButtonTexture;
	ValeTexture creditsTexture;
	ValeTexture mainTexture;

	char startGameButtonText[40];
	char creditsButtonText[40];
	char exitButtonText[40];

	bool bShouldLoadLevel;
	
private:

	/*ValeButton newGameBtn;
	ValeButton creditsBtn;
	ValeButton exitBtn;*/

	void grabButtonText(char* file);

	ValeGraphics *graphics;
	Input *m_input;

	int activeButton;

	bool bUpInputPressed, bDownInputPressed, bAtCreditsScreen, bInGame;
	float inputDelay, timeSinceLastInput;
};

