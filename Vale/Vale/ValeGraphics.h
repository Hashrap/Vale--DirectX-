#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include "gameError.h"
#include "constants.h"

// Color defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

// define d3dx
#define LP_TEXTURE  LPDIRECT3DTEXTURE9
#define LP_SPRITE   LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9
#define VECTOR2     D3DXVECTOR2
#define LP_VERTEXBUFFER LPDIRECT3DVERTEXBUFFER9
#define LP_DXFONT   LPD3DXFONT
#define LP_VERTEXBUFFER LPDIRECT3DVERTEXBUFFER9

namespace graphicsNS
{
    // Some common colors
    // ARGB numbers range from 0 through 255
    // A = Alpha channel (transparency where 255 is opaque)
    // R = Red, G = Green, B = Blue
    const COLOR_ARGB ORANGE  = D3DCOLOR_ARGB(255,255,165,  0);
    const COLOR_ARGB BROWN   = D3DCOLOR_ARGB(255,139, 69, 19);
    const COLOR_ARGB LTGRAY  = D3DCOLOR_ARGB(255,192,192,192);
    const COLOR_ARGB GRAY    = D3DCOLOR_ARGB(255,128,128,128);
    const COLOR_ARGB OLIVE   = D3DCOLOR_ARGB(255,128,128,  0);
    const COLOR_ARGB PURPLE  = D3DCOLOR_ARGB(255,128,  0,128);
    const COLOR_ARGB MAROON  = D3DCOLOR_ARGB(255,128,  0,  0);
    const COLOR_ARGB TEAL    = D3DCOLOR_ARGB(255,  0,128,128);
    const COLOR_ARGB GREEN   = D3DCOLOR_ARGB(255,  0,128,  0);
    const COLOR_ARGB NAVY    = D3DCOLOR_ARGB(255,  0,  0,128);
    const COLOR_ARGB WHITE   = D3DCOLOR_ARGB(255,255,255,255);
    const COLOR_ARGB YELLOW  = D3DCOLOR_ARGB(255,255,255,  0);
    const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255,255,  0,255);
    const COLOR_ARGB RED     = D3DCOLOR_ARGB(255,255,  0,  0);
    const COLOR_ARGB CYAN    = D3DCOLOR_ARGB(255,  0,255,255);
    const COLOR_ARGB LIME    = D3DCOLOR_ARGB(255,  0,255,  0);
    const COLOR_ARGB BLUE    = D3DCOLOR_ARGB(255,  0,  0,255);
    const COLOR_ARGB BLACK   = D3DCOLOR_ARGB(255,  0,  0,  0);
    const COLOR_ARGB FILTER  = D3DCOLOR_ARGB(  0,  0,  0,  0);  // use to specify drawing with colorFilter
    const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB( 64,255,255,255);  // AND with color to get 25% alpha
    const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128,255,255,255);  // AND with color to get 50% alpha
    const COLOR_ARGB BACK_COLOR = BLACK;                         // background color of game

    enum DISPLAY_MODE{TOGGLE, FULLSCREEN, WINDOW};
}

// SpriteData: The properties required by Graphics::drawSprite to draw a sprite
struct SpriteData
{
    int         width;      // width of sprite in pixels
    int         height;     // height of sprite in pixels
    float       x;          // screen location (top left corner of sprite)
    float       y;
    float       scale;      // <1 smaller, >1 bigger
    float       angle;      // rotation angle in radians
    RECT        rect;       // used to select an image from a larger texture
    LP_TEXTURE  texture;    // pointer to texture
    bool        flipHorizontal; // true to flip sprite horizontally (mirror)
    bool        flipVertical;   // true to flip sprite vertically
};
class ValeGraphics
{
private:
	// DirectX 9 stuffs
	LPDIRECT3DDEVICE9	direct3d9Device;
	LPDIRECT3D9			direct3d9;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE		pMode;

	LP_SPRITE sprite;

	HRESULT		result;
	HWND		hWnd;
	bool		fullscreen;
	int			width;
	int			height;
	DWORD		backColor;

	LPD3DXFONT	pFont;
	D3DCOLOR	fontColor;

	void initD3DPresentParams();

private: // Member functions
	bool initDirectX11();
	bool initDirectX9(); //This may not be needed
	
public: // Member functions
	ValeGraphics(void);
	virtual ~ValeGraphics(void);

	void releaseAll();
	void initialize(HWND hw, int width, int height, bool fullscreen);
	HRESULT showBackBuffer();
	bool isAdapterCompatible();
	HRESULT reset();

	HRESULT loadTexture(LPCWSTR filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
	void drawSprite(const SpriteData &spriteData, COLOR_ARGB color = graphicsNS::WHITE);
	void changeDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	LPDIRECT3D9 get3D() { return direct3d9; }
	LPDIRECT3DDEVICE9 get3DDevice() { return direct3d9Device; }
	HDC getDC() { return GetDC(hWnd); }
	HRESULT getDeviceState();
	void setBackColor(DWORD c) {backColor = c;}
	LP_SPRITE getSprite() { return sprite; }

	int getWidth() { return width;}
	int getHeight() { return height;}

	HRESULT beginScene()
	{
		result = E_FAIL;
		if(direct3d9Device == NULL)
			return result;
		direct3d9Device->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0f, 0);
		result = direct3d9Device->BeginScene();
		return result;
	}

	HRESULT endScene()
	{
		result = E_FAIL;
		if(direct3d9Device)
			result = direct3d9Device->EndScene();
		return result;
	}

	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	void spriteEnd()
	{
		sprite->End();
	}

	void DrawTextString(int x, int y, DWORD color, const char *str);
	void DrawTextString(int x, int y, const char *str);
};

