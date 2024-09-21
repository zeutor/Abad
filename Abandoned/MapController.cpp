#include "MapController.hpp"
#include "Constants.hpp"

MapController* MapController::_mapController = nullptr;
//Список ID объектов на карте, через которые игрок не может пройти
std::unordered_set<int> MapController::idOfCollisionObjs;
//Список ID и соответствующих им координатам на tileSet'е текстур земли
std::unordered_map<int, sf::Vector2i> MapController::idOfGroundTextures;
//Список ID и соответствующих им координатам на tileSet'е текстур объектов
std::unordered_map<int, sf::Vector2i> MapController::idOfObjsTextures;

void MapController::getInfoFromFile() {
	std::ifstream file("data/idInfo/collisionObjs.idinfo");
	int numOfObjs;
	file >> numOfObjs;
	for (int i = 0; i < numOfObjs; ++i)
	{
		int var;
		file >> var;
		idOfCollisionObjs.insert(var);
	}
	file.close();
	std::ifstream file1("data/idInfo/tileSetIDgro.idinfo");
	file1 >> numOfObjs;
	for (int i = 0; i < numOfObjs; ++i)
	{
		int id, x, y;
		file1 >> id >> x >> y;
		idOfGroundTextures.insert({ id, sf::Vector2i(x, y) });
	}
	file1.close();
	std::ifstream file2("data/idInfo/tileSetIDobj.idinfo");
	file2 >> numOfObjs;
	for (int i = 0; i < numOfObjs; ++i)
	{
		int id, x, y;
		file2 >> id >> x >> y;
		idOfObjsTextures.insert({ id, sf::Vector2i(x, y) });
	}
	file2.close();
}

MapController* MapController::getController() {
	if (!_mapController)
		_mapController = new MapController();
	return _mapController;
}

sf::Vector2i MapController::getMapSize()
{
	return _mapSize;
}

bool MapController::checkCollision(int direction, sf::Vector2f characterPosition)
{
	int objID = 0;
	int x = 0;
	int y = 0;
	switch (direction)
	{
	case 0:
		y = ((int)(characterPosition.y - PIXELS_PER_CELL / 2 - BLOCK_EPSILON)) / PIXELS_PER_CELL;
		x = ((int)characterPosition.x) / PIXELS_PER_CELL;
		break;
	case 1:
		y = ((int)characterPosition.y) / PIXELS_PER_CELL;
		x = ((int)(characterPosition.x + PIXELS_PER_CELL / 2 + BLOCK_EPSILON)) / PIXELS_PER_CELL;
		break;
	case 2:
		y = ((int)(characterPosition.y + PIXELS_PER_CELL / 2 + BLOCK_EPSILON)) / PIXELS_PER_CELL;
		x = ((int)characterPosition.x) / PIXELS_PER_CELL;
		break;
	case 3:
		y = ((int)characterPosition.y) / PIXELS_PER_CELL;
		x = ((int)(characterPosition.x - PIXELS_PER_CELL / 2 - BLOCK_EPSILON)) / PIXELS_PER_CELL;	
		break;
	}
	if (idOfCollisionObjs.count(_activeMap[1][y < _mapSize.y ? y : 0][x < _mapSize.x ? x : 0]) != 0)
		return true;
	return false;
}

void MapController::getMap(const char* mapTitle)
{
	
	srand(time(0));
	std::string path = "data/maps/";
	path += mapTitle;
	path += ".map";
	std::ifstream file(path);
	
	if (!file.is_open())
		file.close();
	file >> _mapSize.x;
	file >> _mapSize.y;
	_activeMap[0] = new int* [_mapSize.y];
	for (int i = 0; i < _mapSize.y; ++i)
	{
		_activeMap[0][i] = new int[_mapSize.x];
		for (int j = 0; j < _mapSize.x; ++j)
		{
			int val = 0;
			file >> val;
			if (val == 1)
			{
				float randVal = (float)rand()/ (float)RAND_MAX;
				if (randVal < 0.1)
				{
					val = 17;
				}
				else if (randVal < 0.3) {
					val = 16;
				}
			}
			_activeMap[0][i][j] = val;
		}
			
	}
	_activeMap[1] = new int* [_mapSize.y];
	for (int i = 0; i < _mapSize.y; ++i)
	{
		_activeMap[1][i] = new int[_mapSize.x];
		for (int j = 0; j < _mapSize.x; ++j)
		{
			file >> _activeMap[1][i][j];
		}

	}
	file.close();
}

void MapController::drawMap(sf::RenderWindow& window, int mapLayToDraw)
{
	_tileSet.loadFromFile("data/textures/map/tilemap_packed.png");
	sf::Sprite mapTile;
	mapTile.setTexture(_tileSet);
	if (mapLayToDraw)
	{
		for (int i = 0; i < _mapSize.y; ++i)
		{
			for (int j = 0; j < _mapSize.x; ++j)
			{
				if (_activeMap[1][i][j] == 0)
					continue;
				int xPositionOnTileSet = idOfObjsTextures[_activeMap[1][i][j]].x * 16;
				int yPositionOnTileSet = idOfObjsTextures[_activeMap[1][i][j]].y * 16;
				mapTile.setTextureRect(sf::IntRect(xPositionOnTileSet, yPositionOnTileSet, 16, 16));
				mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
				mapTile.setPosition(j * PIXELS_PER_CELL, i * PIXELS_PER_CELL);
				window.draw(mapTile);
			}
		}
	}
	else {
		for (int i = 0; i < _mapSize.y; ++i)
		{
			for (int j = 0; j < _mapSize.x; ++j)
			{
				int xPositionOnTileSet = idOfGroundTextures[_activeMap[0][i][j]].x * 16;
				int yPositionOnTileSet = idOfGroundTextures[_activeMap[0][i][j]].y * 16;
				mapTile.setTextureRect(sf::IntRect(xPositionOnTileSet, yPositionOnTileSet, 16, 16));
				mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
				mapTile.setPosition(j * PIXELS_PER_CELL, i * PIXELS_PER_CELL);
				window.draw(mapTile);
			}
		}
	}
}

bool MapController::isCollisionObjOnPos(sf::Vector2i position)
{
	if (idOfCollisionObjs.count(_activeMap[1][position.y][position.x]) != 0)
		return true;
	return false;
}
