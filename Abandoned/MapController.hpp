#pragma once
#include <string>
#include "Constants.hpp"
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include "SFML/Graphics.hpp"

// ��������� �����, ���������� �� ��������, ��������� ����� � �������������� � �� ���������.
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
	//��� ������� �����:
	//1. �����������
	//2. ��������� �������
	//3. �������-��������
	int** _activeMap[3];
	//������ ID �������� �� �����, ����� ������� ����� �� ����� ������
	static std::unordered_set<int> idOfCollisionObjs;
	//������ ID � ��������������� �� ����������� �� tileSet'� ������� �����
	static std::unordered_map<int, sf::Vector2i> idOfGroundTextures;
	//������ ID � ��������������� �� ����������� �� tileSet'� ������� ��������
	static std::unordered_map<int, sf::Vector2i> idOfObjsTextures;
public:
	MapController(MapController const&) = delete;
	void operator= (MapController const&) = delete;

	static void getInfoFromFile();
	static MapController* getController();
	
	sf::Vector2i getMapSize();

	//��� direction: 0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT
	//True ���� ���� ������ �� ����������
	bool checkCollision(int direction, sf::Vector2f characterPosition);
	void getMap(const char* mapTitle);
	//��� ��������� ����������� ���� 0, � ��� �������� 1
	void drawMap(sf::RenderWindow& window, int mapLayToDraw, int rowToDraw = -1);
	void loadObstacles();
	
	bool isCollisionObjOnPos(sf::Vector2i position);

	
};

