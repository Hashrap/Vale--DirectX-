#include "StdAfx.h"
#include "RisenWarrior.h"
#include "Warden.h"
#include "Vale.h"
#include <stdio.h>
#include <math.h>

//Author:Bradley Lubahn
RisenWarrior::RisenWarrior(void):Risen()
{
	baseMovementSpeed = risenWarriorNS::DEF_MOVEMENT;
	additionalMovementSpeed = 0;	
	
	currHealth = risenWarriorNS::MAX_HEALTH;
	maxHealth = risenWarriorNS::MAX_HEALTH;
	fleeHealth = risenWarriorNS::FLEE_HEALTH;
	
	shielded = false;

	//squared distance to avoid sqrt later
	seekDistance = risenWarriorNS::SEEK_DISTANCE;
	attackDistance = risenWarriorNS::ATTACK_DISTANCE;

	//set beginning state to do nothing
	currentState = IDLE;
	previousState = IDLE;
	//give the risen a type, this is used to avoid casting errors in main loop
	type = WAR;

	velocity = D3DXVECTOR2(0,0);
	position = D3DXVECTOR2(0,0);

}

RisenWarrior::~RisenWarrior(void){}

void RisenWarrior::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,ValeTexture *projText,Warden *p,std::vector<int> tileRef)
{
									
	Risen::initialize(gamePtr, width, height, ncols, textureM,projText);

	speedBuff.initialize(this,risenWarriorNS::BUFF_COOLDOWN,risenWarriorNS::BUFF_CAST_TIME,risenWarriorNS::STATTYPE,risenWarriorNS::BUFF_DURATION,
		risenWarriorNS::BUFF_MULTIPLE,risenWarriorNS::BUFF_ADD);

	attackAbi.initialize(this,risenWarriorNS::ATTACK_COOLDOWN,risenWarriorNS::ATTACK_CTIME,risenWarriorNS::ATTACK_DMG,
		risenWarriorNS::ATTACK_RANGE,risenWarriorNS::ATTACK_ARC);
	name = "Risen Warrior";
	//give risen a reference to the player for seeking/path finding behaviors
	player = p;
	tiles= tileRef;
}

//Runs through the warriors update, gets a new state and performs actions based on its new state
void RisenWarrior::update(float frameTime,std::vector<Risen*>* rVec)
{
	desTarget.x=0;desTarget.y=0;
	speedBuff.update(frameTime);
	attackAbi.update(frameTime);
	
	determineBehavior();
	//std::cout << "Update!!!!" << std::endl;
	switch(currentState)
	{
		case MOVE:
			//possible patrolling.
			//patrol();
			//std::cout << "move" << std::endl;
				break;
		case SEEK:
			//move toward the player
			 desTarget = seek(player->getCenter());
			 desTarget += separate(10000,*rVec)*0.3f;
			//std::cout << "seek" << std::endl;
				break;
		case FLEE:
			//run if low on health
			velocity.x=0;velocity.y=0;
			 desTarget = flee(player->getCenter());
			//std::cout << "flee" << std::endl;
			break;
		case ATTACK:
			//smack the player if close enough
			if(attackAbi.activate(player))
			{
				audio->playCue(MELEE1);
			}
			desTarget += separate(10000,*rVec)*0.3f;
			velocity.x=0;velocity.y=0;
			//std::cout << "attack" << std::endl;
				break;
		case IDLE:
			//do nothing
			//std::cout << "Idle" << std::endl;
			velocity.x=0;velocity.y=0;
				break;
		default:
			std::cout<<"nothing"<< std::endl;
	}
	velocity = desTarget;
	
	Risen::update(frameTime);
}
//uses the distance function to make some very basic choices as to what to do next
void RisenWarrior::determineBehavior()
{
	previousState = currentState;
	
	float dtw = distanceToTarget(player->getPosition());

	if(dtw > seekDistance)
		currentState = IDLE;
	if(dtw < seekDistance)
		currentState = SEEK;
	if(dtw < attackDistance)
		currentState= ATTACK;
	if(dtw < seekDistance * 1.5 && currHealth < fleeHealth)
		currentState = FLEE;
	//std::cout << currentState << std::endl;
}