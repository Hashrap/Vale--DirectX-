#include "StdAfx.h"
#include "ValeRectangle.h"


ValeRectangle::ValeRectangle(void)
{
}

ValeRectangle::ValeRectangle(RECT r)
{
	setRectangle(r.left, r.top, r.right - r.left, r.bottom - r.top);
}

ValeRectangle::ValeRectangle(float x, float y, float w, float l)
{
	setRectangle(x,y,w,l);
}

ValeRectangle::~ValeRectangle(void)
{
}

