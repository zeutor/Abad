#pragma once
#include <string>
#include "Constants.hpp"
#include <fstream>
#include "SFML/Graphics.hpp"

class MapController
{
private:
	MapController() = default;
	sf::Vector2i _mapSize;
	static MapController* _mapController;
	sf::Texture _tileSet;
	int** _activeMap[2];
public:
	MapController(MapController const&) = delete;
	void operator= (MapController const&) = delete;

	static MapController* getController();

	void getMap(const char* mapTitle);
	void drawMap(sf::RenderWindow& window);
};

