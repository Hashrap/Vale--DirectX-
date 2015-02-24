// Authors: Alex George, Spencer Corkran
#pragma once

#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include "ValeGame.h"
#include "Warden.h"
#include "RisenManager.h"
#include "RisenSpawner.h"
#include "RisenSpawnPoint.h"
#include "RisenWarrior.h"
#include "RisenArcher.h"
#include "RisenHound.h"
#include "RisenWarlock.h"
#include "Bow.h"
#include "Heart.h"
#include "Input.h"
#include "ParticleSystem.h"
#include "cGrid.h"
#include "Bar.h"
#include "DXText.h"
#include <time.h>

#define MAX_WIDTH 1024
#define MAX_HEIGHT 1024
#define GRID_CELL_DIMENSIONS 40

class ValeGame;

class Level
{
private: // Data members
	//Tiles
	// all tiles are squares, tile size of 16 means a 16x16 square
	int tileSize;
	int width, height, gridW, gridH;
	int lastUsedParticleSystem;
	std::vector<int> tiles;
	cGrid collision;

	ValeTexture		grass, road, unwalkable, tree[3], ruins[3];

	ValeGraphics*	graphics;
	Input*			input;	
	ValeGame*	gamePointer;

	std::vector<ValeImage> mapImages;
	ValeTexture wardenTexture, projectileTextures, risenWarriorTexture, risenArcherTexture,
		risenHoundTexture, levelTextures, heartTexture, spawnPointTexture;

	//items
	ValeTexture bowTexture[4];

	//hud
	ValeTexture focusBarFillTexture, focusBarBGTexture, focusBarBGMuseTexture, heartHUDTexture;
	ValeTexture quickShotAbilityTexture[4];
	ValeTexture focusShotAbilityTexture[4];
	ValeTexture fleetFeetAbilityTexture;
	DXText quickCD, focusCD, fleetfeetCD, scoreText;
	RECT scoreRect;
	
    char buffer[20];
	VECTOR2 levelCoords;

	//Lists of obejcts
	Warden				warden;
	RisenManager		risenManager;
	RisenSpawner		risenSpawner;

	//Gameplay stats
	int score;
	int hits;
	int nearMisses;
	int shotsFired;
	int sweetSpots;

	std::vector<Bow*> bows;
	std::vector<Heart*> hearts;

	//HUD
	Bar focusBarFill;
	ValeImage focusBarBG;
	ValeImage focusBarBGMuse;
	ValeImage quickShotAbility[4];
	ValeImage focusShotAbility[4];
	ValeImage fleetFeetAbility;
	ValeImage heartHUD[4];

	ParticleSystem	ps;
	std::vector<ParticleSystem> particleSystems;

public: 
	Level(void);
	~Level(void);

	//Basics
	void initialize(ValeGame *gamePtr);
	void update(float frameTime);
	void updateImagePositions();
	void checkInput();
	void draw();
	void wardenDied();

	//Adding things to level
	void addHeart(int levelX, int levelY);
	void addBow(int tileX, int tileY);
	void addBow(int tileX, int tileY, BowType type);

	//remove things from level
	void removeRisen(int index);

	//Collision
	void checkCollision();
	bool testTerrain(VECTOR2 pos);
	bool testXTerrain(VECTOR2 pos, VECTOR2 oldPos);
	bool testYTerrain(VECTOR2 pos, VECTOR2 oldPos);
	bool nearBow();
	Bow* getFirstBow();

	//Accessors and Mutators
	Warden* getWarden() { return &warden; }
	int getScore() { return score; }
	int getNearMisses() { return nearMisses; }
	int getHits() { return hits; }
	int getSweetSpots() { return sweetSpots; }
	int getShotsFired() { return shotsFired; }

	int getMapTileSize() {return tileSize;}
	void getMaxDimensions(int &width, int &height);
	void setGroundImages(ValeGraphics *g);
	VECTOR2 getLevelCoords() { return levelCoords; }

	void setMap(float x, float y);
	void grabMapCoord(int* tileX, int* tileY, float mouseX, float mouseY);

	bool loadMap(char* file, ValeGame *gp);
	bool saveMap(char* file);

	void addTimedSpawn(ValeGame* gamePtr,Risen::types type,float activeTime,D3DXVECTOR2 position,float activationDistance);
	void addStaticSpawn(ValeGame* gamePtr,std::vector<Risen::types>,std::vector<int> amount,D3DXVECTOR2 position,float activationDistance);
};

