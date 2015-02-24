#pragma once
#include "livingentity.h"
#include "FocusShot.h"
#include "SplitShotAbility.h"
#include "SelfBuffAbility.h"
#include "Bow.h"
#include <math.h>

//constant stats of warden
namespace wardenNS
{
	//general
	const int SIZE = 32;
	const float DEF_MOVEMENT = 400;
	const int MAX_HEALTH =  4;
	const std::string NAME = "The Warden";
	const std::string DESC = "The Warden is the sole defender of Vale.";	
	const int NUM_BOWS = 4;

	//fleet FEEEEET
	const float FF_COOLDOWN = 9;
	const float FF_CASTTIME = .1f;
	const StatType FF_STATTYPE = MOVEMENTSPEED;
	const float FF_DURATION = 1.5;
	const float FF_MULTIPLIER = 1.5;
	const float FF_ADDITIVE = 0;
}

//Author:Alex George
class Warden :
	public LivingEntity
{	
private:	
	//attributes
	D3DXVECTOR2	directionVector;	
	
	//abilities

	//m1 abilites - quickshot
	RangedAttack quickShot; // 1
	SplitShotAbility splitShot; // 2
	RangedAttack reboundShot; //3

	//m2 abilities - focus shot
	FocusShot focusShot; // 1
	FocusShot splitFocusShot; // 2
	FocusShot reboundFocusShot; //3
	FocusShot museFocusShot; //4

	//space ability - fleetfeet 
	SelfBuffAbility fleetFeet;
		

	//textures
	ValeTexture *quickShotTex, *focusShotTex;

	//equipment
	bool isUnlocked[wardenNS::NUM_BOWS];
	BowType currentBow;

	//Functions
	void calculateVelocity();

public:
	//Constructors
	Warden(void);
	~Warden(void);

	//Member functions
	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, ValeTexture *quickShot, ValeTexture *focusShot);	
	void update(float frameTime);
	void updateImagePosition(int x, int y);
	void setDirectionVector(bool up, bool down, bool left, bool right);
	void heal(int healthRestored);
	void takeDamage();
	void respawnAtPosition(float x, float y);

	//Functions for ability usage
	bool useQuickShot();
	bool useFleetFeet();
	bool startFocus();
	void endFocus();

	float getFocusChannelPercentage()
	{ 
		switch(currentBow)
		{
		case GUARDIAN:
			return focusShot.getChannelPercent();
		case SPLIT:
			return splitFocusShot.getChannelPercent(); 
		case REBOUNDER:
			return reboundFocusShot.getChannelPercent(); 
		case MUSE:
			return museFocusShot.getChannelPercent(); 
		}
		return 0.0f;
	}
	
	//Item usage
	bool switchBow(BowType bowType);	
	void unlockBow(BowType bowType);

	//Accessors
	bool isFocusing() 
	{ 
		switch(currentBow)
		{
		case GUARDIAN:
			return focusShot.isChanneling(); 
		case SPLIT:
			return splitFocusShot.isChanneling(); 
		case REBOUNDER:
			return reboundFocusShot.isChanneling(); 
		case MUSE:
			return museFocusShot.isChanneling(); 
		}
		return false;	
	}

	D3DXVECTOR2 getDirection() { return directionVector; }
	BowType getBow() { return currentBow; }

	float getQuickShotCooldown() { return quickShot.getTimeRemaining(); }	
	float getFocusShotCooldown() { return focusShot.getTimeRemaining(); }
	float getFleetFeetCooldown() { return fleetFeet.getTimeRemaining(); }

	float actualMovementSpeed()
	{
		if(isFocusing())
			return (baseMovementSpeed + additionalMovementSpeed)/2;
		return baseMovementSpeed + additionalMovementSpeed;
	}
	//Mutators
};

