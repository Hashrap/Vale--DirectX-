// Ryan Caso
// Vale

#pragma once

#include "ValeImage.h"
#include <stdlib.h>
#include <time.h>

class Particle
{
public:
	Particle(void);
	Particle(ValeImage *img, RECT loc, float scl, float startAge, DWORD beginingColor, DWORD endingColor);
	~Particle(void);

	void initialize(ValeImage *img, D3DXVECTOR2 pos, float scl, float startAge, DWORD beginingColor, DWORD endingColor);
	void update(float frametime);
	void draw(void);
	
	void setLifespan(float lifeSpan);
	void setPSPosition(D3DXVECTOR2 newPSPos){psPosition = newPSPos;}
	void setPosition(D3DXVECTOR2 pos){position = pos;}

	void updateImagePosition(int x, int y);
	void resurrect(){bIsAlive = true;}

	// move this out of here into a common math file soon
	void normalize(D3DXVECTOR2 *vector);

private:
	float age;
	float lifespan;
	float p;
	float scale;
	float spread;

	bool bIsAlive;

	D3DXVECTOR2 psPosition;
	D3DXVECTOR2 position;

	ValeImage *image;
	RECT location;

	DWORD startColor;
	DWORD endColor;
};

