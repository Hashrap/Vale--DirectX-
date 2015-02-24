#include "StdAfx.h"
#include "RisenManager.h"
#include <iostream>

//Author:Bradley Lubahn
//Runs the update functionality for all of the risen and Contains the levels vector of risen

RisenManager::RisenManager(void)
{
}


RisenManager::~RisenManager(void)
{
}

void RisenManager::init(int* _score)
{
	// Give the manager the ability to change player score
	score = _score;
}
//Calls the update function for all of the risen, and checks to see if they are still alive,calling remove if they are not
void RisenManager::update(float frameTime)
{
	//Risen update loop, goes through checks the type to avoid casting errors
	//casts it to that type and calls the specific update of the risen
	for(int i=0, count=risen.size(); i < count; i++)
	{
		//Warrior
		if(risen[i]->type == Risen::WAR){
			RisenWarrior* r = dynamic_cast<RisenWarrior*>(risen[i]);
			r->update(frameTime,&risen);
		}
		//Archer
		if(risen[i]->type == Risen::ARCHER){
			RisenArcher* rA = dynamic_cast<RisenArcher*>(risen[i]);
			rA->update(frameTime,&risen);
		}
		//Hound
		if(risen[i]->type == Risen::HOUND){
			RisenHound* rH = dynamic_cast<RisenHound*>(risen[i]);
			rH->update(frameTime,&risen);
		}
		//Warlock
		if(risen[i]->type == Risen::WARLOCK){
			RisenWarlock* rSwag = dynamic_cast<RisenWarlock*>(risen[i]);
			rSwag->update(frameTime);
		}
		//add more here

		
	}
}
void RisenManager::removeRisen(int index)
{
	risen.erase(risen.begin()+index);
	*score += 500;
	std::cout << "KILL: " << *score << std::endl;
}
void RisenManager::draw()
{
	for(int i=0, count= risen.size(); i < count; i++)
	{
		risen[i]->draw();
	}
}
void RisenManager::updateImagePosition(D3DXVECTOR2 levelCoords)
{
	for(int i=0, count=risen.size(); i < count; i++)
	{
		risen[i]->updateImagePosition(levelCoords.x, levelCoords.y);
		risen[i]->updateProjectiles(levelCoords);
	}
}