#pragma once
#include "Projectile.h"
#include "ValeGraphics.h"
#include "ValeTexture.h"
#include "ValeImage.h"
#include "ValeResources.h"
#include <vector>
#include <stack>

class Projectile;

//Author: Alex George

class ProjectileManager
{
public:
	//Constructors
	ProjectileManager(void);
	~ProjectileManager(void);

	//Member Functions
	void initialize(ValeGame *gamePtr, ValeTexture *projTexA, Faction fact);
	void update(float frameTime);
	void draw();
	bool fire(D3DXVECTOR2 shooterPos, float shooterRotation, int projectileSpeed, int damage, int projectileSize, bool pierces, bool ricochets);
	void killProjectile(Projectile* projectile, int index);
	void killProjectile(Projectile* projectile);
	void updateProjectileImages(VECTOR2 v);

	//Accessors
	std::vector<Projectile>* getActiveProjectiles() { return &activeProjectiles; }
	int getPooledAmount() { return pooledProjectiles.size(); }

	//Mutator
	void setFaction(Faction fac) { faction = fac; }
private:
	//Attributes
	ValeTexture* projectileTextureA, *projectileTextureB;

	ValeGraphics *graphics;
	int lifespan, arraySize;
	Faction faction;
	std::vector<Projectile> allProjectiles;
	std::vector<Projectile> activeProjectiles;
	std::vector<Projectile> pooledProjectiles;	
};

