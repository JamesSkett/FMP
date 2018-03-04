#include "Level.h"

#include <fstream>


Level::Level()
{
}


Level::~Level()
{
}

void Level::LoadLevelData(string filePath)
{
	//Loads the level
	ifstream file;

	//Open the file and quit if it fails to open
	file.open(filePath);
	if (file.fail()) 
	{
		perror(filePath.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;

	//Loop through the entire file, getting each row and
	//putting it in the line string.
	while (getline(file, line)) {
		//Push the current line onto the _levelData array
		m_vlevelData.push_back(line);
	}

	//Close the file so we arent keeping open for longer than we need to
	file.close();

}

void Level::SetUpLevelLayout()
{
	char tile;
	//loops through the _levelData vector and processes each tile
	for (int i = 0; i < m_vlevelData.size(); i++)
	{
		for (int j = 0; j < m_vlevelData[i].size(); j++)
		{
			tile = m_vlevelData[i][j];

			switch (tile)
			{
			case '#': 
			case '.':
				//Doesnt need to do anthing, just break.
				break;
			default: //If it gets here, tile hasnt been registered the, so print out a warning
				printf("WARNING: Unknown tile %c at %d,%d", tile, j, i);
				system("Pause");
				break;
			}
		}
	}

}
