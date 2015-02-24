#include "StdAfx.h"
#include "Bow.h"

//Represents an equippable bow
//Author: Alex George
Bow::Bow(void){}

Bow::~Bow(void){}

void Bow::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, BowType type)
{
	bowType = type;
	Equipment::initialize(gamePtr,  width,  height,  ncols, textureM);
}