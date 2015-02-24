#pragma once
#include "InteractiveObject.h"
#include "ProjectileManager.h"
#include "Ability.h"
#include "ValeResources.h"
#include <vector>
#include <algorithm>

class Ability;

//Author:Alex George
//this class is for all living entities that can move around - e.g. the warden, the risen, the boss
class LivingEntity :
	public InteractiveObject
{
protected:
	// Attributes
	ProjectileManager projectileManager;

	std::vector<StatBuff> buffs;

	float baseMovementSpeed; //default movementspeed. Doesn't change after being set.
	float additionalMovementSpeed; //defaults as 0 - buffs and other effects increase this. final movement speed = base + additional	
	
	int currHealth;
	int maxHealth;
	bool alive;

	char * deathSound;

	// Member functions
	void turn(int degrees);

public:
	// Constructors
	LivingEntity(void);
	~LivingEntity(void);
	//int currHealth;
	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, ValeTexture *projText, Faction fact);
	void update(float frameTime);
	void draw();
	void updateProjectiles(VECTOR2 levelCoords);

	bool fire(float projectileSpeed, int damage, int projectileSize, bool pierces, bool ricochets, float rot);
	void addBuff(StatBuff buff);
	void removeBuff(StatBuff buff);
	virtual void takeDamage(int damageTaken);


	//Accessors
	int getPoolAmt() { return projectileManager.getPooledAmount(); }
	ProjectileManager* getProjectileManager() { return &projectileManager; }	
	int getCurrentHealth() { return currHealth; }
	bool isFullHealth() { return currHealth == maxHealth; }	
	bool isAlive() { return alive; }
	virtual float actualMovementSpeed(){return baseMovementSpeed + additionalMovementSpeed;	}
};

