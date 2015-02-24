#include "StdAfx.h"
#include "Heart.h"

//Author: Alex George
Heart::Heart(void){}

Heart::~Heart(void){}

void Heart::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM)
{	
	name = "Heart";
	description = "Heals the player";
	consumed = false;
	faction = NEUTRAL;
	InteractiveObject::initialize(gamePtr,  width,  height,  ncols, textureM);
}

//Heal the warden
bool Heart::heal(Warden* warden)
{
	//if warden is full health or heart is consumed
	if(warden->isFullHealth() || consumed)
		return false;

	warden->heal(heartNS::HEALTH_RESTORED);	
	consumed = true;
	//audio->playCue(HEAL);
	return true;
}

void Heart::draw()
{
	if(!consumed)
	{
		InteractiveObject::draw();
	}
}
