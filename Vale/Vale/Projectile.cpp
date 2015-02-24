#include "StdAfx.h"
#include "Projectile.h"
#include <iostream>

//Represents a projectile fired from a ranged attack ability
//Author:Alex George
Projectile::Projectile(void){}

Projectile::~Projectile(void){}

void Projectile::initialize(ProjectileManager *owner, ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, Faction fact)
{
	manager = owner;
	faction = fact;
	name = "Projectile";
	description = "Moving projectile that hurts enemies";
	value = 0;
	grazed = false;
	InteractiveObject::initialize(gamePtr, width, height, ncols, textureM);

}

void Projectile::update(float frameTime)
{
	age+=frameTime;
	InteractiveObject::update(frameTime);
}

//Shoot a projectile with the given parameters
void Projectile::fire(D3DXVECTOR2 shooterPos, float shooterRotation, int projectileSpeed, int dmg, int projectileSize, bool pierces = false, bool ricochets = false)
{
	image.setScale(projectileSize/12.0f);

	grazed = false;
	value = 0;
	age = 0;
	piercedTargets.clear();
	piercing = pierces;
	ricochet = ricochets;

	//Set initial position and rotation of projectile to the shooter's own
	position = shooterPos;
	VECTOR2 temp = getCenter();
	temp.x = (int)(temp.x/40); // TO-FIX: MAGIC NUMBERS
	temp.y = (int)(temp.y/40); // TO-FIX: Need a way to get grid dimensions from level or cGrid
	cell = temp;
	image.setDegrees(shooterRotation);
	rotation = shooterRotation;

	//Set this projectile's movementVectors by taking the shooter's and adding projectileSpeed.
	velocity.x = cos(rotation * PI/180.0) * projectileSpeed;
	velocity.y = sin(rotation * PI/180.0) * projectileSpeed;

	//Store the damage of the projectile
	damage = dmg;

	size = projectileSize;
}

//cuts damage by 10%. used for piercing shots
void Projectile::reduceDamage()
{
	damage*=.9;
}

int Projectile::pierceTarget(InteractiveObject *target)
{
	piercedTargets.push_back(target);
	reduceDamage();
	if(faction == PLAYER)
	{
		value += MULTIPLIER * (piercedTargets.size()-1); //-1 because the first hit isn't a combo
		//std::cout << "COMBO: " << (piercedTargets.size()-1) << "-" << value << std::endl;
		return value;
	}
	return 0;
}

bool Projectile::hasPierced(InteractiveObject *target)
{
	for(unsigned int i = 0; i < piercedTargets.size(); i++)
	{
		if(piercedTargets[i] == target)
		{
			return true;
		}
	}
	return false;
}