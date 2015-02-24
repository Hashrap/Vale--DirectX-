#pragma once
#include "ability.h"

//Author:Alex George
class AttackAbility :
	public Ability
{
public:
	AttackAbility(void);
	~AttackAbility(void);

	void initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng);

protected:
	float range;	//Range is used by melee-type abilities to determine how close the enemy must be to swing.
				    //Range is used by ranged abilities to determine how far the projectile may go. (this number will be high)
					//Enemies will utilize an attack behavior range to decide when to attack; they won't use this
	int damage;
};