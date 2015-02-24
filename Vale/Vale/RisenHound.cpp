#include "StdAfx.h"
#include "RisenHound.h"


RisenHound::RisenHound(void)
{
	baseMovementSpeed = risenHoundNS::DEF_MOVEMENT;
	additionalMovementSpeed = 0;	
	
	currHealth = risenHoundNS::MAX_HEALTH;
	maxHealth = risenHoundNS::MAX_HEALTH;
	fleeHealth = risenHoundNS::FLEE_HEALTH;
	
	shielded = false;

	//squared distance to avoid sqrt later
	seekDistance = risenHoundNS::SEEK_DISTANCE;
	attackDistance = risenHoundNS::ATTACK_DISTANCE;
	chargeDistance = risenHoundNS::CHARGE_DISTANCE;
	//set beginning state to do nothing
	currentState = IDLE;
	previousState = IDLE;
	//give the risen a type, this is used to avoid casting errors in main loop
	type = HOUND;

	velocity = D3DXVECTOR2(0,0);
	position = D3DXVECTOR2(0,0);
}


RisenHound::~RisenHound(void)
{
}

void RisenHound::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,ValeTexture *projText,Warden *p)
{
									
	Risen::initialize(gamePtr, width, height, ncols, textureM,projText);

	speedBuff.initialize(this,risenHoundNS::BUFF_COOLDOWN,risenHoundNS::BUFF_CAST_TIME,risenHoundNS::STATTYPE,risenHoundNS::BUFF_DURATION,
		risenHoundNS::BUFF_MULTIPLE,risenHoundNS::BUFF_ADD);
	//Normal Melee attack
	attackAbi.initialize(this,risenHoundNS::ATTACK_COOLDOWN,risenHoundNS::ATTACK_CTIME,risenHoundNS::ATTACK_DMG,
		risenHoundNS::ATTACK_RANGE,risenHoundNS::ATTACK_ARC);
	//Strong melee attack for if it successfully charges the player
	chargeAbi.initialize(this,risenHoundNS::CHARGE_COOLDOWN,risenHoundNS::CHARGE_CTIME,risenHoundNS::CHARGE_DMG,
		risenHoundNS::CHARGE_RANGE,risenHoundNS::CHARGE_ARC);
	
	
	name = "Risen Hound";
	//give risen a reference to the player for seeking/path finding behaviors
	player = p;
	//booleans for charge attack
	charging = false;
	attacking = false;
}
void RisenHound::update(float frameTime,std::vector<Risen*>* rVec)
{
	desTarget.x=0;desTarget.y=0;
	speedBuff.update(frameTime);
	attackAbi.update(frameTime);
	chargeAbi.update(frameTime);

	determineBehavior();
	//Checks the current state of the AI, and acts accordingly
	switch(currentState)
	{
		case MOVE:
			//possible patrolling.
			//patrol();
				break;
		case SEEK:
			//move toward the player if they are in sight, and close enough
			 desTarget = seek(player->getCenter());
			 desTarget += separate(10000,*rVec)*0.3;
			//std::cout << "seek" << std::endl;
				break;
		case FLEE:
			//run if low on health
			velocity.x=0;velocity.y=0;
			 desTarget = flee(player->getCenter());
			//std::cout << "flee" << std::endl;
				break;
		case CHARGE_ATTACK:
			//starts a "timer" for a charge attack, after the timer goes off it uses 
			//its charge attack and speed buff ability to move toward the player at 5 times its normal speed
			//start the actual timer
			if(!charging)
			{
				charging = true;
				chargeTimer = risenHoundNS::CHARGE_TIME;
				velocity.x=0;velocity.y=0;
			}
			if(charging)
			{
				chargeTimer -= frameTime;
				velocity.x=0;velocity.y=0;
			}
			//if we are charging and the timer has completed start attacking
			if(charging && chargeTimer < 0 && !attacking)
			{
				desTarget = seek(player->getCenter());
				if(speedBuff.activate())
					audio->playCue(FLEETFEET);
			}
			if(attacking)
			{
				desTarget = seek(player->getCenter());
				
				if(chargeAbi.activate(player))
				{
					audio->playCue(MELEE2);
				}

				charging = false;
				chargeTimer = 0;
				std::cout << "Charge Attack" << std::endl;
				attacking=false;
				for(int i =0, count = buffs.size();i < count; i++)
				{
					removeBuff(buffs[i]);
				}
			}
			break;
		case ATTACK:
			//smack the player if close enough
			attackAbi.activate(player);
			velocity.x=0;velocity.y=0;
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
	velocity = desTarget;
	
	Risen::update(frameTime);
}
//uses the distance function to make some very basic choices as to what to do next
void RisenHound::determineBehavior()
{
	previousState = currentState;
	
	float dtw = distanceToTarget(player->getPosition());
	
	if(!charging)
	{
		if(dtw > seekDistance)
			currentState = IDLE;
		if(dtw < seekDistance)
			currentState = SEEK;
		if(dtw < chargeDistance && chargeAbi.isReady())
			currentState = CHARGE_ATTACK;
		if(dtw < attackDistance && !chargeAbi.isReady() && previousState != CHARGE_ATTACK)
			currentState = ATTACK;
		if(dtw < seekDistance && currHealth < fleeHealth)
			currentState = FLEE;
	}
	//if we are actually close enough to the warden to attack, set attacking to true,this will reset everything else back in the main update
	else if(dtw < attackDistance && chargeAbi.isReady())
	{
		currentState = CHARGE_ATTACK;
		attacking = true;
	}
}