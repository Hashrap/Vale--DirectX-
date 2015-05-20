#include "LevelIO.h"

int main()
{
	LevelIO levelCon;
	
	char in[40];

	std::cout << "Map name: ";
	std::cin >> in;
	int length = levelCon.getLength(in);
	if(in[length -4] == '.')
	{
		if(in[length - 3] != 't' && in[length - 2] != 'x' && in[length - 1] != 't')
		{
			std::cout << "Bad file type. Expected .txt, recieved " << in[length - 3] << in[length - 2] << in[length - 1] << "." << std::endl;
			return 0;
		}
	}
	else
	{
		in[length] = '.';
		in[length+1] = 't';
		in[length+2] = 'x';
		in[length+3] = 't';
		in[length+4] = 0;
	}
	levelCon.loadMap(in);
	// replace txt with vmp
	in[length+1] = 'v';
	in[length+2] = 'm';
	in[length+3] = 'p';
	levelCon.saveMap(in);

	std::cin >> in;
	return 0;
}