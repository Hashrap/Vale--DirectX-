#include "StdAfx.h"
#include "ValeImage.h"

ValeImage::ValeImage(void)
{
	bIsInitialized = false;            // set true when successfully initialized
    spriteData.width = 2;
    spriteData.height = 2;
    spriteData.x = 0.0;
    spriteData.y = 0.0;
    spriteData.scale = 1.0;
    spriteData.angle = 0.0;
    spriteData.rect.left = 0;       // used to select one frame from multi-frame image
    spriteData.rect.top = 0;
    spriteData.rect.right = spriteData.width;
    spriteData.rect.bottom = spriteData.height;
    spriteData.texture = NULL;      // the sprite texture (picture)
    spriteData.flipHorizontal = false;
    spriteData.flipVertical = false;
    columns = 1;
    texture = NULL;
    startFrame = 0;
    endFrame = 0;
    currentFrame = 0;
    frameDelay = 1.0;               // default to 1 second per frame of animation
    animTimer = 0.0;
	bIsVisible = true;                 // the image is visible
    bShouldLoop = true;                    // loop frames
    bIsAnimComplete = false;
    graphics = NULL;                // link to graphics system
    colorFilter = graphicsNS::WHITE; // WHITE for no change
}


ValeImage::~ValeImage(void)
{
}

bool ValeImage::initialize(ValeGraphics *g, int width, int height, int cols, ValeTexture *tex)
{
	try
	{
		graphics = g;
		texture = tex;

		spriteData.texture = texture->getTexture();
		if(width == 0)
			width = texture->getWidth();
		spriteData.width = width;
		if(height == 0)
            height = texture->getHeight();   // use full height of texture
        spriteData.height = height;
		columns = cols;
        if (columns == 0)
            columns = 1;

		spriteData.rect.left = (currentFrame % columns) * spriteData.width;
		// right edge + 1
        spriteData.rect.right = spriteData.rect.left + spriteData.width;
        spriteData.rect.top = (currentFrame / columns) * spriteData.height;
        // bottom edge + 1
        spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch(...)
	{
		return false;
	}
	bIsInitialized = true;
	return true;
}

void ValeImage::draw(COLOR_ARGB color)
{
	if(!bIsVisible || graphics == NULL)
		return;
	spriteData.texture = texture->getTexture();
	if(color == graphicsNS::FILTER)
		graphics->drawSprite(spriteData, colorFilter);
	else
		graphics->drawSprite(spriteData, color);
}

void ValeImage::draw(SpriteData sd, COLOR_ARGB color)
{
	if(!bIsVisible || graphics == NULL)
		return;
	sd.rect = spriteData.rect;
	sd.texture = texture->getTexture();

	if(color == graphicsNS::FILTER)
		graphics->drawSprite(sd, colorFilter);
	else
		graphics->drawSprite(sd, color);
}

void ValeImage::update(float frameTime)
{
	if(endFrame - startFrame > 0)
	{
		animTimer += frameTime;
		if(animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if(currentFrame < startFrame || currentFrame > endFrame)
			{
				if(bShouldLoop)
					currentFrame = startFrame;
				else
				{
					currentFrame = endFrame;
					bIsAnimComplete = true;
				}
			}
			setRect();
		}
	}
}

void ValeImage::setCurrentFrame(int frame)
{
	if(frame >= 0)
	{
		currentFrame = frame;
		bIsAnimComplete = false;
		setRect();
	}
}

inline void ValeImage::setRect()
{
	spriteData.rect.left = (currentFrame % columns) * spriteData.width;
	spriteData.rect.right = spriteData.rect.left + spriteData.width;

	spriteData.rect.top = (currentFrame / columns) * spriteData.height;
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}