#include "StdAfx.h"
#include "Warden.h"

//Author:Alex George
Warden::Warden(void):LivingEntity()
{
	baseMovementSpeed = wardenNS::DEF_MOVEMENT;
	additionalMovementSpeed = 0;
	position = VECTOR2(0,0);
	
	currHealth = wardenNS::MAX_HEALTH;
	maxHealth = wardenNS::MAX_HEALTH;

	name = wardenNS::NAME;
	description = wardenNS::DESC;
}

Warden::~Warden(void){}

void Warden::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, ValeTexture *qsTex, ValeTexture *fsTex)
{
	LivingEntity::initialize(gamePtr, width, height, ncols, textureM, qsTex, PLAYER);
	// Override radius - We want the Warden's hitbox to be a bit smaller than normal
	radius = 12.0f;

	//death sound
	deathSound = "playerdeath";

	//set faction
	faction = PLAYER;

	//Set textures for projectiles
	quickShotTex = qsTex;
	focusShotTex = fsTex;

	//initialize abilities

	//M1 abilities - quick shot
	quickShot.initialize(this, bowNS::QS_COOLDOWN, bowNS::QS_CASTTIME, bowNS::GB_QS_DAMAGE, bowNS::GB_QS_RANGE, bowNS::GB_QS_PROJSPEED, bowNS::QS_PROJSIZE, bowNS::GB_QS_PIERCING,bowNS::GB_QS_RICOCHET);	
	splitShot.initialize(this, bowNS::QS_COOLDOWN, bowNS::QS_CASTTIME, bowNS::SS_QS_DAMAGE, bowNS::SS_QS_RANGE, bowNS::SS_QS_PROJSPEED, bowNS::QS_PROJSIZE, bowNS::SS_QS_PIERCING,bowNS::SS_QS_RICOCHET, bowNS::SS_QS_ARC, bowNS::SS_QS_PROJNUM);
	reboundShot.initialize(this, bowNS::QS_COOLDOWN, bowNS::QS_CASTTIME, bowNS::RB_QS_DAMAGE, bowNS::RB_QS_RANGE, bowNS::RB_QS_PROJSPEED, bowNS::QS_PROJSIZE, bowNS::RB_QS_PIERCING,bowNS::RB_QS_RICOCHET);	
	
	//m2 abilities - focus shot
	focusShot.initialize(this, bowNS::GB_FS_PROJSIZE,  bowNS::GB_FS_RICOCHET,  bowNS::GB_FS_MIN_SWEET_BOUNDARY, bowNS::GB_FS_MAX_SWEET_BOUNDARY,  
		bowNS::GB_FS_MIN_SPEED, bowNS::GB_FS_SWEET_SPEED, bowNS::GB_FS_MAX_SPEED, bowNS::GB_FS_MIN_DAMAGE, bowNS::GB_FS_SWEET_DAMAGE, bowNS::GB_FS_MAX_DAMAGE);

	splitFocusShot.initialize(this, bowNS::SS_FS_PROJSIZE,  bowNS::SS_FS_RICOCHET,  bowNS::SS_FS_MIN_SWEET_BOUNDARY, bowNS::SS_FS_MAX_SWEET_BOUNDARY,  
		bowNS::SS_FS_MIN_SPEED, bowNS::SS_FS_SWEET_SPEED, bowNS::SS_FS_MAX_SPEED, bowNS::SS_FS_MIN_DAMAGE, bowNS::SS_FS_SWEET_DAMAGE, bowNS::SS_FS_MAX_DAMAGE);

	reboundFocusShot.initialize(this, bowNS::RB_FS_PROJSIZE,  bowNS::RB_FS_RICOCHET,  bowNS::RB_FS_MIN_SWEET_BOUNDARY, bowNS::RB_FS_MAX_SWEET_BOUNDARY,  
		bowNS::RB_FS_MIN_SPEED, bowNS::RB_FS_SWEET_SPEED, bowNS::RB_FS_MAX_SPEED, bowNS::RB_FS_MIN_DAMAGE, bowNS::RB_FS_SWEET_DAMAGE, bowNS::RB_FS_MAX_DAMAGE);

	museFocusShot.initialize(this, bowNS::MU_FS_PROJSIZE,  bowNS::MU_FS_RICOCHET,  bowNS::MU_FS_MIN_SWEET_BOUNDARY, bowNS::MU_FS_MAX_SWEET_BOUNDARY,  
		bowNS::MU_FS_MIN_SPEED, bowNS::MU_FS_SWEET_SPEED, bowNS::MU_FS_MAX_SPEED, bowNS::MU_FS_MIN_DAMAGE, bowNS::MU_FS_SWEET_DAMAGE, bowNS::MU_FS_MAX_DAMAGE);

	//fleet feet
	fleetFeet.initialize(this, wardenNS::FF_COOLDOWN, wardenNS::FF_CASTTIME, wardenNS::FF_STATTYPE, wardenNS::FF_DURATION, wardenNS::FF_MULTIPLIER, wardenNS::FF_ADDITIVE);

	//initialize bow
	currentBow = GUARDIAN;

	//Unlock the guardian bow - lock the others.
	isUnlocked[0] = true;
	for(int i = 1; i < wardenNS::NUM_BOWS; i++)
		isUnlocked[i] = false;
}

//Determine the direction of the player
void Warden::setDirectionVector(bool up, bool down, bool left, bool right)
{
	directionVector.x = 0;
	directionVector.y = 0;

	if(up)
		directionVector.y--;
	if(down)
		directionVector.y++;
	if(left)
		directionVector.x--;
	if(right)
		directionVector.x++;

	return;
}

void Warden::update(float frameTime)
{
	//Rotate towards mouse	

	float deltaX = input->getMouseX() - image.getCenterX();
	float deltaY = input->getMouseY() - image.getCenterY();		
	rotation = (float)(atan2(deltaY, deltaX) * 180.0 / PI);
	image.setDegrees(rotation);

	//update abilities
	quickShot.update(frameTime);
	splitShot.update(frameTime);
	reboundShot.update(frameTime);

	focusShot.update(frameTime);
	splitFocusShot.update(frameTime);
	reboundFocusShot.update(frameTime);
	museFocusShot.update(frameTime);

	fleetFeet.update(frameTime);	

	//Update velocity based on input
	calculateVelocity();
	LivingEntity::update(frameTime);
}

//Heal the warden
void Warden::heal(int healthRestored)
{
	currHealth += healthRestored;
	if(currHealth>maxHealth)
		currHealth = maxHealth;
}

//Begin focusing for Focus Shot
bool Warden::startFocus()
{
	switch(currentBow)
	{
	case GUARDIAN:
		return focusShot.beginChanneling();
	case SPLIT:
		return splitFocusShot.beginChanneling();
	case REBOUNDER:
		return reboundFocusShot.beginChanneling();
	default:
		return museFocusShot.beginChanneling();
	}
}

//End focus shot
void Warden::endFocus()
{	
	switch(currentBow)
	{
	case GUARDIAN:
		if(focusShot.endChanneling())	//sweet
			audio->playCue(MEDIUM1);
		else //not sweet
			audio->playCue(PREMATURE);	

		//update cooldowns on other focus shot abilities
		splitFocusShot.setTimeRemaining(focusShot.getTimeRemaining());
		reboundFocusShot.setTimeRemaining(focusShot.getTimeRemaining());
		museFocusShot.setTimeRemaining(focusShot.getTimeRemaining());

		break;
	case SPLIT:
		if(splitFocusShot.endChanneling())	//sweet
			audio->playCue(MEDIUM2);
		else //not sweet
			audio->playCue(PREMATURE);

		//update cooldowns on other focus shot abilities
		focusShot.setTimeRemaining(splitFocusShot.getTimeRemaining());
		reboundFocusShot.setTimeRemaining(splitFocusShot.getTimeRemaining());
		museFocusShot.setTimeRemaining(splitFocusShot.getTimeRemaining());

		break;
	case REBOUNDER:
		if(reboundFocusShot.endChanneling())	//sweet
			audio->playCue(MEDIUM2);
		else //not sweet
			audio->playCue(PREMATURE);

		//update cooldowns on other focus shot abilities
		splitFocusShot.setTimeRemaining(reboundFocusShot.getTimeRemaining());
		focusShot.setTimeRemaining(reboundFocusShot.getTimeRemaining());
		museFocusShot.setTimeRemaining(reboundFocusShot.getTimeRemaining());

		break;
	default:
		if(museFocusShot.endChanneling()) //sweet, cut cooldown
		{
			museFocusShot.museBonus();
			audio->playCue(STRONG2);
		} //not sweet
		else
			audio->playCue(PREMATURE);

		//update cooldowns on other focus shot abilities
		splitFocusShot.setTimeRemaining(museFocusShot.getTimeRemaining());
		reboundFocusShot.setTimeRemaining(museFocusShot.getTimeRemaining());
		focusShot.setTimeRemaining(museFocusShot.getTimeRemaining());
	}
}

//player pressed m1. determine quick shot to use based on equipped bow
bool Warden::useQuickShot()
{
	bool success = false;
	if(currentBow == GUARDIAN) // 1
	{
		//use ability
		success =  quickShot.activate();

		//reset cooldown of other abilities
		splitShot.setTimeRemaining(quickShot.getTimeRemaining());
		reboundShot.setTimeRemaining(quickShot.getTimeRemaining());
	}
	else if(currentBow == SPLIT) // 2
	{
		success = splitShot.activate();

		//reset cooldown of other abilities
		quickShot.setTimeRemaining(splitShot.getTimeRemaining());
		reboundShot.setTimeRemaining(splitShot.getTimeRemaining());
	}
	else if(currentBow == REBOUNDER) // 3
	{
		success = reboundShot.activate();

		//reset cooldown of other abilities
		quickShot.setTimeRemaining(reboundShot.getTimeRemaining());
		splitShot.setTimeRemaining(reboundShot.getTimeRemaining());
	}
	else //muse - 4
	{
		//need to check if this lands. then reduce cooldown on focus shot
		success =  quickShot.activate();

		//reset cooldown of other abilities
		splitShot.setTimeRemaining(quickShot.getTimeRemaining());
		reboundShot.setTimeRemaining(quickShot.getTimeRemaining());	
	}

	//if shot, play sound
	if(success)
	{
		int randomSound = rand() % 3;
		if(randomSound == 0)
			audio->playCue(QS1);
		else if (randomSound == 1)
			audio->playCue(QS2);
		else
			audio->playCue(QS3);
	}

	return success;
}

bool Warden::useFleetFeet()
{
	bool success = fleetFeet.activate();

	if(success)
		audio->playCue(FLEETFEET);
	return success;	
}

//Switches the warden's bow, changing her abilities
bool Warden::switchBow(BowType bowType)
{
	if(isUnlocked[bowType])
		currentBow = bowType;
	
	return isUnlocked[bowType];
}

//Permanently unlocks a bow for future use.
void Warden::unlockBow(BowType bowType)
{
	isUnlocked[bowType] = true;
}

//Update velocity based on input
void Warden::calculateVelocity()
{
	//If up/down is pressed
	if(directionVector.y != 0)
	{
		//If up/down and left/right is pressed
		if(directionVector.x !=0)
		{
			velocity.x = actualMovementSpeed() * directionVector.x * SQRTHALF ;
			velocity.y = actualMovementSpeed() * directionVector.y * SQRTHALF ;
		}
		//If ONLY up/down is pressed
		else
		{
			velocity.y = actualMovementSpeed() * directionVector.y;
			velocity.x = 0;
		}
	}
	//If up/down is NOT pressed
	else
	{
		velocity.y  = 0;
		//If ONLY left/right is pressed
		if(directionVector.x !=0)
			velocity.x = actualMovementSpeed() * directionVector.x;
		//If nothing is pressed
		else
			velocity.x = 0;
	}
}

void Warden::updateImagePosition(int x, int y)
{
	image.setX((float)x);
	image.setY((float)y);	
}

void Warden::takeDamage()
{
	LivingEntity::takeDamage(1);
}

void Warden::respawnAtPosition(float x, float y)
{
	setPosition(VECTOR2(x,y));
	alive = true;
	currHealth = wardenNS::MAX_HEALTH;
}