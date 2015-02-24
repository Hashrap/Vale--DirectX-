#pragma once

#include "ValeGraphics.h"
#include "constants.h"

class ValeTexture
{
private:
	UINT			width;
	UINT			height;
	LP_TEXTURE		texture;
	LPCWSTR			file;
	DWORD			transparencyColor;
	ValeGraphics*	graphics;
	bool			initialized;
	HRESULT			result;

public:
	ValeTexture(void);
	virtual ~ValeTexture(void);

	LP_TEXTURE getTexture() const {return texture;}
	UINT getWidth() const {return width;}
	UINT getHeight() const {return height;}
	void setTransparencyColor(DWORD color);

	virtual bool initialize(ValeGraphics* g, LPCWSTR file);
	virtual void onLostDevice();
	virtual void onResetDevice();
};

