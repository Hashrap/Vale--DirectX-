#pragma once

#include "LivingEntity.h"
#include "Warden.h"



//Author:Bradley Lubahn
class Risen :
	public LivingEntity
{
public:
	Risen(void);
	~Risen(void);

	void initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, ValeTexture *projText);

	void update(float frameTime);
	void beSpawned();
	//void takeDamage(float damage){currHealth -= damage;};
	bool isDead(){return currHealth <= 0;};
	void setWarden(Warden* p);
	
	Warden* player;
	ValeGame* gamePtr;
	enum types{WAR,ARCHER,HOUND,WARLOCK};
	int type;
protected:
	//attributes
	float seekDistance;
	float attackDistance;
	float fleeHealth;
	float fleeDistance;
	bool shielded;
	bool canSeeWarden;
	enum behavior {MOVE, ATTACK, SEEK, FLEE, IDLE, DODGE,RANGE_ATTACK,CHARGE_ATTACK,CAST,CHANNEL};	
	D3DXVECTOR2 desTarget;
	D3DXVECTOR2 forward;
	std::vector<int> tiles;
	
	int currentState;
	int previousState;
	//methods
	virtual D3DXVECTOR2 seek(D3DXVECTOR2 target);
	virtual D3DXVECTOR2 flee(D3DXVECTOR2 target);
	void facePlayer();
	void turnFromPlayer();
	D3DXVECTOR2 avoid(D3DXVECTOR2 obstaclePos,float obstacleRadius,float safeDistance);
	void avoidWalls();
	float magnitude(D3DXVECTOR2 vec);
	float dotProduct(D3DXVECTOR2 v1, D3DXVECTOR2 v2);
	D3DXVECTOR2 perpRight(D3DXVECTOR2 vector);
	D3DXVECTOR2 separate(float safe, std::vector<Risen*> risen);
};
