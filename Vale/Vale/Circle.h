#pragma once
#include "ValeGraphics.h"
#include "ValeRectangle.h"
class Circle
{
public:
	Circle(void);
	Circle(float r);
	~Circle(void);

	

	D3DXVECTOR2 getPosition(){
		return position;
	}
	void getPosition(float* x, float* y){
		*x = position.x;
		*y = position.y;
	}
	void setPosition(D3DXVECTOR2 location){
		position = location;
	}
	void setPosition(float x, float y){
		position.x = x;
		position.y = y;
	}
	float getRadius(){return radius;}
	void setRadius(float r){radius = r;}

private:
	float radius;
	D3DXVECTOR2 position;
};

