#include "StdAfx.h"
#include "bar.h"

bool Bar::initialize(ValeGraphics *graphics, ValeTexture *textureM, int w, int h, int left, int top, float scale, COLOR_ARGB color)
{
    try {
        ValeImage::initialize(graphics, w, h, 1, textureM);
        //setCurrentFrame(dashboardNS::BAR_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
    }
    catch(...)
    {
        return false;
    }

    return true;
}

void Bar::setPosition(int left, int top)
{
	spriteData.x = (float)left;
    spriteData.y = (float)top;
}

//=============================================================================
// set the Bar size
//=============================================================================
void Bar::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    spriteData.rect.right = spriteData.rect.left + (LONG)(spriteData.width*p/100);
}


