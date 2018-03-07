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
	for (unsigned int x = 0; x < m_vlevelData.size(); x++)
	{
		for (unsigned int y = 0; y < m_vlevelData[x].size(); y++)
		{
			tile = m_vlevelData[x][y];

			float xPos = (float)x - 10;
			float yPos = (float)y - 20;

			switch (tile)
			{
			case '#': 
				m_vWalls.push_back(new Tile(colour.DarkSlateGray, yPos / 0.56f, -xPos / 0.56f, 5, 0.25f, true));
				break;
			case '.':
				m_vFloor.push_back(new Tile(colour.WhiteSmoke, yPos / 0.56f, -xPos / 0.56f, 5, 0.25f, false));
				break;
			default: //If it gets here, tile hasnt been registered the, so print out a warning
				printf("WARNING: Unknown tile %c at %d,%d", tile, x, y);
				system("Pause");
				break;
			}
		}
	}

}

void Level::Draw(XMMATRIX view, XMMATRIX projection)
{
	for (unsigned int i = 0; i < m_vFloor.size(); i++)
	{
		m_vFloor[i]->Draw(view, projection);
	}

	for (unsigned int i = 0; i < m_vWalls.size(); i++)
	{
		m_vWalls[i]->Draw(view, projection);
	}

}
