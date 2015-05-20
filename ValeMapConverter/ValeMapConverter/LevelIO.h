#pragma once

#include <iostream>
#include <fstream>
#include <vector>

enum EntityType{
	MONSTER_SPAWN = 0,
	BOW_SPAWN = 1,
	BOOT_SPAWN = 2
};

class LevelIO
{
private:
	int width, height;
	std::vector<char> tiles;
	int playerX, playerY;
	int numMonsterSpawns, numBowSpawns, numBootSpawns;
	std::vector<int> monsterSpawns;
	std::vector<int> bowSpawns;
	std::vector<int> bootSpawns;

	void grabEntityPositions(int numEntities, EntityType entity, std::fstream &stream);
	void grabEntityPosition(float *x, float *y);
	void clearString(char* string);
public:
	LevelIO(void);
	~LevelIO(void);

	bool loadMap(char* file);
	bool saveMap(char* file);

	int getLength(char* string);
};

