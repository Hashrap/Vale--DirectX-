#pragma once
#include "rangedattack.h"

//Author: Alex George
//implementation for the warden's focus shot ability.
class FocusShot :
	public RangedAttack
{
public:
	FocusShot(void);
	~FocusShot(void);
	
	void initialize(LivingEntity *own,  int projRadius, bool ricochets, float minSweet, 
							float maxSweet, float minSp, float sweetSp, float maxSp, float minDmg, float sweetDmg, float maxDmg);
	void update(float frameTime);
	bool activate();


	bool beginChanneling();
	bool endChanneling();

	bool isChanneling() { return channeling; }
	bool isSweet();

	void museBonus();

	float getChannelPercent() { return channelTime / maxChannelTime; }
protected:

	float maxChannelTime; //max time this ability can be channeled. in seconds.

	float minSweetBoundary, maxSweetBoundary; //the boundaries (in seconds) where a "sweetspot" will occur

	float channelTime; //how long can this ability be channeled

	float sweetDamage, sweetSpeed;
	float minSpeed, maxSpeed; //min speed of premature shot. max speed of misfired shot
	float minDamage, maxDamage; //min damage of premature shot. max damage of misfired shot

	bool channeling; //currently channeling?
};

