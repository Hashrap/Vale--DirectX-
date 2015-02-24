#include "StdAfx.h"
#include "ValeTexture.h"


ValeTexture::ValeTexture(void)
{
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;
}


ValeTexture::~ValeTexture(void)
{
	SAFE_RELEASE(texture);
}

bool ValeTexture::initialize(ValeGraphics* g, LPCWSTR f)
{
	try
	{
		graphics = g;
		file = f;

		transparencyColor = SETCOLOR_ARGB(0, 255, 0, 255);

		result = graphics->loadTexture(file, transparencyColor, width, height, texture);
		if(FAILED(result))
		{
			SAFE_RELEASE(texture);
			return false;
		}
	}
	catch(...){return false;}

	initialized = true;
	return true;
}

void ValeTexture::setTransparencyColor(DWORD color)
{
	transparencyColor = color;
	graphics->loadTexture(file, transparencyColor, width, height, texture);
}

void ValeTexture::onLostDevice()
{
	if(!initialized)
		return;
	SAFE_RELEASE(texture);
}

void ValeTexture::onResetDevice()
{
	if(!initialized)
		return;
	graphics->loadTexture(file, transparencyColor, width, height, texture);
}