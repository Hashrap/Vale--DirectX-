#include "StdAfx.h"
#include "RisenArcher.h"

//Author:Bradley Lubahn
RisenArcher::RisenArcher(void)
{
	baseMovementSpeed =  risenArcherNS::DEF_MOVEMENT;
	additionalMovementSpeed = 0;	
	
	currHealth = risenArcherNS::MAX_HEALTH;
	maxHealth =  risenArcherNS::MAX_HEALTH;
	fleeHealth =  risenArcherNS::FLEE_HEALTH;
	
	shielded = false;

	//squared distance to avoid sqrt later
	seekDistance =  risenArcherNS::SEEK_DISTANCE;
	attackDistance =  risenArcherNS::ATTACK_DISTANCE;
	rangedAttackDistance = risenArcherNS::RANGEDATT_DISTANCE;
	//set beginning state to do nothing
	currentState = IDLE;
	previousState = IDLE;
	//give the risen a type, this is used to avoid casting errors in main loop
	type = ARCHER;

	velocity = D3DXVECTOR2(0,0);
	position = D3DXVECTOR2(0,0);
}


RisenArcher::~RisenArcher(void)
{

}
//set up speedBuff, melee and ranged attacks and call Risen init
void RisenArcher::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,ValeTexture *projText, Warden *p)
{
									
	Risen::initialize(gamePtr, width, height, ncols, textureM,projText);

	speedBuff.initialize(this,risenArcherNS::BUFF_COOLDOWN,risenArcherNS::BUFF_CAST_TIME,risenArcherNS::STATTYPE,risenArcherNS::BUFF_DURATION,
		risenArcherNS::BUFF_MULTIPLE,risenArcherNS::BUFF_ADD);

	attackAbi.initialize(this,risenArcherNS::MELEE_COOLDOWN,risenArcherNS::MELEE_CTIME,risenArcherNS::MELEE_DMG,
		risenArcherNS::MELEE_RANGE,risenArcherNS::MELEE_ARC);

	rangedAbi.initialize(this,risenArcherNS::RANGE_COOLDOWN,risenArcherNS::RANGE_CTIME,risenArcherNS::RANGE_DMG,
		risenArcherNS::RANGE_RANGE,risenArcherNS::RANGE_SPEED,risenArcherNS::RANGE_RADIUS,false, false);
	name = "Risen Archer";
	//give risen a reference to the player for seeking/path finding behaviors
	player = p;
}
void RisenArcher::update(float frameTime,std::vector<Risen*>* rVec)
{
	
	desTarget.x=0;desTarget.y=0;
	speedBuff.update(frameTime);
	attackAbi.update(frameTime);
	rangedAbi.update(frameTime);

	determineBehavior();
	
	switch(currentState)
	{
		case MOVE:
			//possible patrolling.
			//patrol();
			//std::cout << "move" << std::endl;
				break;
		case SEEK:
			//move toward the player if they are in sight, and close enough
			 desTarget = seek(player->getCenter());
			 desTarget += separate(10000,*rVec)*0.3;
			 facePlayer();
			//std::cout << "seek" << std::endl;
		case RANGE_ATTACK:
			if(rangedAbi.activate())
			{
				audio->playCue(QS3);
			}
				break;
		case FLEE:
			//run if low on health
			velocity.x=0;velocity.y=0;
			//std::cout << "flee" << std::endl;
				break;
		case ATTACK:
			//smack the player if close enough
			attackAbi.activate(player);
			velocity.x=0;velocity.y=0;
			desTarget = flee(player->getCenter());
			desTarget += separate(10000,*rVec)*0.3;
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
	//desTarget is essentially a force vector which will steer/point the risen in the desired direction 
	//if it is moving based off of its current state.Velocity, and position are handled in living entity
	velocity = desTarget;

	Risen::update(frameTime);
	facePlayer();
}
//function for determining the risens behaviors, uses the distance to the player to determine what to do next
//Simple functionality for now, if time allows this will be modified so the AI is more complex
void RisenArcher::determineBehavior()
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
	if(dtw < attackDistance)
		currentState= ATTACK;

}