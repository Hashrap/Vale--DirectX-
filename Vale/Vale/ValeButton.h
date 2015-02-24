#pragma once

#include "ValeGraphics.h"
#include "ValeTexture.h"
#include "ValeImage.h"

class ValeButton
{
public:
	ValeButton(void);
	~ValeButton(void);

	bool checkMouseClick(float x, float y);
	void update(float frameTime);
	void draw();
	
	void setButtonText(char* text);
	void setImageInfo(bool bIsAnimComplete, int currentFrame, bool bShouldLoop, int startFrame, int endFrame, float frameDelay);
	void toggleActive(){bIsActive = !bIsActive;}
	void setActive(){bIsActive = true;}
	void setInactive(){bIsActive = false;}
	void setPosition(float x, float y){m_position.x = x; m_position.y = y;}
	void setPosition(D3DXVECTOR2 position){m_position = position;}
	void setImage(ValeGraphics *graphics, LPCWSTR file, int numCols){
		m_texture.initialize(graphics, file);
		m_image.initialize(graphics, m_texture.getWidth(), m_texture.getHeight(), numCols, &m_texture);
	}
	void setImage(ValeGraphics *graphics, ValeTexture tex, int numCols){
		m_texture = tex;
		m_image.initialize(graphics, m_texture.getWidth(), m_texture.getHeight(), numCols, &m_texture);
	}
	void setImage(ValeImage image){
		m_image = image;
	}

	ValeImage* getImage(){return &m_image;}

private:
	ValeTexture m_texture;
	ValeImage m_image;
	char m_szButtonText[40];
	D3DXVECTOR2 m_position;
	bool bIsActive;

	bool bPositionInitialized;
};

