#pragma once
#include <string>
#include "Constants.hpp"
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include "SFML/Graphics.hpp"

// Сингелтон класс, отвечающий за загрузку, прогрузку карты и взаимодействие с ее объектами.
class MapController
{
private:
	MapController() = default;
	sf::Vector2i _mapSize;
	static MapController* _mapController;
	unsigned int _numOfTileSets;
	sf::Texture* _tileSet;
	sf::Vector2i* _tileSetSizes;
	unsigned int numOfLays;
	//Две таблицы карты:
	//1. Поверхность
	//2. Надземные объекты
	//3. Объекты-коллизии
	int** _activeMap[3];
	//Список ID объектов на карте, через которые игрок не может пройти
	static std::unordered_set<int> idOfCollisionObjs;
	//Список ID и соответствующих им координатам на tileSet'е текстур земли
	static std::unordered_map<int, sf::Vector2i> idOfGroundTextures;
	//Список ID и соответствующих им координатам на tileSet'е текстур объектов
	static std::unordered_map<int, sf::Vector2i> idOfObjsTextures;
public:
	MapController(MapController const&) = delete;
	void operator= (MapController const&) = delete;

	static void getInfoFromFile();
	static MapController* getController();
	
	sf::Vector2i getMapSize();

	//Для direction: 0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT
	//True если есть объект по наравлению
	bool checkCollision(int direction, sf::Vector2f characterPosition);
	void getMap(const char* mapTitle);
	//Для отрисовки поверхности слой 0, а для объектов 1
	void drawMap(sf::RenderWindow& window, int mapLayToDraw, int rowToDraw = -1);
	void loadObstacles();
	
	bool isCollisionObjOnPos(sf::Vector2i position);

	
};

