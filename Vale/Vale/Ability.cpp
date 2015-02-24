#include "StdAfx.h"
#include "Ability.h"

//Author:Alex George

Ability::Ability(void){}

Ability::~Ability(void){}

void Ability::initialize(LivingEntity *own, float cd, float cTime)
{
	timeRemaining = 0;
	owner = own;
	cooldown = cd;
	castTime = cTime;
}

//Update cooldown
void Ability::update(float frameTime){	timeRemaining -= frameTime;  }

//Use ability. Meant to be overridden
bool Ability::activate()
{
	timeRemaining = cooldown;
	return true;
}

//Used for syncing up ability cooldowns when warden switches bows. 
void Ability::setTimeRemaining(float time)
{
	timeRemaining = time;
}