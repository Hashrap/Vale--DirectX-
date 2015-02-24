#include "StdAfx.h"
#include "ValeMainMenu.h"


ValeMainMenu::ValeMainMenu(void)
{
}


ValeMainMenu::~ValeMainMenu(void)
{
}

void ValeMainMenu::initialize(ValeGraphics *g, Input *input)//, void (*readyToLoadLevel)(void))
{
	activeButton = -1;

	graphics = g;
	m_input = input;

	/*newGameBtn.setImage(graphics, L"..\\..\\Textures\\Menu\\button_sheet.tga", 2);
	creditsBtn.setImage(graphics, L"..\\..\\Textures\\Menu\\button_sheet.tga", 2);
	exitBtn.setImage(graphics, L"..\\..\\Textures\\Menu\\button_sheet.tga", 2);

	newGameBtn.setPosition(graphics->getWidth()/2 - newGameBtn.getImage()->getWidth()/2, graphics->getWidth()/2 - newGameBtn.getImage()->getWidth()/2 - 150.0f);
	creditsBtn.setPosition(graphics->getWidth()/2 - creditsBtn.getImage()->getWidth()/2, graphics->getWidth()/2 - creditsBtn.getImage()->getWidth()/2);
	exitBtn.setPosition(graphics->getWidth()/2 - exitBtn.getImage()->getWidth()/2, graphics->getWidth()/2 - exitBtn.getImage()->getWidth()/2 + 150.0f);

	newGameBtn.setImageInfo(true, 0, false, 0, 3, 0.25f/4);
	creditsBtn.setImageInfo(true, 0, false, 0, 3, 0.25f/4);
	exitBtn.setImageInfo(true, 0, false, 0, 3, 0.25f/4);*/


	creditsTexture.initialize(graphics, L"..\\..\\Textures\\Menu\\creditsScreen.tga");
	creditsImage.initialize(graphics, creditsTexture.getWidth(), creditsTexture.getHeight(), 1, &creditsTexture);
	creditsImage.setX(0.0f);
	creditsImage.setY(0.0f); 

	mainTexture.initialize(graphics, L"..\\..\\Textures\\Menu\\mainScreen.tga");
	mainImage.initialize(graphics, mainTexture.getWidth(), mainTexture.getHeight(), 1, &mainTexture);
	mainImage.setX(0.0f);
	mainImage.setY(0.0f);

	grabButtonText("ValeStrings.txt");
	LPCWSTR btnTexLoc = L"..\\..\\Textures\\Menu\\button_sheet.tga";
	ngButtonTexture.initialize(graphics, L"..\\..\\Textures\\Menu\\NGbutton_sheet.tga");
	cButtonTexture.initialize(graphics, L"..\\..\\Textures\\Menu\\Cbutton_sheet.tga");
	eButtonTexture.initialize(graphics, L"..\\..\\Textures\\Menu\\Ebutton_sheet.tga");
	newGameButton.initialize(graphics, 512, 128, 2, &ngButtonTexture);
	creditsButton.initialize(graphics, 512, 128, 2, &cButtonTexture);
	exitButton.initialize(graphics, 512, 128, 2, &eButtonTexture);

	newGameButton.setX(graphics->getWidth()/2 - newGameButton.getWidth()/2);
	newGameButton.setY(graphics->getWidth()/2 - newGameButton.getWidth()/2 - 150.0f);
	creditsButton.setX(graphics->getWidth()/2 - creditsButton.getWidth()/2);
	creditsButton.setY(graphics->getWidth()/2 - creditsButton.getWidth()/2);
	exitButton.setX(graphics->getWidth()/2 - exitButton.getWidth()/2);
	exitButton.setY(graphics->getWidth()/2 - exitButton.getWidth()/2 + 150.0f);

	newGameButton.setAnimationComplete(true);
	newGameButton.setCurrentFrame(0);
	newGameButton.setLoop(false);
	newGameButton.setFrames(0, 3);
	newGameButton.setFrameDelay(0.25f/4);

	creditsButton.setAnimationComplete(true);
	creditsButton.setCurrentFrame(0);
	creditsButton.setLoop(false);
	creditsButton.setFrames(0, 3);
	creditsButton.setFrameDelay(0.25f/4);

	exitButton.setAnimationComplete(true);
	exitButton.setCurrentFrame(0);
	exitButton.setLoop(false);
	exitButton.setFrames(0, 3);
	exitButton.setFrameDelay(0.25f/4);

	//CallLoadLevel = readyToLoadLevel;
	bShouldLoadLevel = false;
	bUpInputPressed = bDownInputPressed = bAtCreditsScreen = bInGame = false;
	inputDelay = 0.01f;
	timeSinceLastInput = 0.0f;
}

void ValeMainMenu::update(float frameTime)
{
	if(!bInGame)
	{
		/*timeSinceLastInput += frameTime;
		if(timeSinceLastInput > inputDelay)
		{*/
			checkInput();
			//timeSinceLastInput = 0.0f;
			//bUpInputPressed = bDownInputPressed = false;
		//}

		// needed to be able to commit
		/*newGameBtn.update(frameTime);
		creditsBtn.update(frameTime);
		exitBtn.update(frameTime);*/

		switch(activeButton)
		{
		case 0:
			newGameButton.update(frameTime);
			creditsButton.setCurrentFrame(0);
			exitButton.setCurrentFrame(0);
			break;
		case 1:
			newGameButton.setCurrentFrame(0);
			creditsButton.update(frameTime);
			exitButton.setCurrentFrame(0);
			break;
		case 2:
			newGameButton.setCurrentFrame(0);
			creditsButton.setCurrentFrame(0);
			exitButton.update(frameTime);
			break;
		}
	}
}

void ValeMainMenu::checkInput()
{
	if(!bInGame)
	{
		if(m_input->getMouseLButton())
		{
			int c_mX = m_input->getMouseX();
			int c_mY = m_input->getMouseY();

			if(c_mX > newGameButton.getX()+70 && c_mX < newGameButton.getX() + newGameButton.getWidth())
			{
				if(c_mY > newGameButton.getY() && c_mY < newGameButton.getY() + newGameButton.getHeight()/2)
				{
					bShouldLoadLevel = true;
					bInGame = true;
				}
			}
			if(c_mX > creditsButton.getX()+70 && c_mX < creditsButton.getX() + creditsButton.getWidth()/2)
			{
				if(c_mY > creditsButton.getY() && c_mY < creditsButton.getY() + creditsButton.getHeight()/2)
				{
					bAtCreditsScreen = true;
				}
			}
			if(c_mX > exitButton.getX()+70 && c_mX < exitButton.getX() + exitButton.getWidth()/2)
			{
				if(c_mY > exitButton.getY() && c_mY < exitButton.getY() + exitButton.getHeight()/2)
				{
					exit(0);
				}
			}

					/*newGameBtn.checkMouseClick(c_mX, c_mY);
		creditsBtn.checkMouseClick(c_mX, c_mY);
		exitBtn.checkMouseClick(c_mX, c_mY);*/
		}
	
		// spacebar or e or enter
		if(m_input->isKeyDown(0x20) || m_input->isKeyDown(0x45) || m_input->isKeyDown(0xD))
		{
			if(bAtCreditsScreen){
				bAtCreditsScreen = false;
			}
			//if(!bAtCreditsScreen)
			//{
			//	// select the active button
			//	switch(activeButton)
			//	{
			//	case 0:
			//		bShouldLoadLevel = true;
			//		bInGame = true;
			//		//CallLoadLevel();
			//		break;
			//	case 1:
			//		// show credits screen
			//		bAtCreditsScreen = true;
			//		break;
			//	case 2:
			//		//exit the application
			//		exit(0);
			//		break;
			//	}
			//}else
			//{
			//	bAtCreditsScreen = false;
			//}
		}

		//// up arrow or w
		//if(m_input->isKeyDown(0x26) || m_input->isKeyDown(0x57) && !bUpInputPressed)
		//{
		//	bUpInputPressed = true;
		//	activeButton--;
		//	if(activeButton < 0){
		//		activeButton = 0;
		//	}
		//}
		//// down arrow or s
		//if(m_input->isKeyDown(0x28) || m_input->isKeyDown(0x53) && !bDownInputPressed)
		//{
		//	bDownInputPressed = true;
		//	activeButton++;
		//	if(activeButton > 2){
		//		activeButton = 2;
		//	}
		//}
	}
}

void ValeMainMenu::draw()
{
	if(!bInGame)
	{
		if(bAtCreditsScreen)
		{
			creditsImage.draw();
		}
		else
		{
					/*newGameBtn.draw();
		creditsBtn.draw();
		exitBtn.draw();*/
			mainImage.draw();
			newGameButton.draw();
			creditsButton.draw();
			exitButton.draw();
		}
	}
}

void ValeMainMenu::grabButtonText(char* file)
{
	std::fstream fIn;
	fIn.open(file, std::ios_base::in);

	char temp[64];
	bool done = false;
	while(!done)
	{
		fIn >> temp;
		if(strcmp(temp, "MenuStrings") == 0)
		{
			fIn.getline(startGameButtonText, 64, ';');
			fIn.getline(creditsButtonText, 64, ';');
			fIn.getline(exitButtonText, 64, ';');
			done = true;
		}
	}

	/*newGameBtn.setButtonText(startGameButtonText);
	creditsBtn.setButtonText(creditsButtonText);
	exitBtn.setButtonText(exitButtonText);*/

	fIn.close();
}