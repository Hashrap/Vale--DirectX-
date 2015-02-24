#pragma once
#include "attackability.h"
#include <vector>

//Author:Alex George
class MeleeAttack :
	public AttackAbility
{
public:
	MeleeAttack(void);
	~MeleeAttack(void);

	void initialize(LivingEntity *own, float cd, float cTime, int dmg, float rng, float cArc);
	bool activate(LivingEntity* obj);
private:
	//Melee attacks consist of an arc and a range to determine its hit box.
	float cleaveArc;

	std::vector<InteractiveObject> checkCollision(std::vector<InteractiveObject> objectsOnScreen);
};

