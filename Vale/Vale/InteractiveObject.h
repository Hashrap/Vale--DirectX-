#pragma once
#include "ValeTexture.h"
#include "ValeImage.h"
#include "ValeGraphics.h"
#include "ValeResources.h"
#include "ValeGame.h"
#include "math.h"

class ValeResources;

//Author:Alex George

//The superclass of everything that exists in the game world.
//the warden, enemies, csonumables, equipment, projectiles, destructibles will all inherit from this class
class InteractiveObject
{
protected:
	//properties
	ValeImage image;
	Input * input;
	Audio* audio;
	
	VECTOR2 position;	//position, in levelCoords
	VECTOR2 oldPosition;
	VECTOR2 cell;
	GridCell* cellPtr;
	VECTOR2 velocity;
	VECTOR2 deltaV;     //added to velocity during next call to update()
	float radius;		//radius of the bounding circle
	float rotation;
	std::string name;			//String of the name of the object. e.g. "Warden" or "Risen Warrior" or "The Rebounder"
	std::string description;	//Description of the object. e.g. "This item alters quick shot to fire multiple bullets" or "The Risen Warrior is a melee enemy unit. Don't get too close"	
	Faction faction; //Faction of the object. Only relevant for projectiles and living entities.
public:
	InteractiveObject(void);
	~InteractiveObject(void);	
	
	//General methods - override these in subclasses
	void draw();
	virtual void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM);	
	virtual void update(float frameTime);
	virtual void updateImagePosition(float x, float y);

	//Accessors
	VECTOR2 getPosition()	{ return position;}
	VECTOR2 getCenter()		{ return VECTOR2(position.x+(image.getWidth()/2), position.y+(image.getHeight()/2)); }
	VECTOR2 getOldCenter()	{ return VECTOR2(oldPosition.x+(image.getWidth()/2), oldPosition.y+(image.getHeight()/2)); }
	VECTOR2 getCell()		{ return cell; }
	GridCell* getCellPtr()	{ return cellPtr; }
	VECTOR2 getOldPosition(){ return oldPosition; }
	float getRotation()		{ return rotation; }
	VECTOR2 getVelocity()	{ return velocity; }
	float getRadius()		{ return radius; }
	ValeImage getImage()	{ return image; }
	std::string getName()			{ return name; }
	std::string getDescription()	{ return description; }
	Faction getFaction()	{ return faction; }

	//Mutators
	void setPosition(VECTOR2 newPosition) { position = newPosition; }
	void setPosition(float x, float y) { position = VECTOR2(x,y); }
	void setRotation(float newRotation) { rotation  = newRotation; }
	void setVelocity(VECTOR2 newVelocity) { velocity = newVelocity; }
	void setDeltaVelocity(VECTOR2 deltaVelocity) { deltaV = deltaVelocity; }
	void setCell(int x, int y)
	{ 
		cell.x = (float)x;
		cell.y = (float)y;
	}
	void setCellPtr(GridCell* c)
	{
		if(cellPtr != 0)
			cellPtr->remove(this);
		cellPtr = c;
		if(cellPtr != 0)
			c->add(this);
	}

	D3DXVECTOR2 normalize(D3DXVECTOR2 vectorToN);
	float distanceToTarget(D3DXVECTOR2 target);
	float distanceToTarget(InteractiveObject* obj);
};

