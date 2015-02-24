// Author: Spencer Corkran

#include "StdAfx.h"
#include "cGrid.h"
#include <algorithm>

cGrid::cGrid()
{

}

cGrid::cGrid(int gridW, int gridH, int d, int* _score):dimensions(d), score(_score)
{
	for(int i = 0; i < gridW; i++)
	{
		objGrid.push_back( std::vector< GridCell >() );
		for(int j = 0; j < gridH; j++)
		{
			objGrid[i].push_back(GridCell());
		}
	}
}


cGrid::~cGrid(void)
{
}

// Every frame, populateGrid() iterates through the list of game objects and
// adds a pointer to them to whichever cell their position occupies.  Additionally,
// each object is given a pointer to the cell it's held in for easier access.
void cGrid::populateGrid(std::vector< InteractiveObject* > objs)
{
	VECTOR2 oCell, gCell;
	for(unsigned int i = 0; i < objs.size(); i++)
	{
		// Find which cell the object used to occupy
		oCell = objs[i]->getCell();

		// Find which cell the object occupies
		gCell = objs[i]->getCenter();
		gCell.x = (int)(gCell.x / dimensions);
		gCell.y = (int)(gCell.y / dimensions);

		// Add object to cell (renewed every update)
		full.push_back(gCell);
		objs[i]->setCell((int)gCell.x, (int)gCell.y);
		objGrid[gCell.x][gCell.y].bucket.push_back(objs[i]);

		// Add object to cell (renewed when needed)
		/*if(oCell !=  gCell)
		{
			objs[i]->setCell(gCell.x, gCell.y);
			objs[i]->setCellPtr(&(objGrid[gCell.x][gCell.y]));
		}*/	// Else: we're still in the same cell, nothing to be done
	}
}

// When renewing the grid every frame, this function clears it after
// collision calculations have been made.  This ensures no nasty pointers
// are left dangling.
void cGrid::depopulateGrid()
{
	for(unsigned int i = 0; i < full.size(); i++)
	{
		objGrid[full[i].x][full[i].y].bucket.clear();
	}
	full.clear();
}

void cGrid::checkNeighbors(VECTOR2 pos, InteractiveObject* o)
{
		//Check left and right
	for(int a = -1; a < 2; a++)
	{
		//Check up and down
		for(int b = -1; b < 2; b++)
		{
			//Don't check invalid cells like (-1,-1)
			if(pos.x+a >= 0 && pos.y+b >= 0)
			{
				checkBucket(objGrid[pos.x+a][pos.y+b].bucket, o);
			}
		}
	}
}

void cGrid::checkBucket(std::vector<InteractiveObject*> &bucket, InteractiveObject* o)
{
	//Grab all objects in the cell
	for(unsigned int i = 0; i < bucket.size(); i++)
	{
		InteractiveObject * other = bucket[i];

		if(o != other)
		{
			// Warden
			if(o->getFaction() == PLAYER)
			{
				Warden* warden = dynamic_cast<Warden*>(o);

				// Warden v Projectile
				if(other->getName().compare("Projectile") == 0)
				{
					Projectile* projectile = dynamic_cast<Projectile*>(other);
					if(projectile->getFaction() == RISEN)
					{
						if(testGraze(warden, projectile) && !projectile->beenGrazed())
						{
							projectile ->graze();
							*score += SCORE_ON_GRAZE;
							//std::cout << "GRAZE: " << *score << std::endl;
						}
						if(testCollision(warden, projectile))
						{
							resolveWardenvRisenP(warden, projectile);
						}
					}
				}

				//Warden v Heart
				if((other->getName()).compare("Heart") == 0 && o->getFaction() == PLAYER)
				{
					//std::cout << "heart + player";
					Warden* warden = dynamic_cast<Warden*>(o);
					Heart* heart = dynamic_cast<Heart*>(other);
					heart->heal(warden);
				}
			}

			// Risen
			if(o->getFaction() == RISEN)
			{
				Risen* risen = dynamic_cast<Risen*>(o);
				if(other->getName().compare("Projectile") == 0)
				{
					Projectile* projectile = dynamic_cast<Projectile*>(other);
					if(projectile->getFaction() == PLAYER)
					{
						if(testCollision(risen, projectile))
						{
							resolveRisenvWardenP(risen, projectile);
						}
					}
				}
			}
		}
	}
}

// Checks cells adjacent to the passed object for collisions
void cGrid::checkCollisions(InteractiveObject* o)
{
	VECTOR2 pos = o->getCell();
	// Test 9 cells in this order:
	// Northwest, North, Northeast 
	// West, Center, East,
	// Southwest, South, Southest
	checkNeighbors(pos, o);
}

// Simple primitive test for testing collisions between 2 circle-based objects
bool cGrid::testCollision(InteractiveObject* a, InteractiveObject* b)
{
	VECTOR2 dist = a->getCenter() - b->getCenter();
	dist.x = dist.x*dist.x;
	dist.y = dist.y*dist.y;
	float dist2 = dist.x+dist.y;
	float radiusSum = a->getRadius() + b->getRadius();
	return dist2 <= radiusSum * radiusSum;
}

bool cGrid::testGraze(Warden* w, Projectile* p)
{
	VECTOR2 dist = w->getCenter() - p->getCenter();
	dist.x = dist.x*dist.x;
	dist.y = dist.y*dist.y;
	float dist2 = dist.x+dist.y;
	float radiusSum = (w->getRadius()*2) + (p->getRadius());
	return dist2 <= radiusSum * radiusSum;
}

bool cGrid::testWardenvRisenP(Warden* w, Projectile* p)
{
	// Not implemented yet
	return false;
}

bool cGrid::testRisenvWardenP(Risen* r, Projectile* p)
{
	// Not implemented yet
	return false;
}

void cGrid::resolveCollisions(InteractiveObject* a, InteractiveObject* b)
{
	// Not implemented yet
}

void cGrid::resolveWardenvRisenP(Warden* warden, Projectile* projectile)
{
	*score -= SCORE_ON_DAMAGE;
	//delete if not piercing
	if(!projectile->isPiercing())
	{
		warden->takeDamage();
		projectile->getManager()->killProjectile(projectile);
	}
	else
	{
		//if it's piercing, make sure we only damage it once
		if(!projectile->hasPierced(warden))
		{
			warden->takeDamage();
		}
		projectile->pierceTarget(warden);
	}	
}

void cGrid::resolveRisenvWardenP(Risen* risen, Projectile* projectile)
{
	*score += SCORE_ON_HIT;
	//delete if not piercing
	if(!projectile->isPiercing())
	{
		risen->takeDamage(projectile->getDamage());
		projectile->getManager()->killProjectile(projectile);
	}
	else
	{
		//if it's piercing, make sure we only damage it once
		if(!projectile->hasPierced(risen))
		{
			risen->takeDamage(projectile->getDamage());
			*score += projectile->pierceTarget(risen);
		}
	}	
}