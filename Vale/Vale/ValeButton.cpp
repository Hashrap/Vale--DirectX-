#include "StdAfx.h"
#include "ValeButton.h"


ValeButton::ValeButton(void)
{
	bPositionInitialized = false;
}


ValeButton::~ValeButton(void)
{
}

void ValeButton::setButtonText(char *text)
{
	int length = 0;
	while(text[length] != 0)
	{
		length++;
	}

	for(int i = 0; i < length; i++)
	{
		m_szButtonText[i] = text[i];
	}
}

void ValeButton::setImageInfo(bool bIsAnimComplete, int currentFrame, bool bShouldLoop, int startFrame, int endFrame, float frameDelay)
{
	m_image.setAnimationComplete(bIsAnimComplete);
	m_image.setCurrentFrame(currentFrame);
	m_image.setLoop(bShouldLoop);
	m_image.setFrames(startFrame, endFrame);
	m_image.setFrameDelay(frameDelay);
}

bool ValeButton::checkMouseClick(float x, float y)
{
	if(x > m_position.x && x < m_image.getWidth())
	{
		if(y > m_position.y && y < m_image.getHeight())
		{
			return true;
		}
	}
	return false;
}

void ValeButton::update(float frameTime)
{
	if(bIsActive){
		m_image.update(frameTime);
	}

	if(!bPositionInitialized)
	{
		m_image.setX(m_position.x);
		m_image.setY(m_position.y);
		bPositionInitialized = true;
	}
}

void ValeButton::draw()
{
	m_image.draw();
}