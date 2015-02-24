#pragma once
#include "valeimage.h"
#include "constants.h"
#include "valetexture.h"
namespace dashboardNS
{
    const int   IMAGE_SIZE = 128;       // each texture size
    const int   TEXTURE_COLS = 8;       // texture columns
    const int   BAR_FRAME = 32;         // the frame number of the bar
}

class Bar : public ValeImage
{
    public:
    // Initialize the Bar 
    // Pre: *graphics = pointer to Graphics object
    //      *textureM = pointer to TextureManager object
    //      left, top = screen location
    //      scale = scaling (zoom) amount
    //      color = color of bar
    bool initialize(ValeGraphics *graphics, ValeTexture *textureM, int w, int h, int left, int top,
                    float scale, COLOR_ARGB color);
    // Set Bar Size
    void set(float percentOn);

	void setPosition(int left, int top);

    // Override update so setRect() is not called.
    //virtual void update(float frameTime)    {}
};