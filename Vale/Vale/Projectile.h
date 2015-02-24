#pragma once
#include "InteractiveObject.h"
#include "ProjectileManager.h"
#include <math.h>

class ProjectileManager;

#define PI 3.14159265
#define MULTIPLIER 50

//Author:Alex George
//Represents a projectile fired from a ranged ability.
class Projectile :
	public InteractiveObject
{
public:
	//Constructors
	Projectile(void);
	~Projectile(void);

	//Member Functions
	void initialize(ProjectileManager *owner, ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, Faction fact);
	void update(float frameTime);
	void fire(D3DXVECTOR2 shooterPos, float shooterRotation, int projectileSpeed, int dmg, int projectileSize, bool pierces, bool ricochets);
	void reduceDamage();
	int pierceTarget(InteractiveObject* target);
	bool hasPierced(InteractiveObject* target);
	void graze() { if(grazed==false){ grazed = true; } }

	//Accessors
	ProjectileManager* getManager() { return manager; }
	float getAge() { return age; }
	float getDamage() { return damage; }
	bool isPiercing() { return piercing; }
	bool isRicochet() { return ricochet; }
	bool beenGrazed() { return grazed; }

private:
	//Attributes
	ProjectileManager *manager;
	float age, damage, size;
	bool piercing, ricochet;
	bool grazed;
	int value; // increases per pierce

	//keep track of how many targets it's hit.
	std::vector<InteractiveObject*> piercedTargets;
};

