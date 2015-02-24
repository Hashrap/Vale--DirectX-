#include "StdAfx.h"
#include "RangedAttack.h"

//This ability shoots ranged projectiles.
//Author:Alex George
RangedAttack::RangedAttack(void){}

RangedAttack::~RangedAttack(void){}

void RangedAttack::initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng, float projSpeed, int projRadius, bool pierces, bool ricochets)
{
	projectileSpeed = projSpeed;
	projectileRadius = projRadius;
	piercing = pierces;
	ricochet = ricochets;
	AttackAbility::initialize(own, cd, cTime, dmg, rng);
}

bool RangedAttack::activate()
{
	//if out of bullets or on cooldown
	if(owner->getPoolAmt() == 0 || !isReady()) { return false; }
	else
	{
		//Shoot one projectile
		Ability::activate();
		return owner->fire(projectileSpeed, damage, projectileRadius, piercing, ricochet, owner->getRotation());
	}
}