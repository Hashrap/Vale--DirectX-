#include "StdAfx.h"
#include "WarlockSpell.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

WarlockSpell::WarlockSpell(void)
{
}


WarlockSpell::~WarlockSpell(void)
{
}

void WarlockSpell::initialize(
	LivingEntity *own, float cd, float cTime, int dmg, float rng, float projSpeed, int projRadius, bool pierces, bool ricochets)
{
	RangedAttack::initialize(own, cd, cTime, dmg, rng, projSpeed, projRadius, pierces, ricochets);
}

void WarlockSpell::update(float frameTime)
{

	Ability::update(frameTime);
}

bool WarlockSpell::activate()
{
	if(owner->getPoolAmt() == 0 || !isReady()) { return false; }
	else
	{
		// select a spell here TO-DO
		srand(time(NULL));
		int spell = (rand() % SPELL_COUNT) + 1;

		switch(spell)
		{
		case 1:
			spray(120.0f, 6, 3, 200, 80);
			break;
		case 2:
			spray(360.0f, 9, 4, 250, 40, 5);
			break;
		case 3:
			spray(70.0f, 8, 5, 350, 60);
			break;
		}
		Ability::activate();
		counter++;
		return true;
	}
}

void WarlockSpell::spray(float sweep, int rays, int waves, float _speed, float degrade, float wave_offset)
{
	//Shoots multiple projectiles depending on the given arc.
	for(int i = 0; i < rays; i ++)
	{
		float angle;
		if(rays==1)
			angle = owner->getRotation();
		else
			angle = owner->getRotation() + ((0.5f*sweep)) - ( i * ( sweep / (rays-1.0f)));

		for(int j = 0; j < waves; j++)
		{
			float _speed = projectileSpeed-(j*degrade);
			//Determine angle of the projectile. Angle is based on arc given and which projectile we're shooting.
			owner->fire(_speed, damage, projectileRadius, piercing, ricochet, angle+(wave_offset*j));
		}
	}
}