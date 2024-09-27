#include "MapController.hpp"
#include "Constants.hpp"
#include "AStar.hpp"

MapController* MapController::_mapController = nullptr;

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
	if (_activeMap[2][y < _mapSize.y ? y : 0][x < _mapSize.x ? x : 0] != 0)
		return true;
	return false;
}

void MapController::getMap(const char* mapTitle)
{
	
	std::string path = "data/maps/";
	path += mapTitle;
	path += ".map";
	std::ifstream file(path);
	
	if (!file.is_open())
		file.close();
	file >> _playerStartPosition.x;
	file >> _playerStartPosition.y;

	_playerStartPosition *= PIXELS_PER_CELL;

	file >> _mapSize.x;
	file >> _mapSize.y;

	file >> _numOfTileSets;

	_tileSet = new sf::Texture[_numOfTileSets];
	_tileSetSizes = new sf::Vector2i[_numOfTileSets];
	for (int i = 0; i < _numOfTileSets; ++i)
	{
		std::string tileSetName;
		file >> tileSetName;
		tileSetName = "data/textures/map/"+ tileSetName +".png";
		_tileSet[i].loadFromFile(tileSetName);
		file >> _tileSetSizes[i].x;
		file >> _tileSetSizes[i].y;
	}

	for (int k = 0; k < 3; ++k)
	{
		_activeMap[k] = new int* [_mapSize.y];
		for (int i = 0; i < _mapSize.y; ++i)
		{
			_activeMap[k][i] = new int[_mapSize.x];
			for (int j = 0; j < _mapSize.x; ++j)
			{
				int var;
				file >> var;
				if (var == 0)
					int g = 14;
				_activeMap[k][i][j] = var;
			}

		}
	}
	file.close();
}

void MapController::drawMap(sf::RenderWindow& window, int mapLayToDraw, int rowToDraw)
{
	sf::Sprite mapTile;
	if (rowToDraw < 0)
	{
		for (int i = 0; i < _mapSize.y; ++i)
		{
			for (int j = 0; j < _mapSize.x; ++j)
			{
				if (_activeMap[mapLayToDraw][i][j] == 0)
					continue;
				int id = _activeMap[mapLayToDraw][i][j];
				int tileSetId = 0;
				while (id > _tileSetSizes[tileSetId].x * _tileSetSizes[tileSetId].y)
				{					
					id -= _tileSetSizes[tileSetId].x * _tileSetSizes[tileSetId].y;
					++tileSetId;
				}
				
				int xPositionOnTileSet = (id-1) % _tileSetSizes[tileSetId].x * 16;
				int yPositionOnTileSet = (id-1) / _tileSetSizes[tileSetId].x * 16;
				mapTile.setTexture(_tileSet[tileSetId]);
				mapTile.setTextureRect(sf::IntRect(xPositionOnTileSet, yPositionOnTileSet, 16, 16));
				mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
				mapTile.setPosition(j * PIXELS_PER_CELL, i * PIXELS_PER_CELL);
				window.draw(mapTile);
			}
		}
	}
	else {
		for (int j = 0; j < _mapSize.x; ++j)
		{
			if (_activeMap[mapLayToDraw][rowToDraw][j] == 0)
				continue;
			int id = _activeMap[mapLayToDraw][rowToDraw][j];
			int tileSetId = 0;
			while (id > _tileSetSizes[tileSetId].x * _tileSetSizes[tileSetId].y)
			{
				id -= _tileSetSizes[tileSetId].x * _tileSetSizes[tileSetId].y;
				++tileSetId;
			}

			int xPositionOnTileSet = (id - 1) % _tileSetSizes[tileSetId].x * 16;
			int yPositionOnTileSet = (id - 1) / _tileSetSizes[tileSetId].x * 16;
			mapTile.setTexture(_tileSet[tileSetId]);
			mapTile.setTextureRect(sf::IntRect(xPositionOnTileSet, yPositionOnTileSet, 16, 16));
			mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
			mapTile.setPosition(j * PIXELS_PER_CELL, rowToDraw * PIXELS_PER_CELL);
			window.draw(mapTile);
		}
	}
}

void MapController::loadObstacles() {
	for (int i = 0; i < _mapSize.y; ++i)
	{
		for (int j = 0; j < _mapSize.x; ++j)
		{
			if (_activeMap[2][i][j] == 0)
				continue;
			for (int k = 0; k < PIXELS_PER_CELL/PIXELS_FOR_OBSTACLE; ++k)
			{
				AStar::setObstacle(j * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE + k, i * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE);
			}
			for (int k = 0; k < PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE; ++k)
			{
				AStar::setObstacle(j * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE + k, i * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE+ PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE);
			}
			for (int k = 1; k < PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE; ++k)
			{
				AStar::setObstacle(j * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE, i * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE + k);
			}
			for (int k = 1; k < PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE; ++k)
			{
				AStar::setObstacle(j * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE+ PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE, i * PIXELS_PER_CELL / PIXELS_FOR_OBSTACLE + k);
			}
		}
	}
}


bool MapController::isInMapPosition(sf::Vector2i position)
{
	return position.y >= 0 && position.y < _mapSize.y && position.x >= 0 && position.x < _mapSize.x;
}

bool MapController::isCollisionObjOnPos(sf::Vector2i position)
{
	if (isInMapPosition(position))
		return _activeMap[2][position.y][position.x];
	return true;
}

sf::Vector2i MapController::getPlayerStartPosition()
{
	return _playerStartPosition;
}

