#include "stdafx.h"
#include "RisenSpawner.h"
#include "InteractiveObject.h"
//Author:Bradley Lubahn 
//Controls the risen spawn points, and how they interact with the warden.
RisenSpawner::RisenSpawner():InteractiveObject()
{

}


RisenSpawner::~RisenSpawner(void)
{

}

void RisenSpawner::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, std::vector<Risen*>* risen, Warden* ward, std::vector<int> tilePtr)
{	
	InteractiveObject::initialize(gamePtr,  width,  height,  ncols, textureM);
	myGamePtr = gamePtr;
	warden = ward;
	risenVector = risen;
	tiles = tilePtr;
	risenWarriorTexture.initialize(myGamePtr->getGraphics(), RISEN_IMAGE);
	risenArcherTexture.initialize(myGamePtr->getGraphics(),ARCHER_IMAGE);
	risenHoundTexture.initialize(myGamePtr->getGraphics(),HOUND_IMAGE);
	risenWarlockTexture.initialize(myGamePtr->getGraphics(),WARLOCK_IMAGE);
	archerProjectileTexture.initialize(myGamePtr->getGraphics(), ARCHER_PROJECTILE);

	//for(int i = 0; i < 8; i++)
		warlockProjectileTexture/*[i]*/.initialize(myGamePtr->getGraphics(), WARLOCK_RED);

	spawnInterval =risenSpawnNS::SPAWN_INTERVAL;
	spawningTime = risenSpawnNS::SPAWN_TIME;
	
}

void RisenSpawner::update(float frameTime)
{

	for(int i = 0, count = spawnPoints.size(); i < count; i++)
	{
		if(spawnPoints[i]->getActive())
		{
			//if we are spawning do what we need to based on its type
			//Static Type spawn, checks if the point has been visited, if it hasn't it spawns all of the 
			//Risen contained within its vector
			if(spawnPoints[i]->getSpawnType() == RisenSpawnPoint::STATIC)
			{
				if(!spawnPoints[i]->getVisited())
				{
					//std::cout<<"Warriors Created"<<std::endl;
					spawnPoints[i]->setVisited(true);
					spawnVectorOfRisen(i);
				}
			}
			else if(spawnPoints[i]->getSpawnType() == RisenSpawnPoint::TIMED)
			{
				//std::cout<<"Spawn Point:"<<i<<"is a: Timed Spawn point"<<std::endl;
				//if this spawn point hasn't already been trigger start timer
				if(spawnPoints[i]->getVisited()&&spawnPoints[i]->getActive())
				{
					if(spawnPoints[i]->getCurrentSpawnTime() > 0)
					{
							if(spawnPoints[i]->getCurrentSpawnInterval() > 0)
							{
								spawnPoints[i]->setCurrentSpawnInterval(spawnPoints[i]->getCurrentSpawnInterval() - frameTime);
							}
							else
							{
								spawnPoints[i]->setCurrentSpawnInterval(spawnPoints[i]->getSpawnInterval());
								spawnRisen(spawnPoints[i]->getUnitSpawnType(),i);
							}
						spawnPoints[i]->setCurrentSpawnTime(spawnPoints[i]->getCurrentSpawnTime()-frameTime);
					}
					else
					{
						spawnPoints[i]->setActive(false);
					}
				}
				else
				{
					spawnPoints[i]->setVisited(true);
				}

			}
			else if(spawnPoints[i]->getSpawnType() == RisenSpawnPoint::TRIGGER)
			{
				//std::cout<<"Spawn Point:"<<i<<"is a: Trigger Spawn point"<<std::endl;
				if(!spawnPoints[i]->getVisited())
				{
					spawnPoints[i]->setVisited(true);
					spawnPoints[i]->turnOnAndOff();
					spawnInterval = spawnPoints[i]->getSpawnInterval();
				}
				else
				{
					if(spawnPoints[i]->getTrigger())
					{
						if(spawnInterval > 0)
						{
							spawnInterval-=frameTime;
						}
						else
						{
							spawnInterval = spawnPoints[i]->getSpawnInterval();
							spawnRisen(spawnPoints[i]->getUnitSpawnType(),i);
						}
					}
				}
				
			}
			else
			{
				std::cout<<"Spawn not working"<<std::endl;
			}
		}
		else
		{
			//check to see if we are close enought to active the SpawnPoint
			if(spawnPoints[i]->distanceToTarget(warden->getPosition()) < spawnPoints[i]->getSpawnDistance())
			{
				spawnPoints[i]->setActive(true);
				if(spawnPoints[i]->spawnedFirstEnemy)
				{
					spawnRisen(spawnPoints[i]->getUnitSpawnType(),i);
					spawnPoints[i]->spawnedFirstEnemy = true;
				}
			}
		}
	}

	InteractiveObject::update(frameTime);
}

void RisenSpawner::addSpawnPoint(RisenSpawnPoint* addPoint)
{
	spawnPoints.push_back(addPoint);
}
//This method is a helper method to Help keep the update method clean, 
//It loops through and adds the static risen from the spawn point to the main games risen vector
void RisenSpawner::spawnVectorOfRisen(int index)
{
	RisenSpawnPoint* point= spawnPoints[index];
	RisenWarrior* rW;
	RisenArcher* rA;
	RisenHound* rH;
	RisenWarlock* rLock;
	int type;
	//Loop through each of the risen in the array check what type it is, and initialize that type of risen
	for(int j = 0, amountOfRisen = spawnPoints[index]->getRisen().size(); j < amountOfRisen; j++)
	{
		type = spawnPoints[index]->getRisen()[j]->type;
		if(type == Risen::WAR)
		{
			rW = dynamic_cast<RisenWarrior*>(spawnPoints[index]->getRisen()[j]);

			rW->initialize(myGamePtr,32,32,1,&risenWarriorTexture,&archerProjectileTexture,warden,tiles);
			rW->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rW);
		}
		else if(type == Risen::ARCHER)
		{
			rA = dynamic_cast<RisenArcher*>(spawnPoints[index]->getRisen()[j]);

			rA->initialize(myGamePtr,32,32,1,&risenArcherTexture,&archerProjectileTexture,warden);
			rA->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rA);
		}
		else if(type == Risen::HOUND)
		{
			rH = dynamic_cast<RisenHound*>(spawnPoints[index]->getRisen()[j]);

			rH->initialize(myGamePtr,32,32,1,&risenHoundTexture,&archerProjectileTexture,warden);
			rH->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rH);
		}
		else if(type == Risen::WARLOCK)
		{
			rLock = dynamic_cast<RisenWarlock*>(spawnPoints[index]->getRisen()[j]);

			rLock->initialize(myGamePtr,32,32,1,&risenWarlockTexture,&warlockProjectileTexture,warden);
			rLock->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rLock);
		}
		//add more
	}
}
//creates a single risen and adds it to the main risen vector of the game
void RisenSpawner::spawnRisen(int type, int index)
{
	RisenWarrior* rW;
	RisenArcher* rA;
	RisenHound* rH;
	RisenWarlock* rLock;

		if(type == Risen::WAR)
		{
			rW = new RisenWarrior();
			rW->initialize(myGamePtr,32,32,1,&risenWarriorTexture,&archerProjectileTexture,warden,tiles);
			rW->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rW);
		}
		else if(type == Risen::ARCHER)
		{
			rA = new RisenArcher();
			rA->initialize(myGamePtr,32,32,1,&risenArcherTexture,&archerProjectileTexture,warden);
			rA->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rA);
		}
		else if(type == Risen::HOUND)
		{
			rH = new RisenHound();
			rH->initialize(myGamePtr,32,32,1,&risenHoundTexture,&archerProjectileTexture,warden);
			rH->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rH);
		}
		else if(type == Risen::WARLOCK)
		{
			rLock = new RisenWarlock();
			rLock->initialize(myGamePtr,32,32,1,&risenWarlockTexture,&warlockProjectileTexture,warden);
			rLock->setPosition(spawnPoints[index]->getPosition());
			risenVector->push_back(rLock);
		}
		
}