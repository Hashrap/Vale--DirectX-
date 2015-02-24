// Ryan Caso
// Vale

#include "StdAfx.h"
#include "Particle.h"


Particle::Particle(void){}


Particle::Particle(ValeImage *img, RECT loc, float scl, float startAge, DWORD beginingColor, DWORD endingColor)
{
	image = img;
	location = loc;
	scale = scl;
	age = startAge;
	startColor = beginingColor;
	endColor = endingColor;
}


Particle::~Particle(void){}


void Particle::initialize(ValeImage *img, D3DXVECTOR2 pos, float scl, float startAge, DWORD beginingColor, DWORD endingColor)
{
	image = img;
	scale = scl;
	age = startAge;
	startColor = beginingColor;
	endColor = endingColor;
	p = 0.0f;
	psPosition = pos;
	position = pos;
	srand(time(NULL));
	image->setX(position.x);
	image->setY(position.y);
	spread = 1.5f;
	bIsAlive = true;
}

void Particle::update(float frametime)
{
	if(!bIsAlive)
	{
		return;
	}
	age += frametime;
	if(age >= 0.5f)
	{
		// reset age
		age = 0.0f;

		//move paticle back to particleSystem position
		position = psPosition;

		// reset image position
		image->setX(position.x);
		image->setY(position.y);
		bIsAlive = false;
	}
	
	// make a random direction vector
	D3DXVECTOR2 dir;

	dir.x = (2*(float)rand()/((float)RAND_MAX))-1;
	dir.y = (2*(float)rand()/((float)RAND_MAX))-1;
	
	// normalize it
	normalize(&dir);

	//incorporate the spread value of the particle
	dir.x *= spread;
	dir.y *= spread;

	// add random direction to current position
	position.x += dir.x;
	position.y += dir.y;
	
	image->setX(position.x);
	image->setY(position.y);

	// update our image
	image->update(frametime);
}

void Particle::draw()
{
	if(!bIsAlive)
	{
		return;
	}
	image->draw(startColor);
}

// Move to common math file
void Particle::normalize(D3DXVECTOR2 *vector)
{
	float lengthsq, x, y;
	x = vector->x;
	y = vector->y;

	lengthsq = x*x + y*y;
	if(lengthsq <= 0.0000001f)
	{
		vector->x = 0.0f;
		vector->y = 0.0f;
	}
	else
	{
		float component = 1/(sqrtf(lengthsq));
		vector->x = x * component;
		vector->y = y * component;
	}
}

//Sets the object's image's position to the correct screen coordinates using the given level coordinates
void Particle::updateImagePosition(int x, int y)
{
	image->setX(position.x-x);
	image->setY(position.y-y);
}