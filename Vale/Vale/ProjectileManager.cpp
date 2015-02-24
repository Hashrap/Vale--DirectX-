#include "StdAfx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager(void){}

ProjectileManager::~ProjectileManager(void){}

void ProjectileManager::initialize(ValeGame *gamePtr, ValeTexture *projTexA, Faction fact)
{
	faction = fact;
	lifespan = 30;
	arraySize = 500;

	projectileTextureA = projTexA;

	for(int i = 0; i < arraySize; i++)
	{
		Projectile p;			
		p.initialize(this, gamePtr, 12, 12, 1, projTexA, fact);
		allProjectiles.push_back(p);
	}
	pooledProjectiles = allProjectiles;
}

void ProjectileManager::update(float frameTime)
{
	Projectile projectile;
	
	//Loop through active projectiles
	for(unsigned int i =0 ; i < activeProjectiles.size(); i++)
	{
		//Move the projectiles
		activeProjectiles[i].update(frameTime);

		//Remove old projectiles
		if(activeProjectiles[i].getAge() >= lifespan)
		{
			killProjectile(&activeProjectiles[i], i);
		}
	}
}

//Spawns a projectile at the given position with the given rotation, speed, and damage
bool ProjectileManager::fire(D3DXVECTOR2 shooterPos, float shooterRotation, int projectileSpeed, int damage, int projectileSize, bool pierces = false, bool ricochets = false)
{
	//All bullets used, no shot will fire
	if(pooledProjectiles.size() == 0) return false;

	//Grab the last projectile
	Projectile projectile = pooledProjectiles.back();

	//Pop it off
	pooledProjectiles.pop_back();

	//Fire the bullet
	//projectile.initialize();
	projectile.fire(shooterPos, shooterRotation, projectileSpeed, damage, projectileSize, pierces, ricochets);			

	//Set bullet to active
	activeProjectiles.push_back(projectile);

	return true;
}

void ProjectileManager::killProjectile(Projectile* projectile, int index)
{
	//projectile->setCellPtr(0);
	//projectile->setCell(-1,-1);

	//Put it pack in the pool
	pooledProjectiles.push_back(*projectile);

	//Remove the projectile from active projectiles
	activeProjectiles.erase(activeProjectiles.begin() + index);
}


void ProjectileManager::killProjectile(Projectile* projectile)
{
	//projectile->setCellPtr(0);
	//projectile->setCell(-1,-1);
	for(unsigned int i = 0; i < activeProjectiles.size(); i++)
	{
		if(&(activeProjectiles[i]) == projectile)
		{
			//Put it pack in the pool
			pooledProjectiles.push_back(*projectile);

			//Remove the projectile from active projectiles
			activeProjectiles.erase(activeProjectiles.begin()+i);
			break;
		}
	}	
}

void ProjectileManager::updateProjectileImages(VECTOR2 v)
{
	for(unsigned int i = 0; i < activeProjectiles.size(); i++)
	{
		//VECTOR2 newPosition = activeProjectiles[i].getPosition() - v;
		activeProjectiles[i].updateImagePosition(v.x, v.y );
	}
}

void ProjectileManager::draw()
{
	for(unsigned int i = 0; i < activeProjectiles.size(); i++)
	{
		activeProjectiles[i].draw();
	}
}