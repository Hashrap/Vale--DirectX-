// Authors: Alex George, Spencer Corkran

#include "StdAfx.h"
#include "Level.h"
#include <cmath>
#include <algorithm>

namespace hudNS
{
	const float SPAWN_X = 650.0f;
	const float SPAWN_Y = 400.0f;

	const int HEART_X = 50;
	const int HEART_Y = 50;

	const int QUICK_ICON_X = 500;	
	const int FOCUS_ICON_X = 600;
	const int FLEET_ICON_X = 700;

	const int ABILITY_ICON_Y = 600;
	const int ABILITY_ICON_SIZE = 64;

	const int COOLDOWN_SIZE = 64;
	const LPWSTR FONT = L"Arial Bold"; 
    const int BUF_SIZE = 20;
	const COLOR_ARGB FONT_COLOR = graphicsNS::BLACK;
}

Level::Level(void)
{
	width = MAX_WIDTH;
	height = MAX_HEIGHT;
	levelCoords = VECTOR2(0, 0);
}


Level::~Level(void){}


void Level::initialize(ValeGame *gamePtr)
{
	gamePointer = gamePtr;
	graphics = gamePtr->getGraphics();
	input = gamePtr->getInput();

	srand(time(NULL));

	//fonts
    quickCD.initialize(graphics, hudNS::COOLDOWN_SIZE, false, false, hudNS::FONT);
    quickCD.setFontColor(hudNS::FONT_COLOR);
	focusCD.initialize(graphics, hudNS::COOLDOWN_SIZE, false, false, hudNS::FONT);
	focusCD.setFontColor(hudNS::FONT_COLOR);
	fleetfeetCD.initialize(graphics, hudNS::COOLDOWN_SIZE, false, false, hudNS::FONT);
	fleetfeetCD.setFontColor(hudNS::FONT_COLOR);
	scoreText.initialize(graphics, 64, false, false, hudNS::FONT);
	scoreText.setFontColor(SETCOLOR_ARGB(255, 78, 162, 241));

	//textures
	wardenTexture.initialize(graphics, WARDEN_IMAGE);
	projectileTextures.initialize(graphics, PROJECTILE_IMAGE);
	focusBarFillTexture.initialize(graphics, FOCUS_BAR_FILL); 
	focusBarBGTexture.initialize(graphics, FOCUS_BAR_BG); 
	focusBarBGMuseTexture.initialize(graphics, FOCUS_BAR_BG_MUSE); 
	heartHUDTexture.initialize(graphics, HEART_HUD);
	heartTexture.initialize(graphics, HEART_ICON);
	spawnPointTexture.initialize(graphics,SPAWNER_IMAGE);

	//ability textures
	for(int i = 0; i < wardenNS::NUM_BOWS; i++)
	{
		bowTexture[i].initialize(graphics, BOW_ICON[i]);
		quickShotAbilityTexture[i].initialize(graphics, QUICKSHOT_ICON[i]);
		focusShotAbilityTexture[i].initialize(graphics, FOCUSSHOT_ICON[i]);
	}
	fleetFeetAbilityTexture.initialize(graphics, FLEETFEET_ICON);


	// Collision detection
	// Compute grid dimensions from the map size divided by grid dimension size
	float dX = (float)((width * tileSize)/GRID_CELL_DIMENSIONS);
	gridW = (int)ceil(dX);
	float dY = (float)((height * tileSize)/GRID_CELL_DIMENSIONS);
	gridH = (int)ceil(dY);

	// Instantiate the grid
	collision = cGrid(gridW, gridH, GRID_CELL_DIMENSIONS, &score);

	//particle system bs
	ps.initialize(graphics, L"..\\..\\Textures\\Particles\\smoke_hardedge.tga");
	ps.setEmitterShape(EMITTER_SHAPE_NONE);
	ps.setEmitterType(EMITTER_TYPE_DEFAULT);
	D3DXVECTOR2 tempPos(-100.0f, -100.0f);
	ps.setPosition(tempPos);
	
	particleSystems.resize(5);
	for(int i = 0; i < 5; i++)
	{
		particleSystems[i].initialize(graphics, L"..\\..\\Textures\\Particles\\smoke_hardedge.tga");
		particleSystems[i].setEmitterShape(EMITTER_SHAPE_NONE);
		particleSystems[i].setEmitterType(EMITTER_TYPE_DEFAULT);
		D3DXVECTOR2 tempPos(-100.0f, -100.0f);
		particleSystems[i].setPosition(tempPos);
	}
	lastUsedParticleSystem = 4;

	//warden
	warden.initialize(gamePtr, 32, 32, 1, &wardenTexture, &projectileTextures, &projectileTextures);
	D3DXVECTOR2 tmp(hudNS::SPAWN_X, hudNS::SPAWN_Y);
	warden.setPosition(tmp);
	score = 0;

	//Enemies
	risenManager.init(&score);
	risenSpawner.initialize(gamePtr,32,32,1,&spawnPointTexture,&risenManager.risen,&warden,tiles);
	risenSpawner.setPosition(tmp);

	addTimedSpawn(gamePtr,Risen::WAR,30.0,D3DXVECTOR2(2000.0f,1500.0f),400000);

	std::vector<Risen::types> hello1;
	std::vector<int> hello2;
	hello1.push_back(Risen::WARLOCK);
	hello1.push_back(Risen::ARCHER);
	hello1.push_back(Risen::HOUND);
	hello2.push_back(2);
	hello2.push_back(3);
	hello2.push_back(1);
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(750.0f,3200.0f),400000);
	
	hello1.clear();
	hello2.clear();
	hello1.push_back(Risen::HOUND);
	hello2.push_back(1);
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(1500.0f,2750.0f),400000);

	hello1.clear();
	hello2.clear();

	hello1.push_back(Risen::WAR);
	hello1.push_back(Risen::ARCHER);
	hello2.push_back(1);
	hello2.push_back(1);
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(3300.0f,1000.0f),300000);

	hello1.clear();
	hello2.clear();

	hello1.push_back(Risen::WAR);
	hello2.push_back(1);
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(2350.0f,785.0f),320000);

	hello2[0] = 2;
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(2430.0f,175.0f),120000);

	hello1.push_back(Risen::WARLOCK);
	hello2.push_back(1);
	hello2[0] = 1;
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(3650.0f,1900.0f),250000);
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(3700.0f,2000.0f),200000);
	hello1[1] = Risen::WAR;
	addStaticSpawn(gamePtr,hello1,hello2,D3DXVECTOR2(3700.0f,1800.0f),250000);

	//equipment
	addBow(20, 2, SPLIT);
	addBow(19, 21, MUSE);

	//Hud
	scoreRect.left = 50.0f; scoreRect.top = 50.0f; scoreRect.bottom = 150.0f; scoreRect.right = 250.0f;
	//focus bar
	focusBarFill.initialize(graphics, &focusBarFillTexture, 128, 128, 0, 0, 1.0f, graphicsNS::WHITE);
	focusBarBG.initialize(graphics,128, 128, 1, &focusBarBGTexture);
	focusBarBGMuse.initialize(graphics, 128, 128, 1, &focusBarBGMuseTexture);

	for(int i = 0; i < 4; i++)
	{
		heartHUD[i].initialize(graphics, 32, 32, 1, &heartHUDTexture);
		heartHUD[i].setX(hudNS::HEART_X + i*32);
		heartHUD[i].setY(hudNS::HEART_Y);
	}

	//abilities 
	for(int i = 0; i < wardenNS::NUM_BOWS; i++)
	{
		quickShotAbility[i].initialize(graphics, hudNS::ABILITY_ICON_SIZE, hudNS::ABILITY_ICON_SIZE, 1, &quickShotAbilityTexture[i]);

		quickShotAbility[i].setX(hudNS::QUICK_ICON_X);
		quickShotAbility[i].setY(hudNS::ABILITY_ICON_Y);

		focusShotAbility[i].initialize(graphics, hudNS::ABILITY_ICON_SIZE, hudNS::ABILITY_ICON_SIZE, 1, &focusShotAbilityTexture[i]);		

		focusShotAbility[i].setX(hudNS::FOCUS_ICON_X);
		focusShotAbility[i].setY(hudNS::ABILITY_ICON_Y);
	}


	fleetFeetAbility.initialize(graphics, hudNS::ABILITY_ICON_SIZE, hudNS::ABILITY_ICON_SIZE, 1, &fleetFeetAbilityTexture);
	fleetFeetAbility.setX(hudNS::FLEET_ICON_X);
	fleetFeetAbility.setY(hudNS::ABILITY_ICON_Y);
}

//Update everything in the level
void Level::update(float frameTime)
{
	//If warden is using focus shot, slow down time
	if(warden.isFocusing())
		frameTime/=2;

	//Warden
	warden.update(frameTime);

	//if warden died, respawn
	if(!warden.isAlive())
	{
		wardenDied();
	}


	//Particle systems
	for(unsigned int i = 0; i < particleSystems.size(); i++)
	{
		particleSystems[i].update(frameTime);
	} 
	ps.update(frameTime);
	
	//Risen
	risenSpawner.update(frameTime);
	risenManager.update(frameTime);
	
	//remove dead risen
	for(unsigned int i = 0 ; i < risenManager.risen.size(); i++)
		if(!risenManager.risen[i]->isAlive())
			removeRisen(i);

	//bows
	for(unsigned int i = 0; i < bows.size(); i++)
	{
		bows[i]->update(frameTime);
	}	

	//hearts
	for(unsigned int i = 0; i < hearts.size(); i++)
	{
		hearts[i]->update(frameTime);
	}

	// Check collision
	checkCollision();

	//Update images after other calculations
	updateImagePositions();
}

void Level::updateImagePositions()
{
	//Grab the warden's position
	D3DXVECTOR2 wardenPosition = warden.getPosition();

	levelCoords = VECTOR2(wardenPosition.x - GAME_WIDTH/2, wardenPosition.y - GAME_HEIGHT/2);

	
	//update map
	setMap(levelCoords.x, levelCoords.y);	
	
	//update warden
	warden.updateImagePosition(GAME_WIDTH/2 /*- wardenNS::SIZE/2*/, GAME_HEIGHT/2 /*- wardenNS::SIZE/2*/);
	warden.updateProjectiles(levelCoords);

	//particles
	//ps.moveWithMap(levelCoords.x, levelCoords.y);
	ps.updateImagePosition(levelCoords.x, levelCoords.y);
	for(unsigned int i = 0 ; i < particleSystems.size(); i++)
	{
		particleSystems[i].updateImagePosition(levelCoords.x, levelCoords.y);
	}

	//Update risen
	risenSpawner.updateImagePosition(levelCoords.x, levelCoords.y);
	for(int i = 0, count = risenSpawner.getSpawnPoints().size(); i < count; i++)
	{
		risenSpawner.getSpawnPoints()[i]->updateImagePosition(levelCoords.x, levelCoords.y);
	}
	risenManager.updateImagePosition(levelCoords);

	//Update bows
	for(unsigned int i= 0; i < bows.size();  i++)
	{
		bows[i]->updateImagePosition(levelCoords.x, levelCoords.y);
	}

	//update hearts
	for(unsigned int i = 0; i < hearts.size(); i ++)
	{
		hearts[i]->updateImagePosition(levelCoords.x, levelCoords.y);
	}

	//update hud
	if(warden.isFocusing())
	{
		if(warden.getBow() == MUSE)
		{
			focusBarBGMuse.setVisible(true);
			focusBarBGMuse.setX(input->getMouseX()-64);
			focusBarBGMuse.setY(input->getMouseY()-64);
		}
		else
		{
			focusBarBG.setVisible(true);
			focusBarBG.setX(input->getMouseX()-64);
			focusBarBG.setY(input->getMouseY()-64);
		}

		

		focusBarFill.set(warden.getFocusChannelPercentage()*100);
		focusBarFill.setPosition(input->getMouseX()-64, input->getMouseY()-64);
		focusBarFill.setVisible(true);
	}
	else
	{
		focusBarFill.setVisible(false);
		focusBarBG.setVisible(false);
		focusBarBGMuse.setVisible(false);
	}
}

void Level::checkInput()
{	

	//RIGHT MOUSE BUTTON --- FOCUS SHOT
	if(!warden.isFocusing() && input->getMouseRButton())
	{
		warden.startFocus();
	}
	//RIGHT MOUSE BUTTON --- FOCUS SHOT
	if(warden.isFocusing() && !input->getMouseRButton())
	{
		warden.endFocus();
	}
	//LEFT MOUSE BUTTON --- QUICK SHOT
	if(!warden.isFocusing() && input->getMouseLButton())
	{		
		warden.useQuickShot();
	}
	//SPACE BAR --- FLEET FEET
	if(!warden.isFocusing() && input->isKeyDown(0x20))
	{
		warden.useFleetFeet();
	}

	// 'E' --- EQUIP ITEM
	if(!warden.isFocusing() && input->isKeyDown(0x45))
	{
		if(nearBow())
		{
			Bow *pickedBow = getFirstBow();
			pickedBow->equip();
			warden.unlockBow(pickedBow->getBowType());
		}
	}

	// '1' --- GUARDIAN BOW
	if(!warden.isFocusing() && input->isKeyDown(0x31))
	{
		warden.switchBow(GUARDIAN);
	}
	// '2' --- SPLIT SHOT
	else if(!warden.isFocusing() && input->isKeyDown(0x32))
	{
		warden.switchBow(SPLIT);
	}
	// '3' --- REBOUND SHOT
	else if(!warden.isFocusing() && input->isKeyDown(0x33))
	{
		warden.switchBow(REBOUNDER);
	}
	// '4' --- MUSE SHOT
	else if(!warden.isFocusing() && input->isKeyDown(0x34))
	{
		warden.switchBow(MUSE);
	}

	//Send the warden some direction input     0x57 'W'                0x53 'S'                0x41 'A'                0x44 'D'
	warden.setDirectionVector(input->isKeyDown(0x57), input->isKeyDown(0x53), input->isKeyDown(0x41), input->isKeyDown(0x44));
}

void Level::checkCollision()
{
	// Assemble a vector of all InteractiveObjects for collision testing
	std::vector< InteractiveObject* > objs;
	std::vector< Projectile >* shots;

	//add warden
	objs.push_back(&warden);

	//add risen and risen projectiles
	for(unsigned int i = 0; i < risenManager.risen.size(); i++)
	{
		shots = risenManager.risen[i]->getProjectileManager()->getActiveProjectiles();
		objs.push_back(risenManager.risen[i]);
		for(unsigned int j = 0; j < shots->size(); j++)
		{
			objs.push_back(&((*shots)[j]));
		}
	}

	//add warden projectiles
	shots =	warden.getProjectileManager()->getActiveProjectiles();
	for(unsigned int i = 0; i < shots->size(); i++)
	{
		objs.push_back(&((*shots)[i]));
	}

	//add hearts
	for(unsigned int i = 0; i < hearts.size(); i ++)
	{
		objs.push_back(hearts[i]);
	}


	// Add objects to the grid
	collision.populateGrid(objs);

	// Check collisions against the warden;
	collision.checkCollisions(&warden);

	// Check collisions against the risen
	for(unsigned int i = 0; i < risenManager.risen.size(); i++)
	{
		collision.checkCollisions(risenManager.risen[i]);
	}

	if(testXTerrain(warden.getCenter(), warden.getOldCenter()))
	{
		warden.setPosition(warden.getOldPosition().x, warden.getPosition().y);
	}
	if(testYTerrain(warden.getCenter(), warden.getOldCenter()))
	{
		warden.setPosition(warden.getPosition().x, warden.getOldPosition().y);
	}

	//Warden projectiles
	shots = warden.getProjectileManager()->getActiveProjectiles();
	if(shots->size() > 0)
	{
		for(int i = shots->size()-1; i >= 0; i--)
		{
			// If the position collides with terrain
			if(testTerrain((*shots)[i].getCenter()))
			{
				// Add a particlesystem
				lastUsedParticleSystem++;
				if(lastUsedParticleSystem > 4){
					lastUsedParticleSystem = 0;
				}
				particleSystems[lastUsedParticleSystem].setPosition((*shots)[i].getCenter());
				particleSystems[lastUsedParticleSystem].activate();

				// Remove bullets from play and from the collision grid	
				warden.getProjectileManager()->killProjectile(&((*shots)[i]), i);
			}
		}
	}

	//Risen and Risen Projectiles
	for(unsigned int i = 0; i < risenManager.risen.size(); i++)
	{
		//Risen Projectiles
		shots = risenManager.risen[i]->getProjectileManager()->getActiveProjectiles();
		if(shots->size() > 0)
		{
			for(int j = shots->size()-1; j >= 0; j--)
			{
				// If the position collides with terrain
				if(testTerrain((*shots)[j].getCenter()))
				{
					// Remove bullets from play and from collision grid
					risenManager.risen[i]->getProjectileManager()->killProjectile(&((*shots)[j]), j);
				}
			}
		}

		// Risen vs Terrain
		if(testXTerrain(risenManager.risen[i]->getCenter(), risenManager.risen[i]->getOldCenter()))
		{
			// Reset X
			risenManager.risen[i]->setPosition(risenManager.risen[i]->getOldPosition().x, risenManager.risen[i]->getPosition().y);
		}
		if(testYTerrain(risenManager.risen[i]->getCenter(), risenManager.risen[i]->getOldCenter()))
		{
			// Reset Y
			risenManager.risen[i]->setPosition(risenManager.risen[i]->getPosition().x, risenManager.risen[i]->getOldPosition().y);
		}
	}

	collision.depopulateGrid();
}

// Check object positions against the vector of tiles used to create the map
// Returns true for a collision, false for no collision.
bool Level::testTerrain(VECTOR2 pos)
{
	int objX = pos.x / tileSize;
	int objY = pos.y / tileSize;

	// Value of 2 indicates a walkable tile
	return (tiles[(objY*height)+objX] != 2);
}

bool Level::testXTerrain(VECTOR2 pos, VECTOR2 oldPos)
{
	int objX = pos.x / tileSize;
	int objY = pos.y / tileSize;
	int oX = oldPos.x / tileSize;
	int oY = oldPos.y / tileSize;
	if(objX != oX && tiles[(oY*height)+objX] != 2)
		return true;
	return false;
}

bool Level::testYTerrain(VECTOR2 pos, VECTOR2 oldPos)
{
	int objY = pos.y / tileSize;
	int objX = pos.x / tileSize;
	int oX = oldPos.x / tileSize;
	int oY = oldPos.y / tileSize;

	if(objY != oY && tiles[(objY*height)+oX] != 2)
		return true;
	return false;
}

Bow* Level::getFirstBow()
{
	//check warden against equipment. maybe return the first item?
	for(unsigned int i = 0; i < bows.size(); i++)
		 if(!bows[i]->isEquipped() && collision.testCollision(&warden, bows[i]))
			 return bows[i];

	return new Bow();
}

//is the warden colliding with equipment on the ground?
bool Level::nearBow()
{
	for(unsigned int i = 0; i < bows.size(); i++)
		if(!bows[i]->isEquipped() && collision.testCollision(&warden, bows[i]))
			 return true;
	
	return false;
}

// sets the passed in variables to the max width and height of the map
// so far unused
void Level::getMaxDimensions(int &width, int &height)
{
	width = MAX_WIDTH;
	height = MAX_HEIGHT;
}

//draws everything
void Level::draw()
{
	//draw map
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			mapImages[(i*height)+j].draw();
		}
	}

	//draw warden
	warden.draw();

	//Draw particles
	for(unsigned int i = 0; i < particleSystems.size(); i++)
		particleSystems[i].draw();
	ps.draw();
	
	//Risen
	for(int i = 0, count = risenSpawner.getSpawnPoints().size(); i < count; i++)
		risenSpawner.getSpawnPoints()[i]->draw();
	risenManager.draw();


	//hearts
	for(unsigned int i = 0; i < hearts.size(); i++)
		hearts[i]->draw();

	//bows
	for(unsigned int i = 0; i < bows.size(); i++)
		bows[i]->draw();

	//HUD

	//health
	for(int i = 0; i < wardenNS::MAX_HEALTH; i++)
		if(warden.getCurrentHealth() >= i+1) //draw full heart
			heartHUD[i].draw();
		else	//else draw transparent
			heartHUD[i].draw(D3DCOLOR_ARGB(100,255,255,255));

	//focusing bar	
	focusBarBG.draw(D3DCOLOR_ARGB(204,255,255,255));
	focusBarBGMuse.draw(D3DCOLOR_ARGB(204,255,255,255));
	focusBarFill.draw();

	//ability icons
	for(int i = 0; i < wardenNS::NUM_BOWS; i++)
	{
		if(warden.getBow() == i)
		{
			quickShotAbility[i].draw();
			focusShotAbility[i].draw();
		}
	}
	fleetFeetAbility.draw();
	
	//quickshot cooldown text
	if(warden.getQuickShotCooldown()>=0)
	{
		_snprintf_s (buffer, hudNS::BUF_SIZE, "%d", (int)(ceil(warden.getQuickShotCooldown())));
		quickCD.print(buffer, hudNS::QUICK_ICON_X + hudNS::COOLDOWN_SIZE/4, hudNS::ABILITY_ICON_Y);
	}

	//focus shot cooldown text
	if(warden.getFocusShotCooldown()>=0)
	{
		_snprintf_s (buffer, hudNS::BUF_SIZE, "%d", (int)(ceil(warden.getFocusShotCooldown())));
		focusCD.print(buffer, hudNS::FOCUS_ICON_X + hudNS::COOLDOWN_SIZE/4, hudNS::ABILITY_ICON_Y);
	}

	//focus shot cooldown text
	if(warden.getFleetFeetCooldown()>=0)
	{
		_snprintf_s (buffer, hudNS::BUF_SIZE, "%d", (int)(ceil(warden.getFleetFeetCooldown())));
		fleetfeetCD.print(buffer, hudNS::FLEET_ICON_X + hudNS::COOLDOWN_SIZE/4, hudNS::ABILITY_ICON_Y);
	}

	if(score < 0){score = 0;}
	// display score in upper right
	itoa(score, buffer, 10);
	scoreText.print(buffer, graphics->getWidth() - 150, 50);
}

void Level::setGroundImages(ValeGraphics* g)
{
	grass.initialize(g, L"..\\..\\Textures\\Terrain\\Grass2.tga");
	unwalkable.initialize(g, L"..\\..\\Textures\\Terrain\\unwalkable_terrain.tga");
	tree[0].initialize(g, L"..\\..\\Textures\\Terrain\\tree.tga");
	tree[1].initialize(g, L"..\\..\\Textures\\Terrain\\tree2.tga");
	tree[2].initialize(g, L"..\\..\\Textures\\Terrain\\tree3.tga");
	road.initialize(g, L"..\\..\\Textures\\Terrain\\rock_sortof.png");
	ruins[0].initialize(g, L"..\\..\\Textures\\Terrain\\rock.tga");
	ruins[1].initialize(g, L"..\\..\\Textures\\Terrain\\rock2.tga");
	ruins[2].initialize(g, L"..\\..\\Textures\\Terrain\\rock3.tga");
	//door? animated

	// if map starts loading incorrectly, uncomment this to help debug
	// prints out the ints representing the level in the same order as
	// they should be used to construct level
	/*int temp = 0;
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			std::cout << tiles[(i*height)+j];
			temp++;

			if(temp >= width)
			{
				std::cout << std::endl;
				temp = 0;
			}
		}
	}*/

	mapImages.resize(width * height);
	//map.resize(width * height);
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			/*Tile t;
			t.size = tileSize;
			t.x = j * t.size;
			t.y = i * t.size;
			t.location.left = t.x;
			t.location.top = t.y;
			t.location.right = t.x + tileSize;
			t.location.bottom = t.y + tileSize;*/

			int r;

			switch(tiles[(i*height)+j])
			{
			case 0:// nothing
				break;
			case 1:
				// unwalkable
				mapImages[(i*height)+j].initialize(g, tileSize, tileSize, 1, &unwalkable);
				//t.tileImage.initialize(g, tileSize, tileSize, 1, &dirtGrass);
				break;
			case 2:
				// walkable
				mapImages[(i*height)+j].initialize(g, tileSize, tileSize, 1, &grass);
				//t.tileImage.initialize(g, tileSize, tileSize, 1, &road);
				break;
			case 3:
				r = rand() % 3;
				mapImages[(i*height)+j].initialize(g, tileSize, tileSize, 1, &tree[r]);
				//tree
				break;
			case 4:
				r = rand() % 3;
				mapImages[(i*height)+j].initialize(g, tileSize, tileSize, 1, &ruins[r]);
				//ruins/wall piece
				break;
			case 5:
				// door
				break;
			case 6:
				//playerspawn
				break;
			case 7:
				//monster spawn
				break;
			case 8:
				// bow pickup
				break;
			case 9:
				// boots pickup
				break;
			case 10:
				// destructibles
				break;
			}
			/*t.tileImage.setX(t.x);
			t.tileImage.setY(t.y);
			map[(i*height)+j] = t;*/
		}
	}
}

// Sets the location of the map to to the given x and y values
// This function differs from scrollMap() only in that it operates as an '=' rather than '+='
void Level::setMap(float x, float y)
{
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			mapImages[(i*height)+j].setX((j*tileSize) - x);
			mapImages[(i*height)+j].setY((i*tileSize) - y);
		}
	}
}

// populates the variables passed in for tileX and tileY with the
// map coords (x,y) position in the map vector
void Level::grabMapCoord(int* tileX, int* tileY, float mouseX, float mouseY)
{
	*tileX = static_cast<int>(mouseX - mapImages[0].getX()) / tileSize;
	*tileY = static_cast<int>(mouseY - mapImages[0].getY()) / tileSize;

	// don't bother with negative map coords
	if(*tileX < 0)
		*tileX = 0;
	if(*tileY < 0)
		*tileY = 0;

}

bool Level::loadMap(char* file, ValeGame *gp)
{
	gamePointer = gp;

	tileSize = 128;

	// first find out the file type
	// if its not a valid type throw an error and return false
	int length = 0;
	while(file[length] != '\0')
	{
		length++;
	}

	// checks for a valid file type - we do not trust anything other than .vmp files
	if(file[length -4] != '.' && file[length - 3] != 'v' && file[length - 2] != 'm' && file[length - 1] != 'p')
	{
		// bad file type
		// error
		return false;
	}
	
	// since it is of the right file type, as far as we can tell,
	// open it and let us see what happens.

	
	{ // scopes the stream

		// grab the lines and go
		std::fstream binIn;
		binIn.open(file, std::ios::in|std::ios::binary);

		char mapName[6];
		binIn.read(mapName, 6);

		// set to 0/(randoms) mostly for correct input reading checks (debug stuff)
		int mapWidth = 0;
		int mapHeight = 0;
		width = 100;
		height = 200;
		//

		// grab the dimensions so we know how big of a vector we are going to populate
		if(!binIn.eof())
		{
			binIn.read(reinterpret_cast<char *>(&mapWidth), sizeof(int));
			width = mapWidth;
		}
		if(!binIn.eof())
		{
			binIn.read(reinterpret_cast<char *>(&mapHeight), sizeof(int));
			height = mapHeight;
		}

		//int location_debug = -1;

		// resize the vector since we know the size we need
		tiles.resize(width * height);

		// now loop to grab each tile
		// and set it in the vector
		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				//location_debug = ((i*height)+j);
				if(binIn.eof())
					break;
				char temp;
				binIn.get(temp);
				switch(temp)
				{
				case 'x':
					// unwalkable area
					tiles[(i*height)+j] = 1;
					break;
				case 'f':
					// walkable area (floor)
					tiles[(i*height)+j] = 2;
					break;
				case 't':
					// tree
					tiles[(i*height)+j] = 3;
					break;
				case 'r':
					// ruin wall/piece
					tiles[(i*height)+j] = 4;
					break;
				case 'd':
					// door
					tiles[(i*height)+j] = 2;
					break;
				case 'D':
					// Destructibles
					tiles[(i*height)+j] = 10;
				}
			}
		}

		// read in player spawn
		int playerSpawnx, playerSpawny;
		binIn.read(reinterpret_cast<char *>(&playerSpawnx), sizeof(int));
		binIn.read(reinterpret_cast<char *>(&playerSpawny), sizeof(int));

		// num monster spawns
		int numMonsterSpawns = 0;
		binIn.read(reinterpret_cast<char *>(&numMonsterSpawns), sizeof(int));
		
		// monster spawns
		for(int i = 0; i < numMonsterSpawns*2; i+=2)
		{
			int tempx, tempy;
			binIn.read(reinterpret_cast<char *>(&tempx), sizeof(int));
			binIn.read(reinterpret_cast<char *>(&tempy), sizeof(int));

			if(i > 3)
			{
				D3DXVECTOR2 tempVec;
				tempVec.x = 22*tileSize + tileSize/2;
				tempVec.y = 21*tileSize + tileSize/2;
				addTimedSpawn(gamePointer, Risen::WARLOCK, 6.0f, tempVec, 320000.0f);
				
			}
			else if(i < 2)
			{
				D3DXVECTOR2 tempVec;
				tempVec.x = 7*tileSize + tileSize/2;
				tempVec.y = 19*tileSize + tileSize/2;
				addTimedSpawn(gamePointer, Risen::WAR, 20.0f, tempVec, 320000.0f);
			}
			else
			{
				D3DXVECTOR2 tempVec;
				tempVec.x = 10*tileSize + tileSize/2;
				tempVec.y = 10*tileSize + tileSize/2;
				addTimedSpawn(gamePointer, Risen::ARCHER, 16.0f, tempVec, 320000.0f);
			}
		}

		// num bow spawns
		int numBowSpawns = 0;
		binIn.read(reinterpret_cast<char *>(&numBowSpawns), sizeof(int));

		//bow spawns
		for(int i = 0; i < numBowSpawns*2; i+=2)
		{
			int tempx, tempy;
			binIn.read(reinterpret_cast<char *>(&tempx), sizeof(int));
			binIn.read(reinterpret_cast<char *>(&tempy), sizeof(int));
		}

		binIn.close();
	} // end of binIn scope

	return true;
}

void Level::addTimedSpawn(ValeGame* gamePtr,Risen::types type,float activeTime,D3DXVECTOR2 position,float activationDistance)
{
	RisenSpawnPoint* sP = new RisenSpawnPoint();
	sP->initialize(gamePtr,32,32,1,&spawnPointTexture,activationDistance);
	sP->setPosition(position);
	sP->setTimed(type,activeTime);
	risenSpawner.addSpawnPoint(sP);
}
void Level::addStaticSpawn(ValeGame* gamePtr,std::vector<Risen::types> types,std::vector<int> amounts,D3DXVECTOR2 position,float activationDistance)
{
	RisenSpawnPoint* sP= new RisenSpawnPoint();
	sP->initialize(gamePtr,32,32,1,&spawnPointTexture,activationDistance);
	for(unsigned int i = 0; i < types.size();i++)
	{
		sP->createRisen(types[i],amounts[i]);
	}
	
	sP->setPosition(position);
	risenSpawner.addSpawnPoint(sP);
}

void Level::addHeart(int levelX, int levelY)
{
	Heart* newHeart = new Heart();
	newHeart->initialize(gamePointer, heartNS::SIZE, heartNS::SIZE, 1, &heartTexture);
	newHeart->setPosition(VECTOR2(levelX, levelY));
	hearts.push_back(newHeart);
}

void Level::addBow(int tileX, int tileY)
{
	bool muse = rand()%2 == 0;
	if(muse)
		addBow(tileX, tileY, MUSE);
	else
		addBow(tileX, tileY, SPLIT);
}
	
void Level::addBow(int tileX, int tileY, BowType type)
{
	Bow *bow = new Bow();
	bow->initialize(gamePointer, bowNS::SIZE, bowNS::SIZE, 1, &bowTexture[type], type);
	//center the bow on the tile
	bow->setPosition(VECTOR2(tileX * tileSize - tileSize/2 - bowNS::SIZE/2, tileY * tileSize - tileSize/2- bowNS::SIZE/2));
	bows.push_back(bow);
}

void Level::removeRisen(int index)
{
	// 1/3 chance to spawn heart
	if(rand()%3 == 0)
	{
		addHeart(risenManager.risen[index]->getPosition().x,risenManager.risen[index]->getPosition().y );
	}
	risenManager.risen.erase(risenManager.risen.begin() + index);
}

void Level::wardenDied()
{
	warden.respawnAtPosition(hudNS::SPAWN_X, hudNS::SPAWN_Y);
	score -= 1000;
	//cut score?
}