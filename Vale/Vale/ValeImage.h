#pragma once

#include "ValeTexture.h"
#include "constants.h"

class ValeImage
{
protected:
	ValeGraphics* graphics;
	ValeTexture* texture;
	SpriteData spriteData;
	COLOR_ARGB colorFilter;

	int columns;
	int startFrame;
	int endFrame;
	int currentFrame;

	float frameDelay;
	float animTimer;

	bool bShouldLoop;
	bool bIsVisible;
	bool bIsInitialized;
	bool bIsAnimComplete;

	HRESULT result;

public:
	ValeImage(void);
	virtual ~ValeImage(void);

	// Getters
	const virtual SpriteData& getSpriteInfo(){return spriteData;}
	virtual bool getVisible() {return bIsVisible;}
	virtual float getX() {return spriteData.x;}
	virtual float getY() {return spriteData.y;}
	virtual float getScale() {return spriteData.scale;}
	virtual int getWidth() {return spriteData.width;}
	virtual int getHeight() {return spriteData.height;}
	virtual float getCenterX() {return spriteData.x + spriteData.width/2*getScale();}
	virtual float getCenterY() {return spriteData.y + spriteData.height/2*getScale();}
	virtual VECTOR2 getCenter() {return VECTOR2(getCenterX(), getCenterY());}
	virtual float getDegrees() {return spriteData.angle*(180.0f/(float)PI);}
	virtual float getRadians() {return spriteData.angle;}
	virtual float getFrameDelay() {return frameDelay;}
	virtual int getStartFrame() {return startFrame;}
	virtual int getEndFrame() {return endFrame;}
	virtual int getCurrentFrame() {return currentFrame;}
	virtual RECT getSpriteDataRect() {return spriteData.rect;}
	virtual bool getAnimationComplete() {return bIsAnimComplete;}
	virtual COLOR_ARGB getColorFilter() {return colorFilter;}

	// Setters
	virtual void setX(float newX) {spriteData.x = newX;}
	virtual void setY(float newY) {spriteData.y = newY;}
	virtual void setScale(float s) {spriteData.scale = s;}
	virtual void setDegrees(float deg) {spriteData.angle = deg*((float)PI/180.0f);}
	virtual void setRadians(float rad) {spriteData.angle = rad;}
	virtual void setVisible(bool v) {bIsVisible = v;}
	virtual void setFrameDelay(float d) {frameDelay = d;}
	virtual void setFrames(int start, int end) {startFrame = start; endFrame = end;}
	virtual void setSpriteDataRect(RECT r) {spriteData.rect = r;}
	virtual void setLoop(bool shouldLoop) {bShouldLoop = shouldLoop;}
	virtual void setAnimationComplete(bool animComplete) {bIsAnimComplete = animComplete;}
	virtual void setColorFilter(COLOR_ARGB color) {colorFilter = color;}
	virtual void setTexture(ValeTexture *tex) {texture = tex;}
	// non-inline setters
	virtual void setCurrentFrame(int frame);
	virtual void setRect();

	virtual bool initialize(ValeGraphics * g, int width, int height, int cols, ValeTexture *texture);
	virtual void flipHorizontal(bool flip) {spriteData.flipHorizontal = flip;}
	virtual void flipVertical(bool flip) {spriteData.flipVertical = flip;}
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);
	virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE);
	virtual void update(float frameTime);
};

