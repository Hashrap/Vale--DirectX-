#include "StdAfx.h"
#include "LivingEntity.h"

//Author:Alex George
LivingEntity::LivingEntity():InteractiveObject()
{
	//Set additional stats to 0
	additionalMovementSpeed = 0;

	alive = true;
}


LivingEntity::~LivingEntity(){}

void LivingEntity::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, ValeTexture *projText, Faction fact)
{	
	projectileManager.initialize(gamePtr, projText, fact);
	InteractiveObject::initialize(gamePtr,  width,  height,  ncols, textureM);
}

void LivingEntity::update(float frameTime)
{
	//Buffs
	//reset stat modifiers
	additionalMovementSpeed = 0;

	//loop through buffs and apply them
	for(unsigned int i = 0; i < buffs.size(); i++)
	{
		buffs[i].elapsedTime+=frameTime;

		switch(buffs[i].alteredStat)
		{
		case MOVEMENTSPEED:
			additionalMovementSpeed = buffs[i].additive + (( buffs[i]. multiplier * baseMovementSpeed) - baseMovementSpeed);
			break;
		}

		//remove buffs that have reached the end of their duration
		if(buffs[i].elapsedTime >= buffs[i].duration)
		{
			buffs.erase(buffs.begin() + i);
		}
	}

	//Health and Death
	if(alive && currHealth <= 0)
	{
		currHealth = 0;
		alive = false;
		audio->playCue(deathSound);
	}

	//Update projectile manager
	projectileManager.update(frameTime);
	InteractiveObject::update(frameTime);
}

void LivingEntity::updateProjectiles(VECTOR2 levelCoords)
{
	projectileManager.updateProjectileImages(levelCoords);
}

void LivingEntity::draw()
{
	projectileManager.draw();
	InteractiveObject::draw();
}

//Tells the projectile manager to fire a projecitle
bool LivingEntity::fire(float projectileSpeed, int damage, int projectileSize, bool pierces, bool ricochets, float rot)
{
	return projectileManager.fire(VECTOR2(position.x + image.getWidth()/2, position.y + image.getWidth()/2) , rot, projectileSpeed, damage, projectileSize, pierces, ricochets);
}

//Adds a buff to the LE
void LivingEntity::addBuff(StatBuff buff)
{
	buffs.push_back(buff);
}

//Removes selected buff before its duration runs out.
void LivingEntity::removeBuff(StatBuff buffToFind)
{
	for(unsigned int i = 0 ;i < buffs.size(); i++)
		if(buffs[i].alteredStat == buffToFind.alteredStat && buffs[i].elapsedTime == buffToFind.elapsedTime)
			buffs.erase(buffs.begin() + i);
}

void LivingEntity::takeDamage(int damageTaken)
{
	if(currHealth>0)
		currHealth -= damageTaken;
}