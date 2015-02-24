#include "StdAfx.h"
#include "Equipment.h"

//Equipment. Parent of Bows.
//Author: Alex George
Equipment::Equipment(){}

Equipment::~Equipment(){}

void Equipment::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM)
{
	faction = NEUTRAL;
	equipped = false;
	InteractiveObject::initialize(gamePtr,  width,  height,  ncols, textureM);
}

void Equipment::draw()
{
	//Override draw so that we only draw it on the ground if it's not equipped
	if(!equipped)
		InteractiveObject::draw();
}

void Equipment::equip()
{
	equipped = true;
}

void Equipment::unequip()
{
	equipped = false;
}