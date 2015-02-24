#include "StdAfx.h"
#include "ValeGraphics.h"


ValeGraphics::ValeGraphics(void)
{
	direct3d9 = NULL;
	direct3d9Device = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
	sprite = NULL;
	backColor = SETCOLOR_ARGB(255, 0,0,0);
	fontColor = D3DCOLOR_ARGB(255, 255, 0,0);
}


ValeGraphics::~ValeGraphics(void)
{
	releaseAll();
}

void ValeGraphics::initialize(HWND hwnd, int w, int h, bool fullScreen)
{
	hWnd = hwnd;
	width = w;
	height = h;
	fullscreen = fullScreen;

	initDirectX9();
}

// Initialize Directx9 and the Device
bool ValeGraphics::initDirectX9()
{
	direct3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(direct3d9 == NULL){
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D 9."));
	}

	initD3DPresentParams();

	// tries to setup fullscreen
	if(fullscreen)
	{
		if(isAdapterCompatible()){
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		}else{
			throw(GameError(gameErrorNS::FATAL_ERROR, "The Graphics device does not support the specified resolution and/or format."));
		}
	}

	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	result = direct3d9->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
		hWnd, behavior, &d3dpp, &direct3d9Device);

	if(FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D 9 device."));

	result = D3DXCreateSprite(direct3d9Device, &sprite);
	if(FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));

	D3DXCreateFontA(direct3d9Device, 32, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, (LPCSTR)TEXT("Arial"), &pFont);
	
	return false;
}

// Initializes the Direct3D presentation parameters
void ValeGraphics::initD3DPresentParams()
{
	try{
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if(fullscreen)
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing D3D9 presentation parameters."));
	}
}

HRESULT ValeGraphics::showBackBuffer()
{
	result = E_FAIL;
	result = direct3d9Device->Present(NULL, NULL, NULL, NULL);
	return result;
}

//=============================================================================
// Load the texture into default D3D memory (normal texture use)
// For internal engine use only. Use the TextureManager class to load game textures.
// Pre: filename is name of texture file.
//      transcolor is transparent color
// Post: width and height = size of texture
//       texture points to texture
// Returns HRESULT
//=============================================================================
HRESULT ValeGraphics::loadTexture(LPCWSTR filename, COLOR_ARGB transcolor,
                                UINT &width, UINT &height, LP_TEXTURE &texture)
{
    // The struct for reading file info
    D3DXIMAGE_INFO info;
    result = E_FAIL;

    try{
        if(filename == NULL)
        {
            texture = NULL;
            return D3DERR_INVALIDCALL;
        }
    
        // Get width and height from file
        result = D3DXGetImageInfoFromFile(filename, &info);
        if (result != D3D_OK)
            return result;
        width = info.Width;
        height = info.Height;
    
        // Create the new texture by loading from file
        result = D3DXCreateTextureFromFileEx( 
			direct3d9Device,    //3D device
            filename,           //image filename
            info.Width,         //texture width
            info.Height,        //texture height
            1,                  //mip-map levels (1 for no chain)
            0,                  //usage
			D3DFMT_UNKNOWN,     //surface format (default)
            D3DPOOL_DEFAULT,    //memory class for the texture
            D3DX_DEFAULT,       //image filter
            D3DX_DEFAULT,       //mip filter
            transcolor,         //color key for transparency
            &info,              //bitmap file info (from loaded file)
            NULL,               //color palette
            &texture );         //destination texture

    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
    }
    return result;
}

void ValeGraphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if(spriteData.texture == NULL)
		return;

	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width/2*spriteData.scale),
											(float)(spriteData.height/2*spriteData.scale));

	D3DXVECTOR2 translate = D3DXVECTOR2((float)(spriteData.x), (float)(spriteData.y));

	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	if(spriteData.flipHorizontal)
	{
		scaling.x *= -1;
		spriteCenter.x -= (float)(spriteData.width*spriteData.scale);
		translate.x += (float)(spriteData.width*spriteData.scale);
	}
	if(spriteData.flipVertical)
	{
		scaling.y *= -1;
		spriteCenter.y -= (float)(spriteData.height*spriteData.scale);
		translate.y += (float)(spriteData.height*spriteData.scale);
	}

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, &scaling, &spriteCenter, (float)(spriteData.angle), &translate);

	sprite->SetTransform(&matrix);
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

void ValeGraphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try{
        switch(mode)
        {
        case graphicsNS::FULLSCREEN:
            if(fullscreen)      // if already in fullscreen mode
                return;
            fullscreen = true; break;
        case graphicsNS::WINDOW:
            if(fullscreen == false) // if already in window mode
                return;
            fullscreen = false; break;
        default:        // default to toggle window/fullscreen
            fullscreen = !fullscreen;
        }
        reset();
        if(fullscreen)  // fullscreen
        {
            SetWindowLong(hWnd, GWL_STYLE,  WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
        }
        else            // windowed
        {
            SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            SetWindowPos(hWnd, HWND_TOP, 0,0,GAME_WIDTH,GAME_HEIGHT,
                SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

            // Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);   // get size of client area of window
            MoveWindow(hWnd,
                       0,                                           // Left
                       0,                                           // Top
                       GAME_WIDTH+(GAME_WIDTH-clientRect.right),    // Right
                       GAME_HEIGHT+(GAME_HEIGHT-clientRect.bottom), // Bottom
                       TRUE);                                       // Repaint the window
        }

    } catch(...)
    {
        // An error occured, try windowed mode 
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP, 0,0,GAME_WIDTH,GAME_HEIGHT,
            SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

        // Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);   // get size of client area of window
        MoveWindow(hWnd,
                    0,                                           // Left
                    0,                                           // Top
                    GAME_WIDTH+(GAME_WIDTH-clientRect.right),    // Right
                    GAME_HEIGHT+(GAME_HEIGHT-clientRect.bottom), // Bottom
                    TRUE);                                       // Repaint the window
    }
}

//=============================================================================
// Checks the adapter to see if it is compatible with the BackBuffer height,
// width and refresh rate specified in d3dpp. Fills in the pMode structure with
// the format of the compatible mode, if found.
// Pre: d3dpp is initialized.
// Post: Returns true if compatible mode found and pMode structure is filled.
//       Returns false if no compatible mode found.
//=============================================================================
bool ValeGraphics::isAdapterCompatible()
{
	UINT modes = direct3d9->GetAdapterModeCount(D3DADAPTER_DEFAULT, 
                                            d3dpp.BackBufferFormat);
    for(UINT i=0; i<modes; i++)
    {
        result = direct3d9->EnumAdapterModes( D3DADAPTER_DEFAULT, 
                                        d3dpp.BackBufferFormat,
                                        i, &pMode);
        if( pMode.Height == d3dpp.BackBufferHeight &&
            pMode.Width  == d3dpp.BackBufferWidth &&
            pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
            return true;
    }
    return false;
}

HRESULT ValeGraphics::getDeviceState()
{
	result = E_FAIL;
	if(direct3d9Device == NULL)
		return result;
	result = direct3d9Device->TestCooperativeLevel();
	return result;
}

HRESULT ValeGraphics::reset()
{
	result = E_FAIL;
	initD3DPresentParams();
	result = direct3d9Device->Reset(&d3dpp);
	return result;
}

//Text Drawing code from: http://code.google.com/p/sg210-maze/source/browse/trunk/MazeOfdoom-Brian.cpp
void ValeGraphics::DrawTextString(int x, int y, DWORD color, const char *str)
{
	RECT rTxt = {x,y,x+200,y+100};
	pFont->DrawTextA(NULL, str, -1, &rTxt, DT_LEFT, color);
}

void ValeGraphics::DrawTextString( int x, int y, const char *str )
{
	// Output the text, left aligned
	RECT rTxt = {x, y, x + 200, y + 100};
	pFont->DrawTextA( NULL, str, -1, &rTxt, DT_CALCRECT, 0 );
    pFont->DrawTextA(NULL, str, -1, &rTxt, DT_LEFT, fontColor );
}

void ValeGraphics::releaseAll()
{
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(direct3d9Device);
	SAFE_RELEASE(direct3d9);
}