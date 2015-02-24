// Author: Bradley Lubahn, Spencer Corkran

#include "StdAfx.h"
#include "RisenWarlock.h"


RisenWarlock::RisenWarlock(void)
{
	baseMovementSpeed = risenWarlockNS::DEF_MOVEMENT;
	maxHealth = risenWarlockNS::MAX_HEALTH;
	currHealth = maxHealth;
	fleeHealth = risenWarlockNS::FLEE_HEALTH;

	shielded = false;

	seekDistance =  risenWarlockNS::SEEK_DISTANCE;
	attackDistance =  risenWarlockNS::ATTACK_DISTANCE;
	rangedAttackDistance = risenWarlockNS::RANGEDATT_DISTANCE;
	fleeDistance = risenWarlockNS::FLEE_DISTANCE;

	currentState = IDLE;
	previousState = IDLE;

	velocity = D3DXVECTOR2(0,0);
	position = D3DXVECTOR2(0,0);

	type = WARLOCK;
}


RisenWarlock::~RisenWarlock(void)
{
}

void RisenWarlock::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,ValeTexture *projText, Warden *p)
{
	Risen::initialize(gamePtr, width, height, ncols, textureM,projText);

	spray.initialize(this, risenWarlockNS::SPRAY_COOLDOWN, risenWarlockNS::SPRAY_CTIME, risenWarlockNS::SPRAY_DMG, risenWarlockNS::SPRAY_RANGE, risenWarlockNS::SPRAY_SPEED, risenWarlockNS::SPRAY_RADIUS, false, false);
	primary.initialize(this, risenWarlockNS::RANGE_COOLDOWN, risenWarlockNS::RANGE_CTIME, risenWarlockNS::RANGE_DMG, risenWarlockNS::RANGE_RANGE, risenWarlockNS::RANGE_SPEED, risenWarlockNS::RANGE_RADIUS, false, false);
	absorb.initialize(this, risenWarlockNS::ABSORB_COOLDOWN, risenWarlockNS::ABSORB_CTIME, risenWarlockNS::ABSORB_STATTYPE, risenWarlockNS::ABSORB_DURATION, risenWarlockNS::ABSORB_MULTIPLIER, risenWarlockNS::ABSORB_ADDITIVE);

	name = "Risen Warlock";
	player = p;
}

void RisenWarlock::update(float frameTime)
{
	desTarget.x=0;desTarget.y=0;

	primary.update(frameTime);
	spray.update(frameTime);
	absorb.update(frameTime);

	determineBehavior();
	
	switch(currentState)
	{
		case MOVE:
				break;
		case SEEK:
			//move toward the player if they are in sight, and close enough
			 desTarget = seek(player->getPosition());
			 facePlayer();
		case RANGE_ATTACK:
			velocity.x=0;velocity.y=0;
			if(spray.activate())
				audio->playCue(DAMAGE);
				break;
		case FLEE:
			//run if low on health
			desTarget = flee(player->getCenter());
				break;
		case CAST:
			velocity.x=0;velocity.y=0;
				break;
		case IDLE:
			//do nothing
			velocity.x=0;velocity.y=0;
				break;
		default:
			std::cout<<"nothing"<< std::endl;
	}
	//desTarget is essentially a force vector which will steer/point the risen in the desired direction 
	//if it is moving based off of its current state.Velocity, and position are handled in living entity
	velocity = desTarget;

	Risen::update(frameTime);
	facePlayer();
}

//function for determining the risens behaviors, uses the distance to the player to determine what to do next
//Simple functionality for now, if time allows this will be modified so the AI is more complex
void RisenWarlock::determineBehavior()
{
	previousState = currentState;
	
	float dtw = distanceToTarget(player->getPosition());
	
	if(dtw > seekDistance)
		currentState = IDLE;
	if(dtw < seekDistance)
		currentState = SEEK;
	//seek the player if they are with seeking distance but out of ranged attacking distance
	if(dtw < seekDistance && dtw < rangedAttackDistance)
		currentState = RANGE_ATTACK;
	if(dtw < rangedAttackDistance){}
		//currentState = CAST;
	if(dtw < fleeDistance || (currHealth < fleeHealth && dtw < seekDistance))
		currentState = FLEE;

}