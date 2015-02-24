#include "StdAfx.h"
#include "Risen.h"

//Author:Bradley Lubahn
Risen::Risen(void):LivingEntity(){	}


Risen::~Risen(void){ }

void Risen::initialize(ValeGame *gamePtr, int width, int height, int ncols, ValeTexture *textureM, ValeTexture *projText)
{
	deathSound = "risendeath";
	faction = RISEN;
	LivingEntity::initialize(gamePtr, width, height, ncols, textureM, projText, faction);	
}

void Risen::update(float frameTime)
{
	if(currentState != FLEE)
	{
		facePlayer();
	}
	else
	{
		turnFromPlayer();
	}

	LivingEntity::update(frameTime);
	this->updateImagePosition(position.x,position.y);
}

//Basic Reynolds seek for moving closer to the player
D3DXVECTOR2 Risen::seek(D3DXVECTOR2 target)
{
	
	D3DXVECTOR2 desVel;
	desVel = target - position;
	desVel = normalize(desVel) * actualMovementSpeed();
		
	//velocity = desTarget;
	return desVel;
}
//Run Away from Warden When Low on Health
D3DXVECTOR2 Risen::flee(D3DXVECTOR2 target)
{
	D3DXVECTOR2 desVel;
	desVel= position- target;
	desVel = normalize(desVel) *  actualMovementSpeed();

	return desVel;
}
void Risen::avoidWalls()
{
	int height;
	int tileSize = 128;
	int width= 0;
	int temp =0;
	int tempX = 0;
	int tempY= 0;

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			std::cout << tiles[(i*height)+j];
			temp++;
			if(tiles[(j*width)+i] != 2)
			{
				//if(distanceToTarget());
			}
			if(temp >= width)
			{
				std::cout << std::endl;
			    temp = 0;
			}
		}
	}
}
//Simple avoid function to move away from walls, currently not being used
D3DXVECTOR2 Risen::avoid(D3DXVECTOR2 obstaclePos,float obstacleRadius,float safeDistance)
{
	D3DXVECTOR2 desVel;
	D3DXVECTOR2 vectorToObstacleCenter= obstaclePos - position;
	float distance = magnitude(vectorToObstacleCenter);
	D3DXVECTOR2 right = perpRight(position);

	if(distance - obstacleRadius > safeDistance)
		return D3DXVECTOR2(0,0);
	// if object behind me return zero vector
	forward = normalize(velocity);
	

	if(dotProduct(vectorToObstacleCenter,forward) < 0)
		return D3DXVECTOR2(0,0);

	float rightDotVTOC = dotProduct(vectorToObstacleCenter,right);

	if(abs(rightDotVTOC) > obstacleRadius + radius)
		return D3DXVECTOR2(0,0);

	if(rightDotVTOC < 0)
	{
		desVel = right * actualMovementSpeed();
	}
	else
	{
		desVel = right * -actualMovementSpeed();
	}

	desVel = desVel * safeDistance/(distance + 0.1);
	return desVel;
}
//Rotate the Risen the face the player
void Risen::facePlayer()
{
	//Rotate towards player	
	float deltaX =  (player->getPosition().x + player->getImage().getWidth()) - ( position.x + image.getWidth());
	float deltaY = (player->getPosition().y + player->getImage().getHeight()) - ( position.y + image.getHeight());

	rotation = atan2(deltaY, deltaX) * 180.0 / PI;
	image.setDegrees(rotation);
}

//Rotate the Risen away from the player
void Risen::turnFromPlayer()
{
	//Rotate towards player	
	float deltaX =  (player->getPosition().x + player->getImage().getWidth()) - ( position.x + image.getWidth());
	float deltaY = (player->getPosition().y + player->getImage().getHeight()) - ( position.y + image.getHeight());

	rotation = atan2(deltaY, deltaX) * 180.0 / PI;
	image.setDegrees(rotation-180);
}

//Simple Math functions for movement
float Risen::magnitude(D3DXVECTOR2 vec)
{
	return sqrt( (vec.x*vec.x) + (vec.y*vec.y) );
}
float Risen::dotProduct(D3DXVECTOR2 v1, D3DXVECTOR2 v2)
{
	return ((v1.x * v2.x )+ (v1.y *v2.y));
}
D3DXVECTOR2 Risen::perpRight(D3DXVECTOR2 vector)
{
	return D3DXVECTOR2(-vector.y,vector.x);
}
D3DXVECTOR2 Risen::separate(float safe,std::vector<Risen*> risen)
{
			
			D3DXVECTOR2 desVel = D3DXVECTOR2();
			float _safeDistance = safe;
			float tempDistance = 0;
			
			std::vector<Risen*> tempRisen = risen; 
			std::vector<float> distanceVector;
			std::vector<float>  indexVector;
			
			for(unsigned int i = 0; i < tempRisen.size(); i++)
			{
				if(this != tempRisen[i])
				{
					tempDistance = distanceToTarget(tempRisen[i]->getPosition());
					if(tempDistance < _safeDistance)
					{
						distanceVector.push_back(tempDistance);
						indexVector.push_back(i);
					}
				}
				
			}
			
			D3DXVECTOR2 tempVel;
			for(unsigned int j = 0; j < indexVector.size(); j++)
			{
				desVel = flee(tempRisen[indexVector[j]]->getPosition());
				
				//desVel = desVel+ (tempVel *( 1/(distanceVector[j]+0.000001) ) ) ;
			}
			desVel = normalize(desVel);
			
			desVel = desVel * actualMovementSpeed();
			//desVel = desVel-velocity;
			
			
			return desVel;
}