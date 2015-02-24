#pragma once
#include "InteractiveObject.h"
#include "LivingEntity.h"
#include "ValeRectangle.h"
#include "ValeResources.h"

class LivingEntity;

//Author: Alex George

//Represents an ability - can be used any living entity
class Ability
{
public:
	Ability(void);
	~Ability(void);

	void initialize(LivingEntity *own, float cd, float cTime);
	virtual void update(float gameTime);

	void setTimeRemaining(float time);
	float getTimeRemaining() { return timeRemaining; }

	//Use ability
	bool activate();

	//Is it off cooldown?
	bool isReady() { return timeRemaining <= 0; }
protected:
	LivingEntity *owner;
	
	float cooldown, timeRemaining;
	float castTime;
};

