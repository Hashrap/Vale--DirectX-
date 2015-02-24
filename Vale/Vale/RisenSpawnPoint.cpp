#include "StdAfx.h"
#include "RisenSpawnPoint.h"


//Author: Bradley Lubahn
/*
	The Risen Spawn points come in three different types(Timed, Trigger, and Static)
	They cannot be used in combination
	
	Timed points will spawn a specified enemy type until the duration of the spawn end.
	The Enemies will spawn at a fixed interval which can be changed in RisenSpawner

	Trigger points will spawn a specified enemy type until the spawn point is turned off
	To turn it off call turnOnAndOff, which will toogle its current state

	Static points must be created ahead of time, these points will store a vector of risen
	that the spawn can take and then give to level. These are intend to be smaller group,and 
	all risen in the vector will spawn at the same time. The createRisen function can be used to
	populate the vector, each call will add more risen into the vector to be spawn when it is reached.
*/

RisenSpawnPoint::RisenSpawnPoint():InteractiveObject()
{
	spawnType = Risen::WAR;

	//Timed
	isTimedSpawn = false;
	spawnTimer = 30;
	//Trigger
	triggerSpawn = false;
	active = false;
	//Static
	staticSpawn = false; 

	spawning = false;
	
}


RisenSpawnPoint::~RisenSpawnPoint(void)
{
}

void RisenSpawnPoint::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,float distanceToActivate)
{	
	InteractiveObject::initialize(gamePtr,  width,  height,  ncols, textureM);
	spawnDistance = distanceToActivate;
	spawnInterval =risenSpawnNS::SPAWN_INTERVAL;
	visited = false;
	spawnedFirstEnemy = false;
}
int RisenSpawnPoint::getSpawnType()
{
	//returns the current type of spawn point
	if(isTimedSpawn)
		return TIMED;
	if(triggerSpawn)
		return TRIGGER;
	if(staticSpawn)
		return STATIC;

	return INACTIVE;
}
void RisenSpawnPoint::update(float frameTime)
{
	InteractiveObject::update(frameTime);
}
//Sets this Spawn point to be a timed point
void RisenSpawnPoint::setTimed(Risen::types type, float time)
{
	isTimedSpawn = true;
	spawnType = type;
	spawnTimer = time;

	currentSpawnTime=spawnTimer;
	currentSpawnInterval=spawnInterval;

	staticSpawn = false;
	triggerSpawn = false;

}
//Sets this Spawn point to be a  trigger spawn point
void RisenSpawnPoint::setTriggerSpawn(Risen::types type)
{
	spawnType = type;
	triggerSpawn = true;

	staticSpawn = false;
	isTimedSpawn = false;
}
//Sets this Spawn point to be a Trigger spawn, and adds on to the current vector of risen used by this type of spawn 
//point, if you want multiple types of risen the function currently works that you must call it once per type of risen
void RisenSpawnPoint::createRisen(Risen::types type, int amount)
{
	//add to a vector of risen, if it is
	if(type == Risen::WAR)
	{
		for(int i=0; i < amount; i++)
		{
			RisenWarrior* rW = new RisenWarrior();
			rW->setPosition(VECTOR2(this->getPosition().x,this->getPosition().y));
			risen.push_back(rW);
		}
	}
	else if(type == Risen::ARCHER)
	{
		for(int i=0; i < amount; i++)
		{
			RisenArcher* rA = new RisenArcher();
			rA->setPosition(VECTOR2(this->getPosition().x,this->getPosition().y));
			risen.push_back(rA);
		}
	}
	else if(type == Risen::HOUND)
	{
		for(int i=0; i < amount; i++)
		{
			RisenHound* rH = new RisenHound();
			rH->setPosition(VECTOR2(this->getPosition().x,this->getPosition().y));
			risen.push_back(rH);
		}
	}
	//add more
	else if(type == Risen::WARLOCK)
	{
		for(int i=0; i < amount; i++)
		{
			RisenWarlock* rLock = new RisenWarlock();
			rLock->setPosition(VECTOR2(this->getPosition().x,this->getPosition().y));
			risen.push_back(rLock);
		}
	}
	//set this mode to true and  Other modes to false
	staticSpawn = true;
	isTimedSpawn = false;
	triggerSpawn = false;

}
void RisenSpawnPoint::turnOnAndOff()
{
	if(active)
		active = false;
	else
		active = true;
}