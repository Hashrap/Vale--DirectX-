#pragma once
#include "rangedattack.h"

#define SPELL_COUNT 3

// Author: Spencer Corkran, Alex George
// Implementation of the Risen Warlock's spellcasting powers
class WarlockSpell :
	public RangedAttack
{
public:
	WarlockSpell(void);
	~WarlockSpell(void);
	
	void initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng, float projSpeed, int projRadius, bool pierces, bool ricochets);
	bool activate();
	void update(float frameTime);

	//Spell effects
	void spray(float sweep, int rays, int waves, float speed, float degrade, float wave_offset = 0);

	bool isChanneling() { return channeling; }
	float getChannelTime() { return channelTime; }

protected:
	float channelTime; //how long can this ability be channeled
	bool channeling; //currently channeling?
	int counter; // Keep track of how many spells have been fired consecutively
};

