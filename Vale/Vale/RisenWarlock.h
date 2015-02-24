// Author: Bradley Lubahn, Spencer Corkran

#pragma once

#include "Risen.h"
#include "Warden.h"
#include "SelfBuffAbility.h"
#include "WarlockSpell.h"
#include "RangedAttack.h"

namespace risenWarlockNS
{
	//general
	const int SIZE = 32;
	const float DEF_MOVEMENT = 300;
	const int MAX_HEALTH =  30;
	const int FLEE_HEALTH = 10;

	const float FLEE_DISTANCE = 60000;
	const float SEEK_DISTANCE = 300000; //500*500
	const float ATTACK_DISTANCE = 5625; //75*75
	const float RANGEDATT_DISTANCE = 120000; //300*300

	//Ranged attack parameters
	const float RANGE_COOLDOWN = 3.0f;
	const float RANGE_CTIME = 0.0f;
	const int RANGE_DMG = 1;
	const float RANGE_RANGE = 1000.0f;
	const float RANGE_SPEED = 100.0f;
	const int RANGE_RADIUS = 20;

	//Spell parameters
	const float SPRAY_COOLDOWN = 3.0f;
	const float SPRAY_CTIME = 0.0f;
	const int SPRAY_DMG = 1;
	const float SPRAY_RANGE = 2000.0f;
	const float SPRAY_SPEED = 200.0f;
	const int SPRAY_RADIUS = 15;

	//Absorb parameters
	const float ABSORB_COOLDOWN = 10.0f;
	const float ABSORB_CTIME = 2.0f;
	const StatType ABSORB_STATTYPE = CURRENTHEALTH;
	const float ABSORB_DURATION = 4.0f;
	const float ABSORB_MULTIPLIER = 2.0f;
	const float ABSORB_ADDITIVE = 0.0f;
}

class RisenWarlock:
	public Risen
{
public:
	RisenWarlock(void);
	~RisenWarlock(void);

	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, ValeTexture *projText, Warden *p);
	void update(float frameTime);
private:
	void determineBehavior();
private:
	//Attributes
	float rangedAttackDistance;
	//Buffs and Abilties
	SelfBuffAbility absorb;
	WarlockSpell spray;
	RangedAttack primary;
};

