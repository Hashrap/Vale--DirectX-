#include "StdAfx.h"
#include "AttackAbility.h"

//Represents an ability meant to deal damage.
//Author:Alex George
AttackAbility::AttackAbility(void){}

AttackAbility::~AttackAbility(){}


void AttackAbility::initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng)
{
	damage = dmg;
	range = rng;
	Ability::initialize(own, cd, cTime);
}