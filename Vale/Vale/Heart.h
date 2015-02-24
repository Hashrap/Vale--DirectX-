#pragma once
#include "Warden.h"

namespace heartNS
{
	const int SIZE = 24;
	const int HEALTH_RESTORED = 1;
}

//Author: Alex George
//Represents a consumable heart that drops from enemies. Heals the warden when walked over
class Heart :
	public InteractiveObject
{

private:
	bool consumed;

public:
	Heart(void);
	~Heart(void);

	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM);
	bool heal(Warden *warden);
	void draw();
	
	bool isConsumed(){ return consumed; }

};

