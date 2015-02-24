#pragma once

#include "ValeGraphics.h"
#include "ValeTexture.h"
#include "ValeImage.h"
#include "Particle.h"
#include <vector>
#include <string>

enum EmitterShape{
	EMITTER_SHAPE_NONE = 0,
	EMITTER_SHAPE_CIRCLE = 1,
	EMITTER_SHAPE_RECTANGLE = 2,
	EMITTER_SHAPE_CONE = 3
};

enum EmitterType{
	EMITTER_TYPE_DEFAULT = 0,
	EMITTER_TYPE_SINGLE_BURST = 1,
	EMITTER_TYPE_PULSE = 2,
};

class ParticleSystem
{
public:
	ParticleSystem(void);
	~ParticleSystem(void);
	
	// public memeber funcitons
	void initialize(ValeGraphics* g, LPCWSTR file);
	void update(float frametime);
	void activate(void);
	void deactivate(void);
	void stop(void);
	//void moveWithMap(float x, float y);
	void draw(void);
	void updateImagePosition(float x, float y);

	// setters
	void setEmitterShape(EmitterShape eShape){emitterShape = eShape;}
	void setEmitterType(EmitterType eType){emitterType = eType;}
	void setPosition(D3DXVECTOR2 pos);
	bool setTextureAtPosition(int pos, ValeTexture *t, int numCols);
	bool setTextureAtPosition(int pos, char* file);

	// public memeber variables
	int max_particles;
	int numParticlesAlive;
	bool bIsActive;

private:
	//pointer to the graphics device
	ValeGraphics *graphics;

	std::vector<Particle> particles;
	std::vector<ValeImage> images;
	//std::vector<ValeTexture> textures;

	ValeImage image;
	bool bIsImageInitialized;
	EmitterShape emitterShape;
	EmitterType emitterType;
	D3DXVECTOR2 position;
	char fileLoc[64];
	LPCWSTR fileLocation;
	ValeTexture textures[8];
};