#include "MapController.hpp"

MapController* MapController::_mapController = nullptr;

MapController* MapController::getController() {
	if (!_mapController)
		_mapController = new MapController();
	return _mapController;
}

void MapController::getMap(const char* mapTitle)
{
	
	srand(time(0));
	std::string path = "maps/";
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
					val = 11;
				}
				else if (randVal < 0.3) {
					val = 12;
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

void MapController::drawMap(sf::RenderWindow& window)
{
	_tileSet.loadFromFile("./textures/map/tilemap_packed.png");
	sf::Sprite mapTile;
	mapTile.setTexture(_tileSet);
	for (int i = 0; i < _mapSize.y; ++i)
	{
		for (int j = 0; j < _mapSize.x; ++j)
		{
			switch (_activeMap[0][i][j])
			{
			case 1:
				mapTile.setTextureRect(sf::IntRect(0, 0, 16, 16));
				break;
			case 12:
				mapTile.setTextureRect(sf::IntRect(32, 0, 16, 16));
				break;
			case 11:
				mapTile.setTextureRect(sf::IntRect(16, 0, 16, 16));
				break;
			case 2:
				mapTile.setTextureRect(sf::IntRect(16, 32, 16, 16));
				break;
			case 3:
				mapTile.setTextureRect(sf::IntRect(0, 16, 16, 16));
				break;
			case 4:
				mapTile.setTextureRect(sf::IntRect(16, 16, 16, 16));
				break;
			case 5:
				mapTile.setTextureRect(sf::IntRect(32, 16, 16, 16));
				break;
			case 6:
				mapTile.setTextureRect(sf::IntRect(32, 32, 16, 16));
				break;
			case 7:
				mapTile.setTextureRect(sf::IntRect(32, 48, 16, 16));
				break;
			case 8:
				mapTile.setTextureRect(sf::IntRect(16, 48, 16, 16));
				break;
			case 9:
				mapTile.setTextureRect(sf::IntRect(0, 48, 16, 16));
				break;
			case 10:
				mapTile.setTextureRect(sf::IntRect(0, 32, 16, 16));
				break;
			}
			mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
			mapTile.setPosition(j * PIXELS_PER_CELL, i * PIXELS_PER_CELL);
			window.draw(mapTile);
		}
	}
	for (int i = 0; i < _mapSize.y; ++i)
	{
		for (int j = 0; j < _mapSize.x; ++j)
		{
			switch (_activeMap[1][i][j])
			{
			case 1:
				mapTile.setTextureRect(sf::IntRect(80, 0, 16, 16));
				break;
			case 2:
				mapTile.setTextureRect(sf::IntRect(64, 16, 16, 16));
				break;
			case 3:
				mapTile.setTextureRect(sf::IntRect(64, 0, 16, 16));
				break;
			default:
				continue;
			}
			mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
			mapTile.setPosition(j * PIXELS_PER_CELL, i * PIXELS_PER_CELL);
			window.draw(mapTile);
		}
	}
}
