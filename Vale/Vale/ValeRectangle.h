#pragma once
#include "ValeGraphics.h"
#include "Circle.h"
class ValeRectangle
{
public:
	ValeRectangle(void);
	ValeRectangle(RECT r);
	ValeRectangle(float x, float y, float w, float l);
	~ValeRectangle(void);


	// getters
	float getWidth(){return width;}
	float getLength(){return length;}
	void getDimensions(float* w, float* l){
		*w = width;
		*l = length;
	}
	D3DXVECTOR2 getPosition(){
		return position;
	}
	void getPosition(float* x, float* y){
		*x = position.x;
		*y = position.y;
	}

	// setters
	void setWidth(float w){width = w;}
	void setLength(float l){length = l;}
	void setDimensions(float w, float l){
		width = w;
		length = l;
	}
	void setRectangle(float x, float y, float w, float l){
		position.x = x;
		position.y = y;
		width = w;
		length = l;
	}
	void setPosition(D3DXVECTOR2 location){
		position = location;
	}
	void setPosition(float x, float y){
		position.x = x;
		position.y = y;
	}

private:
	float width, length;
	D3DXVECTOR2 position;
};

