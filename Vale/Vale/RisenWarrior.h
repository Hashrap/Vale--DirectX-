#pragma once

#include "Risen.h"
#include "Warden.h"
#include "SelfBuffAbility.h"
#include "MeleeAttack.h"

namespace risenWarriorNS
{
	//general
	const int SIZE = 32;
	const float DEF_MOVEMENT = 200;
	const int MAX_HEALTH =  25;
	const int FLEE_HEALTH = 10;

	const float SEEK_DISTANCE = 320000; //400*400
	const float ATTACK_DISTANCE = 5625; //75*75
	//buff parameters for enemy speed boosts
	const float BUFF_COOLDOWN = 15.0f;
	const float BUFF_DURATION = 5.0f;
	const float BUFF_CAST_TIME= 1.0f;
	const float BUFF_MULTIPLE = 1.5f;
	const float BUFF_ADD = 0;
	const StatType STATTYPE = MOVEMENTSPEED;
	//attack parameters for warriors melee attack
	const float ATTACK_COOLDOWN = 3.0f;
	const float ATTACK_CTIME = 0.0f;
	const int ATTACK_DMG = 1;
	const int ATTACK_RANGE = 100;
	const float ATTACK_ARC = 90;
}

//Author:Bradley Lubahn
class RisenWarrior :
	public Risen
{
public:
	RisenWarrior(void);
	~RisenWarrior(void);
	//Method Definition
	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,ValeTexture *projText, Warden *p,std::vector<int> tiles);
	void update(float frameTime,std::vector<Risen*>* rVec);
	
private:
	//Risen Warrior Specific methods
	void determineBehavior();
	//Buffs and Abilties 
	SelfBuffAbility speedBuff;

	MeleeAttack attackAbi;
};

