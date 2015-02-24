#pragma once
#include "rangedattack.h"

//Author:Alex George

//Shoots multiple projectiles in an arc.
class SplitShotAbility :
	public RangedAttack
{
public:
	SplitShotAbility(void);
	~SplitShotAbility(void);

	void initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng, float projSpeed, int projRadius, bool pierces, bool ricochets, float arc, int numProj);
	bool activate();

protected: 
	float spreadArc;
	int numProjectiles;
};

