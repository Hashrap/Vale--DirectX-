#include "StdAfx.h"
#include "MeleeAttack.h"
#include <iostream>

//Author:Alex George
MeleeAttack::MeleeAttack(void){}

MeleeAttack::~MeleeAttack(void){}

void MeleeAttack::initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng,  float cArc)
{
	cleaveArc = cArc;
	AttackAbility::initialize(own, cd, cTime, dmg, rng);
}

bool MeleeAttack::activate(LivingEntity* obj)
{
	if(isReady())
	{
		//do ability
		//Check collisions within an arc using the range as the radius and cleaveArc as the angle
		//std::cout<<"MeleeAttack"<<std::endl;
		float dist2 = owner->distanceToTarget(obj);
		float radiusSum = (range*2);
		if(dist2 <= radiusSum * radiusSum)
		{
			if(owner->getFaction() == RISEN)
			{
				obj->takeDamage(damage);
			}
		}

		Ability::activate();
		return true;
	}

	//if not ready
	return false;
}

std::vector<InteractiveObject> MeleeAttack::checkCollision(std::vector<InteractiveObject> objectsOnScreen)
{
	//Get position of owner
	//Check delta distance against range
	
	//check angle of object against cleaveArc

	//return list of hit things
	std::vector<InteractiveObject> colliders(5);
	return colliders;
}