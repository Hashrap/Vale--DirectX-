//Author: Spencer Corkran

#pragma once

#include "InteractiveObject.h"
#include "Projectile.h"
#include "Warden.h"
#include "Risen.h"
#include "Heart.h"
#include "ValeResources.h"
#include <vector>

#define SCORE_ON_KILL 100
#define SCORE_ON_HIT 20
#define SCORE_ON_DAMAGE 50
#define SCORE_ON_GRAZE 10
#define SCORE_ON_DEATH 1000

///
 // cGrid is essentially a 2D vector of a pointer vectors for collision testing.
 // The outer two vectors correspond to the X and Y coordinates of the grid, and
 // each X/Y cell contains a vector of pointers to objects located in the cell.  This way,
 // Vale knows to only collision check objects in adjacent cells
///

class cGrid
{
private:
	std::vector< std::vector< GridCell > > objGrid;
	std::vector< VECTOR2 > full;
	int dimensions;

	// Access to the player's score for grazing
	int* score;

public:
	//Constructors
	cGrid();
	cGrid(int gridW, int gridH, int d, int* _score);
	~cGrid(void);

	//Member Methods
	void populateGrid(std::vector< InteractiveObject* > objs);
	void depopulateGrid();
	void checkCollisions(InteractiveObject* o);

	//Accessors
	std::vector< std::vector< GridCell > >* getGrid() { return &objGrid; }

	//Helper Methods
	//Generics
	bool testCollision(InteractiveObject* a, InteractiveObject* b);
	void checkNeighbors(VECTOR2 pos, InteractiveObject* o);
	void checkBucket(std::vector<InteractiveObject*> &bucket, InteractiveObject* o);
	void resolveCollisions(InteractiveObject* a, InteractiveObject* b);

	//Specifics
	bool testGraze(Warden* w, Projectile* p);
	bool testWardenvRisenP(Warden* w, Projectile* p);
	bool testRisenvWardenP(Risen* r, Projectile* p);
	void resolveWardenvRisenP(Warden* w, Projectile* p);
	void resolveRisenvWardenP(Risen* r, Projectile* p);
};

