#pragma once
#include "InteractiveObject.h"

//Author: Alex George
class Equipment :
	public InteractiveObject
{
protected:
	bool equipped;
	//Warden *owner;
public:
	Equipment();
	~Equipment();

	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM);
	void draw();

	//override, but make sure to call parent
	virtual void equip();
	virtual void unequip();

	bool isEquipped() { return equipped; }
};

