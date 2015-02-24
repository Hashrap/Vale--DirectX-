#pragma once
#include "InteractiveObject.h"
#include <vector>
#include "Risen.h"
#include "RisenSpawnPoint.h"
#include "RisenWarrior.h"
#include "RisenArcher.h"
#include "RisenHound.h"
#include "RisenWarlock.h"

class RisenSpawner :
	public InteractiveObject
{
public:
	RisenSpawner(void);
	~RisenSpawner(void);

	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,
		std::vector<Risen*>* risen,Warden* ward,std::vector<int> tilePtr);	
	void update(float frameTime);
	
	void addSpawnPoint(RisenSpawnPoint* addPoint);
	std::vector<RisenSpawnPoint*> getSpawnPoints(){return spawnPoints;};
	void setSpawnTime(float time){spawningTime = time;};
private:
	//pointers from the game for initializing the risen
	ValeGame *myGamePtr;
	Warden* warden;
	ValeTexture risenWarriorTexture, risenArcherTexture, risenHoundTexture, risenWarlockTexture, archerProjectileTexture,
		warlockProjectileTexture;
	//Vector of spawn points and access to the Risen vector from the game
	std::vector<Risen*>* risenVector;
	std::vector<RisenSpawnPoint*> spawnPoints; 
	std::vector<int> tiles;

	int activeIndex;
	void spawnRisen(int type,int index);
	void spawnVectorOfRisen(int index);
	float spawningTime;
	float spawnInterval;
};

