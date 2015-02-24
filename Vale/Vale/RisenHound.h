#pragma once
#include "Risen.h"
#include "Warden.h"
#include "SelfBuffAbility.h"
#include "MeleeAttack.h"
//Author: Bradley Lubahn
namespace risenHoundNS
{
	//general
	const int SIZE = 32;
	const float DEF_MOVEMENT = 300;
	const int MAX_HEALTH =  15;
	const int FLEE_HEALTH = 0;

	const float SEEK_DISTANCE = 320000; //400*400
	const float ATTACK_DISTANCE = 5625; //75*75
	const float CHARGE_DISTANCE = 22500;  //150*150
	//buff parameters for enemy speed boosts
	const float BUFF_COOLDOWN = 15.0f;
	const float BUFF_DURATION = 10.0f;
	const float BUFF_CAST_TIME= 1.0f;
	const float BUFF_MULTIPLE= 3.0f;
	const float BUFF_ADD = 0.0f;
	const StatType STATTYPE = MOVEMENTSPEED;
	//attack parameters for melee attack
	const float ATTACK_COOLDOWN = 3;
	const float ATTACK_CTIME = 0.0f;
	const int ATTACK_DMG = 1;
	const float ATTACK_RANGE = 100;
	const float ATTACK_ARC = 90;
	//attack parameters for charging attack
	const float CHARGE_COOLDOWN = 20.0f;
	const float CHARGE_CTIME = 1.0f;
	const int CHARGE_DMG = 3;
	const float CHARGE_RANGE = 100.0f;
	const float CHARGE_ARC = 90.0f;
	//Simple timer
	const int CHARGE_TIME = 3;
}

class RisenHound :
	public Risen
{
public:
	RisenHound(void);
	~RisenHound(void);
	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,ValeTexture *projText, Warden *p);
	void update(float frameTime,std::vector<Risen*>* rVec);

private:
	//Risen Warrior Specific methods
	void determineBehavior();
	//Buffs and Abilties 
	bool charging;
	bool attacking;
	float chargeDistance;
	float chargeTimer;

	SelfBuffAbility speedBuff;

	MeleeAttack attackAbi;
	MeleeAttack chargeAbi;
};

