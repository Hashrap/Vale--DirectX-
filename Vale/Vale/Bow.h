#pragma once
#include "equipment.h"
#include "ValeResources.h"

class Warden;

//Author: Alex George
class Bow :
	public Equipment
{
private:
	BowType bowType;

public:
	Bow(void);
	~Bow(void);

	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, BowType type);

	BowType getBowType() { return bowType; }
};

