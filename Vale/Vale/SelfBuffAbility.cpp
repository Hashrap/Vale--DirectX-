#include "StdAfx.h"
#include "SelfBuffAbility.h"

//This provides a short duration stat buff (Movement speed, damage)
//Author:Alex George
SelfBuffAbility::SelfBuffAbility(void){}

SelfBuffAbility::~SelfBuffAbility(void){}

void SelfBuffAbility::initialize(LivingEntity *own, float cd, float cTime, StatType stat, float buffDuration, float statMultiplier, float statAdditive)
{
	buff.elapsedTime = 0;
	buff.alteredStat = stat;
	buff.duration = buffDuration;
	buff.multiplier = statMultiplier;
	buff.additive = statAdditive;

	Ability::initialize(own, cd, cTime);
}

bool SelfBuffAbility::activate()
{
	//Add buff to LivingEntity's vector of buffs
	//LivingEntity will then need to count down from duration and remove the buff when it hits 0.
	if(isReady())
	{
		Ability::activate();
		owner->addBuff(buff);	
		return true;
	}
	return false;
}

