#include "StdAfx.h"
#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(void)
{
	max_particles = 100;
	numParticlesAlive = 0;
	bIsActive = false;
	bIsImageInitialized = false;
	emitterType = EMITTER_TYPE_DEFAULT;
	emitterShape = EMITTER_SHAPE_NONE;
}


ParticleSystem::~ParticleSystem(void)
{
}

void ParticleSystem::initialize(ValeGraphics* g, LPCWSTR file)
{
	//keep the pointer
	graphics = g;
	//keep location of texture
	fileLocation = file;

	// setup textures with default smoke
	for(int i = 0; i < 8; i++)
	{
		textures[i].initialize(g, fileLocation);
	}
	// give each particle that is made its own image
	for(int i = 0; i < max_particles; i++)
	{
		ValeImage vi;
		vi.initialize(g, 128, 128, 4, &textures[0]);
		vi.setAnimationComplete(false);
		vi.setCurrentFrame(0);
		vi.setLoop(true);
		vi.setFrames(0, 15);
		vi.setScale(0.25f);
		vi.setFrameDelay(0.5f/16);
		vi.setColorFilter(graphicsNS::MAGENTA);
		images.push_back(vi);
	}
	// images are now initialized, safe to draw
	bIsImageInitialized = true;
}

// returns the success of the operation
bool ParticleSystem::setTextureAtPosition(int pos, ValeTexture *t, int numCols)
{
	if(pos < 0 || pos > 7)
		return false;

	textures[pos] = *t;
	return true;
}

bool ParticleSystem::setTextureAtPosition(int pos, char* file)
{
	if(pos < 0 || pos > 7)
		return false;

	bool result;
	result = textures[pos].initialize(graphics, (LPCWSTR)file);
}

void ParticleSystem::update(float frametime)
{
	// we are not active or if we do not have an image, do nothing
	if(!bIsActive || !bIsImageInitialized)
		return;

	// make sure we have enough particles
	if(numParticlesAlive < max_particles)
	{
		// make new particle
		Particle p;
		p.initialize(&images[numParticlesAlive], position, 100.0f, 0.0f, SETCOLOR_ARGB(255, 255, 255, 255), SETCOLOR_ARGB(0,0,0,0));
		numParticlesAlive++;
		particles.push_back(p);
	}
	for(unsigned int i = 0; i < particles.size(); i++)
	{
		particles[i].update(frametime);
	}
}

void ParticleSystem::draw()
{
	for(unsigned int i = 0; i < particles.size(); i++)
	{
		particles[i].draw();
	}
}

void ParticleSystem::updateImagePosition(float x, float y)
{
	for(unsigned int i = 0; i < particles.size(); i++)
	{
		particles[i].updateImagePosition(x, y);
	}
}

void ParticleSystem::activate(void)
{
	if(!bIsActive)
		bIsActive = true;

	if(numParticlesAlive > 0)
	{
		for(int i = 0; i < numParticlesAlive; i++)
		{
			particles[i].resurrect();
		}
	}
}

// Stops particle creation and lets any particles currently
// alive finish their lives before being removed
void ParticleSystem::deactivate(void)
{
	if(bIsActive)
		bIsActive = false;

	// call graceful cleanup on all particles
}

// stops particle generation and instantly removes all particles
// useful for any systems offscreen
void ParticleSystem::stop(void)
{
	if(bIsActive)
		bIsActive = false;

	// kill all particles
}

void ParticleSystem::setPosition(D3DXVECTOR2 pos)
{
	position = pos;
	if(numParticlesAlive > 0)
	{
		for(int i = 0; i < numParticlesAlive; i++)
		{
			particles[i].setPosition(position);
		}
	}
}