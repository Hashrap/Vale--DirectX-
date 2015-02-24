#pragma once
#include "AttackAbility.h"

//Author:Alex George
//Ranged attack. The default implementation shoots a single projectile in a line.
class RangedAttack :
	public AttackAbility
{
public:
	RangedAttack(void);
	~RangedAttack(void);

	void initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng, float projSpeed, int projRadius, bool pierces, bool ricochets);
	bool activate();

protected:
	float projectileSpeed;
	int projectileRadius; 
	bool piercing, ricochet; //If piercing, the projectile will continue travelling if it collides with an enemy.
};

