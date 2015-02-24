#include "StdAfx.h"
#include "SplitShotAbility.h"

//A ranged attack that shoots multiple projectiles in an arc
//Author:Alex George
SplitShotAbility::SplitShotAbility(void){}


SplitShotAbility::~SplitShotAbility(void){}

void SplitShotAbility::initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng, float projSpeed, int projRadius, bool pierces, bool ricochets, float arc, int numProj)
{
	spreadArc = arc;
	numProjectiles = numProj;
	RangedAttack::initialize(own, cd, cTime, dmg, rng, projSpeed, projRadius, pierces, ricochets);
}

bool SplitShotAbility::activate()
{
	//if out of bullets or on cooldown
	if(owner->getPoolAmt() == 0 || !isReady()) { return false; }
	else
	{
		Ability::activate();

		//Shoots multiple projectiles depending on the given arc.
		for(int i = 0; i < numProjectiles; i ++)
		{
			//Determine angle of the projectile. Angle is based on arc given and which projectile we're shooting.
			float angle = (owner->getRotation() + (.5*spreadArc)) - ( i * ( spreadArc / (numProjectiles - 1) ) );
			owner->fire(projectileSpeed, damage, projectileRadius, piercing, ricochet, angle);
		}
		return true;
	}
}