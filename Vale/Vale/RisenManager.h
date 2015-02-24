#pragma once
#include "Risen.h"
#include "RisenWarrior.h"
#include "RisenArcher.h"
#include "RisenHound.h"
#include "RisenWarlock.h"
#include <vector>



class RisenManager
{
public:
	RisenManager(void);
	~RisenManager(void);
	void init(int* _score);
	void update(float frameTime);
	void draw();
	void checkCollisions();
	void updateImagePosition(D3DXVECTOR2 levelCoords);
	void removeRisen(int index);
	void addRisen(Risen::types);

	std::vector<Risen*> risen;
	int* score;
};

