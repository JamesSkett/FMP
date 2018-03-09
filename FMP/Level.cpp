#include "Level.h"

#include <fstream>

Level::Level()
{

}


Level::~Level()
{
	
	m_vlevelData.clear();
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

void Level::SetUpLevelLayout(vector <Tile*> &vWalls, vector <Tile*> &vFloor, Player* &player)
{
	char tile;
	//loops through the _levelData vector and processes each tile
	for (unsigned int x = 0; x < m_vlevelData.size(); x++)
	{
		for (unsigned int y = 0; y < m_vlevelData[x].size(); y++)
		{
			tile = m_vlevelData[x][y];

			//move the tiles so they display from top left corner
			float xPos = (float)x - 10;
			float yPos = (float)y - 20;

			//check the current tile
			switch (tile)
			{
			case '#': //create a wall tile
				vWalls.push_back(new Tile(colour.DarkSlateGray, yPos / m_tileOffset, -xPos / m_tileOffset, 5, 0.25f, 0.25f, 0.25f, true));
				break;
			case '.': //create a floor tile
				vFloor.push_back(new Tile(colour.WhiteSmoke, yPos / m_tileOffset, -xPos / m_tileOffset, 5, 0.25f, 0.25f,0.25f, false));
				break;
			case '@':
				vFloor.push_back(new Tile(colour.WhiteSmoke, yPos / m_tileOffset, -xPos / m_tileOffset, 6, 0.25f, 0.25f, 0.25f, false));
				player = new Player(colour.DarkCyan, yPos / m_tileOffset, -xPos / m_tileOffset, 1, 0.125f, 0.125f, 0.125f);
				break;
			default: //If it gets here, tile hasnt been registered the, so print out a warning
				printf("WARNING: Unknown tile %c at %d,%d", tile, x, y);
				system("Pause");
				break;
			}
		}
	}



}
