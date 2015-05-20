#include "LevelIO.h"


LevelIO::LevelIO(void)
{
}


LevelIO::~LevelIO(void)
{
}

bool LevelIO::loadMap(char* file)
{
	// first find out the file type
	// if its not a valid type throw an error and return false
	int length = getLength(file);
	if(file[length -4] != '.' && file[length - 3] != 't' && file[length - 2] != 'x' && file[length - 1] != 't')
	{
		// bad file type
		// error
		std::cout << "Bad file type. Expected .txt" << std::endl;
		return false;
	}
	
	// since it is of the right file type, as far as we can tell,
	// try opening it and let us see what happens.
	//try
	{
		// grab the lines and go
		std::fstream fIn;
		fIn.open(file, std::ios::in);

		char mapName[6];
		fIn.getline(mapName, 6);
		std::cout << mapName << std::endl;

		// set to 0/(rrandoms) mostly for correct input reading checks (debug stuff)
		int mapWidth = 0;
		int mapHeight = 0;
		width = 100;
		height = 200;
		
		// grab the dimensions so we know how big of a vector we are going to populate
		fIn >> mapWidth >> mapHeight;
		width = mapWidth;
		height = mapHeight;
		std::cout << "Map width, height: " << width << ", " << height << std::endl;

		// resize the vector since we know the size we need
		tiles.resize(width * height);

		// now loop to grab each tile
		// and set it in the vector
		std::cout << "Reading map lines..." << std::endl;
		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				if(fIn.eof())
					break;
				char temp;
				fIn.get(temp);
				if(!(temp == 10 || temp == 0))
				{
					//std::cout << temp << std::endl;
					tiles[(i*height)+j] = temp;
				}
				else
					j--;
				//switch(temp)
				//{
				//case 'x':
				//	// unwalkable area
				//	tiles[(i*height)+j] = 1;
				//	break;
				//case 'f':
				//	// walkable area (floor)
				//	tiles[(i*height)+j] = 2;
				//	break;
				//case 't':
				//	// tree
				//	tiles[(i*height)+j] = 3;
				//	break;
				//case 'r':
				//	// ruin wall/piece
				//	tiles[(i*height)+j] = 4;
				//	break;
				//case 'd':
				//	// door
				//	tiles[(i*height)+j] = 5;
				//	break;
				//case 's':
				//	// player spawn
				//	tiles[(i*height)+j] = 6;
				//	break;
				//case 'm':
				//	// monster spawn point
				//	tiles[(i*height)+j] = 7;
				//	break;
				//case 'b':
				//	// bow pickup
				//	tiles[(i*height)+j] = 8;
				//	break;
				//case 'o':
				//	// boots pickup
				//	tiles[(i*height)+j] = 9;
				//	break;
				//case 'D':
				//	// Destructibles
				//	tiles[(i*height)+j] = 10;
				//}
			}
		}

		// Grab the player spawn
		std::cout << "Finished reading map." << "\nSearching for player spawn..." << std::endl;
		char playerSpawn[16];
		fIn >> playerSpawn;
		// strip out the ps, parenthesis, and comma
		// first number is x tile, second number is y tile
		int length = getLength(playerSpawn);
		if(length > 0)
		{
			// it is known that the first three chars will be "ps("
			// so we can skip that crap -- the letters are just so people can
			// read a map by its text file, all we need are the numbers and people
			// can't really read binary anyway...

			int readPos = 3; // 3 chars skipped, starts at 4th... don't forget 0 is the first index
			// temp arrays for input
			char tempx[6];
			char tempy[6];

			int offset = 0;
			while(playerSpawn[readPos] != ',')
			{
				tempx[offset] = playerSpawn[readPos];
				readPos++;
				offset++;
			}
			readPos++;
			offset = 0;
			while(playerSpawn[readPos] != ')')
			{
				tempy[offset] = playerSpawn[readPos];
				readPos++;
				offset++;
			}

			playerX = atoi(tempx);
			playerY = atoi(tempy);
		}

		bool bFoundPlayerSpawn = true;
		// force a player spawn at 1,1 if there is none or if its off the map
		// this does not account for 1,1 being unwalkable...
		if(playerX < 1){
			playerX = 1;
			bFoundPlayerSpawn = false;
		}
		if(playerY < 1){
			playerY = 1;
			bFoundPlayerSpawn = false;
		}
		
		if(bFoundPlayerSpawn){
			std::cout << "Found player spawn at " << playerX << "," << playerY << "." << "\nSearching for monster spawns..." << std::endl;
		}
		else{
			std::cout << "No player spawn found, forcing player spawn at " << playerX << "," << playerY << "." << "\nSearching for monster spawns..." << std::endl;
		}

		// char array used to store input from file for saving
		char tempSpawns[16];

		numMonsterSpawns = 0;
		fIn >> numMonsterSpawns;
		if(numMonsterSpawns > 0){
			grabEntityPositions(numMonsterSpawns, EntityType::MONSTER_SPAWN, fIn);
		}
		std::cout << numMonsterSpawns << " monster spawns found in map.\nSearching for bow spawns..." << std::endl;

		numBowSpawns = 0;
		fIn >> numBowSpawns;
		if(numBowSpawns > 0){
			grabEntityPositions(numBowSpawns, EntityType::BOW_SPAWN, fIn);
		}
		std::cout << numBowSpawns << " bow spawns found in map.\nMap Conversion complete." << std::endl;

		fIn.close();
	}
	//catch()
	{

	}

	return true;
}

void LevelIO::grabEntityPositions(int numEntities, EntityType entity, std::fstream &stream)
{
	// temp is used to store the whole entity info line (ex: "ms(12,3)" is an entity info line)
	char temp[16];
	int length = 0;// used to store the length of the entity info line

	for(int i = 0; i < numEntities; i++)
	{
		stream >> temp;
		length = getLength(temp);
		if(length > 0)
		{
			//skip first 3 still
			int readPos = 3; // our position in the temp array
			// temp arrays for input
			char tempx[6];
			char tempy[6];

			// grabs the x by searching for things infront of the delimeter ','
			// then gets y based off x position
			int offset = 0; // our position in the tempx array
			while(temp[readPos] != ',')// go until we find the delim
			{
				tempx[offset] = temp[readPos];
				readPos++;
				offset++;
			}
			tempx[offset] = 0;// add null terminator

			// add to readPos to get us past the delim othwerwise we will try to
			// convert a comma to a float and bad things will happen
			readPos++;
			offset = 0;// reset our offset for the new array - tempy
			while(temp[readPos] != ')')
			{
				tempy[offset] = temp[readPos];
				readPos++;
				offset++;
			}
			tempy[offset] = 0;// add null terminator

			// add these spawn positions to the correct entity type spawn list
			if(entity == EntityType::BOOT_SPAWN){}
			else if(entity == EntityType::MONSTER_SPAWN)
			{
				monsterSpawns.push_back(atoi(tempx));
				monsterSpawns.push_back(atoi(tempy));
			}
			else if(entity == EntityType::BOW_SPAWN)
			{
				bowSpawns.push_back(atoi(tempx));
				bowSpawns.push_back(atoi(tempy));
			}
			// clean up the strings otherwise we might get garbage data
			clearString(tempx);
			clearString(tempy);
		}
	}
}

void LevelIO::grabEntityPosition(float *x, float *y)
{

}

int LevelIO::getLength(char* string)
{
	int length = 0;
	while(string[length] != 0)
		length++;
	return length;
}

/**
	clears a string by setting the first position in the array to
	0 and all other positions to the windows unintialized value of -52
		- not sure if this is actually a good idea or if I should just set
		it all to 0s? 
*/
void LevelIO::clearString(char* string)
{
	int length = getLength(string);
	string[0] = 0;
	for(int i = 1; i < length; i++)
	{
		string[i] = -52;
	}
}

// saves the map out to a file (file extension should be included in parameter)
//		- can add extension checking later
// currently all maps will have a map name of "Map 1"
//		- will change this to the file name with the extension stripped
//		- might add way of controlling this (via overloading or else pulling the name from elsewhere)
bool LevelIO::saveMap(char* file)
{
	std::fstream binOut;
	binOut.open(file, std::ios::out|std::ios::binary);

	char mapName[6] = {'M', 'a', 'p', ' ', '0', '\0'};
	binOut.write(mapName, 6);

	// writes out the map dimensions
	binOut.write(reinterpret_cast<const char *>(&width), sizeof(int));
	binOut.write(reinterpret_cast<const char *>(&height), sizeof(int));

	// writes out the tile types
	char tile_t[2] = {'x', '\0'};
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			tile_t[0] = tiles[(i*height)+j];
			if(tiles[(i*height)+j] != 10)
				binOut.write(tile_t, sizeof(char));
		}
	}
	// write out player spawn
	binOut.write(reinterpret_cast<const char *>(&playerX), sizeof(int));
	binOut.write(reinterpret_cast<const char *>(&playerY), sizeof(int));

	// write out monster spawns
	// number of spawns
	binOut.write(reinterpret_cast<const char *>(&numMonsterSpawns), sizeof(int));
	for(int i = 0; i < numMonsterSpawns*2; i+=2)
	{
		binOut.write(reinterpret_cast<const char *>(&monsterSpawns[i]), sizeof(int));
		binOut.write(reinterpret_cast<const char *>(&monsterSpawns[i+1]), sizeof(int));
	}
	
	// write out bow spawns
	binOut.write(reinterpret_cast<const char *>(&numBowSpawns), sizeof(int));
	for(int i = 0; i < numBowSpawns*2; i+=2)
	{
		binOut.write(reinterpret_cast<const char *>(&bowSpawns[i]), sizeof(int));
		binOut.write(reinterpret_cast<const char *>(&bowSpawns[i+1]), sizeof(int));
	}

	binOut.close();
	return true;
}