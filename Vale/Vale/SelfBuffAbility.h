#pragma once
#include "ability.h"


//Author:Alex George

//Self Buff abilities apply a stat change to the owner of the ability. e.g. increased movement speed
class SelfBuffAbility :
	public Ability
{
public: 

	SelfBuffAbility(void);
	~SelfBuffAbility(void);	

	void initialize(LivingEntity *own, float cd, float cTime, StatType stat, float buffDuration, float statMultiplier, float statAdditive);
	bool activate();

protected:
	StatBuff buff;	
};