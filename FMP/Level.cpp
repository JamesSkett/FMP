#include "Level.h"
#include "Floor.h"
#include "Wall.h"
#include "Renderer.h"

#include "Tile.h"
#include "Player.h"
#include "Monster.h"

#include <fstream>

Level::Level()
{

}


Level::~Level()
{
	
	m_vlevelData.clear();
}

void Level::LoadLevelData(const char* filePath)
{
	//Loads the level
	std::ifstream file;

	//Open the file and quit if it fails to open
	file.open(filePath);
	if (file.fail()) 
	{
		perror(filePath);
		system("PAUSE");
		exit(1);
	}

	std::string line;

	//Loop through the entire file, getting each row and
	//putting it in the line string.
	while (getline(file, line)) {
		//Push the current line onto the _levelData array
		m_vlevelData.push_back(line);
	}

	//Close the file so we arent keeping open for longer than we need to
	file.close();

}

void Level::SetUpLevelLayout(std::vector <Tile*> &tilemap, Player* &player, Monster* &monster)
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
				tilemap.push_back(new Wall(yPos / m_tileOffset, 0.f, -xPos / m_tileOffset, 0.23f, 0.34f, 0.34f, 0.34f, 2));
				break;
			case '.': //create a floor tile
				tilemap.push_back(new Floor(yPos / m_tileOffset, -0.22f, -xPos / m_tileOffset, 0.23f, 0.29f, 0.29f, 0.29f, 1));
				break;
			case 'd': //create a floor tile
				tilemap.push_back(new Floor(yPos / m_tileOffset, -0.22f, -xPos / m_tileOffset, 0.23f, 0.29f, 0.29f, 0.29f, 3));
				break;
			case '@':
				tilemap.push_back(new Floor(yPos / m_tileOffset, -0.22f, -xPos / m_tileOffset, 0.23f, 0.29f, 0.29f, 0.29f, 1));
				player = new Player(yPos / m_tileOffset, -0.1f, -xPos / m_tileOffset, 0.125f, 0.245f, 0.245f);
				break;
			case 'M':
				tilemap.push_back(new Floor(yPos / m_tileOffset, -0.22f, -xPos / m_tileOffset, 0.23f, 0.29f, 0.29f, 0.29f, 1));
				monster = new Monster(yPos / m_tileOffset, -xPos / m_tileOffset, 10.f, 0.125f, 0.245f, 0.245f);
				break;
			default: //If it gets here, tile hasnt been registered the, so print out a warning
				char s[128];
				sprintf_s(s, "ERROR! Unknown character on tile %d, %d", x, y);
				OutputDebugString(s);
				break;
			}
		}
	}

	

}
