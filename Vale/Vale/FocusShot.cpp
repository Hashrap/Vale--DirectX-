#include "StdAfx.h"
#include "FocusShot.h"

//The Warden's M2 ability
//Author:Alex George
FocusShot::FocusShot(void){}

FocusShot::~FocusShot(void){}

void FocusShot::initialize(LivingEntity *own,  int projRadius, bool ricochets, float minSweet, 
						   float maxSweet, float minSp, float sweetSp, float maxSp, float minDmg, float sweetDmg, float maxDmg)
{
	//static properties
	maxChannelTime = bowNS::FS_MAX_CHANNEL_TIME/2;
	channelTime = 0;
	channeling = false;	

	//speed
	minSpeed = minSp;
	sweetSpeed = sweetSp;
	maxSpeed = maxSp;

	minDamage = minDmg;
	sweetDamage = sweetDmg;
	maxDamage = maxDmg;

	minSweetBoundary = minSweet/2;
	maxSweetBoundary = maxSweet/2;

	RangedAttack::initialize(own, bowNS::FS_COOLDOWN, bowNS::FS_CASTTIME, sweetDamage, bowNS::FS_RANGE, sweetSpeed, projRadius, true, ricochets);
}

bool FocusShot::activate()
{
	//if out of bullets or on cooldown
	if(owner->getPoolAmt() == 0 || !isReady()) { return false; }
	else
	{
		//Determine strength and speed of bullet based on channel time.
		if(channelTime < minSweetBoundary) //If it's a premature shot
		{
			//damage and speed begins at a minimum and scales up depending on channelTime
			
			//formula: min + (.8sweet - min)(channelTime/minSweetBoundary)
			damage = minDamage + (.8f * sweetDamage - minDamage)*(channelTime/minSweetBoundary);
			projectileSpeed = minSpeed + (.8f * sweetSpeed - minSpeed)*(channelTime/minSweetBoundary);
			piercing = true;
		}
		else if(channelTime <= maxSweetBoundary) //sweet spot!
		{
			//damage and speed are static in the sweet spot boundary
			damage = sweetDamage;
			projectileSpeed = sweetSpeed;
			piercing = true;
		}
		else // we got ourselves a misfire.  channelTime > maxSweet. higher damage and speed, no piercing
		{
			//formula = 1.1sweet + (max - 1.1sweet)(channelTime/maxChannelTime)
			damage = (1.1f * sweetDamage) + ( maxDamage - (1.1f * sweetDamage))*(channelTime/maxChannelTime);
			projectileSpeed = (1.1f * sweetSpeed) + ( maxSpeed - (1.1f * sweetSpeed))*(channelTime/maxChannelTime);
			piercing = false;
		}

		//fire
		channelTime = 0;
		Ability::activate();
		return owner->fire(projectileSpeed, damage, projectileRadius, piercing, ricochet, owner->getRotation());
	}
}

//begin channeling the focus shot
bool FocusShot::beginChanneling()
{	
	channeling = isReady();
	return channeling;	
}

//end channeling the focus shot
bool FocusShot::endChanneling()
{
	if(channeling)
	{
		channeling = false;
		bool sweet = isSweet();
		activate();
		return sweet;
	}
	return false;
}

void FocusShot::update(float frameTime)
{
	//if channeling, keep track of channelTime
	if(channeling)
	{		
		channelTime+=frameTime;
		if(channelTime >= maxChannelTime)
			endChanneling();
	}

	//update cooldown
	Ability::update(frameTime);
}

//return whether the current channel was a sweet shot.
bool FocusShot::isSweet()
{
	return (channelTime > minSweetBoundary && channelTime <= maxSweetBoundary);
}

//muse was equipped during a sweetspot. cut the cooldown in half!
void FocusShot::museBonus(){ timeRemaining/=2; }