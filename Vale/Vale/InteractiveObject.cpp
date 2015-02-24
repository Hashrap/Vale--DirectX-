#include "StdAfx.h"
#include "InteractiveObject.h"

//Author:Alex George
InteractiveObject::InteractiveObject(void)
{
	//Set defaults
	position = VECTOR2(0,0);
	oldPosition = VECTOR2(0,0);
	velocity = VECTOR2(0,0);
	deltaV = VECTOR2(0,0);
	cell = VECTOR2(-1,-1);
	cellPtr = 0;
	radius = 0.0f;
	rotation = 0.0f;
}

InteractiveObject::~InteractiveObject(void){}

//Initialize the object. Called after creation.
void InteractiveObject::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM)
{
	//Set input and audio
	input = gamePtr->getInput();                
    audio = gamePtr->getAudio();               

    image.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	if(width = height)
	{
		radius = width/2;
	}
}

//Draw's the object's image
void InteractiveObject::draw()	{ image.draw(); }

//Update function. Updates velocity and position.
void InteractiveObject::update(float frameTime)
{
	//update velocity and position
	velocity += deltaV;
	oldPosition = position;
	position += velocity * frameTime;

	//reset deltaV
    deltaV.x = 0;
    deltaV.y = 0;
	
    image.update(frameTime);
}

//Sets the object's image's position to the correct screen coordinates using the given level coordinates
void InteractiveObject::updateImagePosition(float x, float y)
{
	image.setX(position.x-x);
	image.setY(position.y-y);
}

//Simple normalization function that returns a unit vector
D3DXVECTOR2 InteractiveObject::normalize(D3DXVECTOR2 vectorToN)
{
	float mag = sqrt(vectorToN.x * vectorToN.x + vectorToN.y * vectorToN.y);
	if(mag == 0)
	{
		return D3DXVECTOR2();
	}
	else
	{
		vectorToN.x = vectorToN.x / mag;
		vectorToN.y = vectorToN.y / mag;
		return vectorToN;
	}
}
//simple distance check used by other functions(doesn't use sqrt, so distances are also squared when making checks)
float InteractiveObject::distanceToTarget(D3DXVECTOR2 target)
{
	if(target)
	{
		float dtw = ((position.x - target.x) *(position.x - target.x)+
				(position.y - target.y) *(position.y - target.y));
		return dtw;
	}
		return 0;
}

//simple distance check used by other functions(doesn't use sqrt, so distances are also squared when making checks)
// Overload: Interactive object
float InteractiveObject::distanceToTarget(InteractiveObject* obj)
{
		VECTOR2 dist = getCenter() - obj->getCenter();
		dist.x = dist.x*dist.x;
		dist.y = dist.y*dist.y;
		float dist2 = dist.x+dist.y;
		return dist2;
}