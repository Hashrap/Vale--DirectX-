#pragma once
#include "interactiveobject.h"
#include "Risen.h"
#include "RisenWarrior.h"
#include "RisenArcher.h"
#include "RisenHound.h"
#include "RisenWarlock.h"
//Author:Bradley Lubahn

namespace risenSpawnNS{
	const int SPAWN_INTERVAL = 5;
	const int SPAWN_TIME = 30;
	const float SPAWN_DISTANCE =200000;//
}

class RisenSpawnPoint :
	public InteractiveObject
{
public:
	RisenSpawnPoint(void);
	~RisenSpawnPoint(void);
	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM,float distanceToActivate);
	//sets it so that timed is true, and will summon the given type of risen until the time runs out
	//based on the spawn interval
	void setTimed(Risen::types type, float time);
	//adds the risen type to the current vector of risen, and how many you want
	//This will set the timed spawner to be false, and it will spawn all of the
	//risen that are in the vector when the warden gets to the trigger
	void createRisen(Risen::types type, int amount);
	//trigger spawn will continue spawning a specific type of enemy until the trigger is hit
	//aka the warden causes the spawn points to shut off
	void setTriggerSpawn(Risen::types type);
	bool getTrigger(){return active;};
	void turnOnAndOff();
	bool spawnedFirstEnemy;
	//Types of Spawns
	enum spawnTypes{TIMED,TRIGGER,STATIC,INACTIVE};
	void update(float frameTime);
	//gets for the type of spawn point this is going to be
	int getSpawnType();
	int getUnitSpawnType(){return spawnType;};
	std::vector<Risen*> getRisen(){return risen;};
	
	
	//Accessors
	bool getVisited(){return visited;}
	bool getActive(){return spawning;};
	float getSpawnDistance(){return spawnDistance;};
	float getSpawnInterval(){return spawnInterval;};
	float getSpawnTime(){return spawnTimer;};
	float getCurrentSpawnTime(){return currentSpawnTime;}
	float getCurrentSpawnInterval(){return currentSpawnInterval;}
	//Mutators
	void setVisited(bool yes){visited = yes;}
	void setActive(bool active){spawning = active;}
	void setSpawnTime(float duration){spawnTimer=duration;};
	void setSpawnInterval(float inter){spawnInterval=inter;};
	void setSpawnDistance(float distance){spawnDistance=distance;};
	void setCurrentSpawnTime(float time){currentSpawnTime = time;};
	void setCurrentSpawnInterval(float time){currentSpawnInterval = time;};
private:
	//Timed Variables
	bool isTimedSpawn;
	int spawnType;
	//Trigger Variables
	bool triggerSpawn;
	bool active;
	//Static Varibles
	bool staticSpawn;
	std::vector<Risen*> risen;
	//Common Properties
	bool spawning;
	bool visited;
	float spawnDistance;
	float spawnInterval;
	float spawnTimer;
	
	float currentSpawnTime;
	float currentSpawnInterval;
};

