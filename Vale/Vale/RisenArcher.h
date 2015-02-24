#pragma once
#include "Risen.h"
#include "Warden.h"
#include "SelfBuffAbility.h"
#include "MeleeAttack.h"
#include "RangedAttack.h"
//Author:Bradley Lubahn
namespace risenArcherNS
{
	//general
	const int SIZE = 32;
	const float DEF_MOVEMENT = 450;
	const int MAX_HEALTH =  25;
	const int FLEE_HEALTH = 10;

	const float SEEK_DISTANCE = 320000; //500*500
	const float ATTACK_DISTANCE = 5625; //75*75
	const float RANGEDATT_DISTANCE = 120000; //300*300
	//buff parameters for enemy speed boosts
	const float BUFF_COOLDOWN = 15;
	const float BUFF_DURATION =5;
	const float BUFF_CAST_TIME=1;
	const float BUFF_MULTIPLE=1.5;
	const float BUFF_ADD = 0;
	const StatType STATTYPE = MOVEMENTSPEED;
	//attack parameters for if the warden gets to close
	const int MELEE_COOLDOWN = 3;
	const float MELEE_CTIME = 0;
	const int MELEE_DMG = 1;
	const int MELEE_RANGE = 100;
	const float MELEE_ARC = 90;
	//Ranged attack parameters
	const int RANGE_COOLDOWN = 2;
	const float RANGE_CTIME = 0;
	const int RANGE_DMG = 1;
	const int RANGE_RANGE = 1000;
	const float RANGE_SPEED = 500;
	const float RANGE_RADIUS = 20;
}
class RisenArcher :
	public Risen
{
public:
	RisenArcher(void);
	~RisenArcher(void);

	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,ValeTexture *projText, Warden *p);
	void update(float frameTime,std::vector<Risen*>* rVec);

private:
	//Methods
	void determineBehavior();
	//Attribute
	float rangedAttackDistance;
	//Buffs and Abilties
	SelfBuffAbility speedBuff;
	MeleeAttack attackAbi;
	RangedAttack rangedAbi;
};

