#pragma once

class InteractiveObject;

#include "InteractiveObject.h"
#include <vector>

//Throw all of our globally used enums and structs up in here

enum Faction { PLAYER, RISEN, NEUTRAL };

//List of bow types
const int NUM_BOWS = 4;
enum BowType { GUARDIAN, SPLIT, REBOUNDER, MUSE };

//List of possible stats we will want to change.
enum StatType { MOVEMENTSPEED, CURRENTHEALTH };

//Defines a StatBuff (used for abilities such as FleetFeet)
struct StatBuff {
	float elapsedTime; //How long this buff has been active - remove it when it reaches the duration
	float duration; //How long the buff will last
	float multiplier; //Multiplies the stat to change by this number
	float additive; //Flat increase
	StatType alteredStat;
};

// Cell for an object collision grid
struct GridCell
{
	std::vector< InteractiveObject* > bucket;

	void remove(InteractiveObject* obj)
	{
		for(unsigned int i = 0; i < bucket.size(); i++)
		{
			if(obj == bucket[i])
				bucket.erase(bucket.begin()+i);
		}
	}

	void add(InteractiveObject* obj)
	{
		bool flag = true;
		for(unsigned int i = 0; i < bucket.size(); i++)
		{
			if(bucket[i] == obj)
				flag = false;
		}
		if(flag)
			bucket.push_back(obj);
	}
};

// graphic images
const LPCWSTR WARDEN_IMAGE =   L"..\\..\\Textures\\Entities\\warden.png";   
const LPCWSTR PROJECTILE_IMAGE = L"..\\..\\Textures\\Projectiles\\arrow.png";  
const LPCWSTR RISEN_IMAGE =  L"..\\..\\Textures\\Entities\\risenWarrior.png";   
const LPCWSTR ARCHER_IMAGE = L"..\\..\\Textures\\Entities\\risenArcher.png"; 
const LPCWSTR HOUND_IMAGE = L"..\\..\\Textures\\Entities\\risenHound.png"; 
const LPCWSTR WARLOCK_IMAGE = L"..\\..\\Textures\\Entities\\risenWarlock.png"; 
const LPCWSTR SPAWNER_IMAGE = L"..\\..\\Textures\\Entities\\spawner.png"; 
//abilities
const LPCWSTR QUICKSHOT_ICON[4] =  { L"..\\..\\Textures\\Icons\\quickshot_ability.jpg", L"..\\..\\Textures\\Icons\\splitshot_ability.jpg", L"..\\..\\Textures\\Icons\\reboundshot_ability.jpg", L"..\\..\\Textures\\Icons\\musequickshot_ability.jpg"};
const LPCWSTR FOCUSSHOT_ICON[4] =  { L"..\\..\\Textures\\Icons\\focusshot_ability.jpg", L"..\\..\\Textures\\Icons\\splitfocusshot_ability.jpg", L"..\\..\\Textures\\Icons\\focusshot_ability.jpg", L"..\\..\\Textures\\Icons\\musefocusshot_ability.jpg"};
const LPCWSTR FLEETFEET_ICON =  L"..\\..\\Textures\\Icons\\fleetfeet_ability.jpg";

//items
const LPCWSTR BOW_ICON[4] =  { L"..\\..\\Textures\\Icons\\guardian_bow.jpg", L"..\\..\\Textures\\Icons\\splitshot_bow.jpg", L"..\\..\\Textures\\Icons\\rebound_bow.jpg", L"..\\..\\Textures\\Icons\\muse_bow.jpg" };
const LPCWSTR HEART_ICON = L"..\\..\\Textures\\Entities\\heart.png";

//hud
const LPCWSTR FOCUS_BAR_FILL =  L"..\\..\\Textures\\HUD\\focusBarFill.png";
const LPCWSTR FOCUS_BAR_BG =  L"..\\..\\Textures\\HUD\\focusBarBG.png";
const LPCWSTR FOCUS_BAR_BG_MUSE =  L"..\\..\\Textures\\HUD\\focusBarBGmuse.png";
const LPCWSTR HEART_HUD =  L"..\\..\\Textures\\HUD\\heart.png";

const LPCWSTR WARLOCK_RED = L"..\\..\\Textures\\Projectiles\\redBullet.png";
const LPCWSTR WARLOCK_GREEN = L"..\\..\\Textures\\Projectiles\\greenBullet.png";
const LPCWSTR WARLOCK_BLUE = L"..\\..\\Textures\\Projectiles\\blueBullet.png";
const LPCWSTR WARLOCK_TEAL = L"..\\..\\Textures\\Projectiles\\tealBullet.png";
const LPCWSTR WARLOCK_YELLOW = L"..\\..\\Textures\\Projectiles\\yellowBullet.png";
const LPCWSTR WARLOCK_BLACK = L"..\\..\\Textures\\Projectiles\\blackBullet.png";
const LPCWSTR WARLOCK_WHITE = L"..\\..\\Textures\\Projectiles\\whiteBullet.png";
const LPCWSTR COLORS[8] = { WARLOCK_RED, WARLOCK_GREEN, WARLOCK_BLUE, WARLOCK_TEAL, WARLOCK_YELLOW, WARLOCK_BLACK, WARLOCK_WHITE };

const LPCWSTR ARCHER_PROJECTILE = L"..\\..\\Textures\\Projectiles\\arrow.png";

#define SQRTHALF 0.70710678118f

//stats for abilities
namespace bowNS
{
	const int SIZE = 32;
	////quickshot

	//same for all bows
	const float QS_COOLDOWN = 1.0f; 
	const float QS_CASTTIME = 0.3f;
	const int QS_PROJSIZE = 8;

	//default	
	const int GB_QS_DAMAGE = 10;
	const float GB_QS_RANGE = 200.0f;
	const float GB_QS_PROJSPEED = 1000.0f;	
	const bool GB_QS_PIERCING = false;
	const bool GB_QS_RICOCHET = false;

	//split shot
	const int SS_QS_DAMAGE = 6;
	const float SS_QS_RANGE = 200.0f;
	const float SS_QS_PROJSPEED = 1000.0f;
	const bool SS_QS_PIERCING = false;
	const bool SS_QS_RICOCHET = false;
	const float SS_QS_ARC = 30.0f;
	const int SS_QS_PROJNUM = 3;

	//rebounder
	const int RB_QS_DAMAGE = 10;
	const float RB_QS_RANGE = 200.0f;
	const float RB_QS_PROJSPEED = 1000.0f;	
	const bool RB_QS_PIERCING = false;
	const bool RB_QS_RICOCHET = true;

	////focus shot

	//same for every bow
	const float FS_COOLDOWN = 4.5f;
	const float FS_CASTTIME = 0.3f;
	const float FS_MAX_CHANNEL_TIME = 1.1f;
	const float FS_RANGE = 800.0f;

	//default	
	const int GB_FS_PROJSIZE = 8;
	const bool GB_FS_RICOCHET = false;

	const float GB_FS_MIN_DAMAGE = 10.0f;
	const float GB_FS_SWEET_DAMAGE = 20.0f;	
	const float GB_FS_MAX_DAMAGE = 25.0f;

	const float GB_FS_MIN_SPEED = 1000.0f;
	const float GB_FS_SWEET_SPEED = 1600.0f;
	const float GB_FS_MAX_SPEED = 2000.0f;	
	const float GB_FS_MIN_SWEET_BOUNDARY = .806f;
	const float GB_FS_MAX_SWEET_BOUNDARY = .953f;

	//split shot
	//larger projectile, less damage, slower projectile	
	const int SS_FS_PROJSIZE = 20;

	const int SS_FS_MIN_DAMAGE = 8;
	const int SS_FS_SWEET_DAMAGE = 16;	
	const int SS_FS_MAX_DAMAGE = 22;

	const float SS_FS_MIN_SPEED = 800.0f;
	const float SS_FS_SWEET_SPEED = 1200.0f;
	const float SS_FS_MAX_SPEED = 1600.0f;
	
	const float SS_FS_MIN_SWEET_BOUNDARY = .806f;
	const float SS_FS_MAX_SWEET_BOUNDARY = .953f;
	const bool SS_FS_RICOCHET = false;

	//rebounder
	//ricochets, less damage
	const int RB_FS_PROJSIZE = 12;

	const int RB_FS_MIN_DAMAGE = 8;
	const int RB_FS_SWEET_DAMAGE = 16;	
	const int RB_FS_MAX_DAMAGE = 22;

	const float RB_FS_MIN_SPEED = 800.0f;
	const float RB_FS_SWEET_SPEED = 1200.0f;
	const float RB_FS_MAX_SPEED = 1600.0f;
	
	const float RB_FS_MIN_SWEET_BOUNDARY = .806f;
	const float RB_FS_MAX_SWEET_BOUNDARY = .953f;
	const bool RB_FS_RICOCHET = true;

	//muse	
	const int MU_FS_PROJSIZE = 8;
	const bool MU_FS_RICOCHET = false;

	const int MU_FS_MIN_DAMAGE = 10;
	const int MU_FS_SWEET_DAMAGE = 20;	
	const int MU_FS_MAX_DAMAGE = 25;

	const float MU_FS_MIN_SPEED = 1000.0f;
	const float MU_FS_SWEET_SPEED = 1600.0f;
	const float MU_FS_MAX_SPEED = 2000.0f;	
	const float MU_FS_MIN_SWEET_BOUNDARY = .843f;
	const float MU_FS_MAX_SWEET_BOUNDARY = .916f;
}