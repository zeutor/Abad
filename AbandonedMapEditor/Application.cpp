#include "Application.hpp"
#include "Constants.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

bool renderMapLay[3]{};
int** mapArray[3];

sf::Vector2i _mapSize;

sf::Texture* _tileSet;
sf::Vector2i *_tileSetSizes;

string* tileSetTitles;

int _numOfTileSets;

/// <summary>
/// Отрисовывает карту на экране в соответвтвии с позицией камеры, отображаемым слоем
/// </summary>
/// <param name="camPos">Позиция камеры.</param>
/// <param name="zeroPos">Позиция начала карты</param>
/// <param name="lay">Слой для отрисовки</param>
/// <param name="x">Максимальная координата x для карты.</param>
/// <param name="y">Максимальная координата y для карты.</param>
/// <param name="window">Окно на котором будет отображение.</param>
void renderMap(sf::Vector2i camPos, sf::Vector2i zeroPos, int lay, int x, int y, sf::RenderWindow* window) {
	sf::Sprite mapTile;
	if (lay < 2)
	{
		for (int i = 0; i < y; ++i)
		{
			for (int j = 0; j < x; ++j)
			{
				int toCheckX = j - camPos.x/PIXELS_PER_CELL;
				int toCheckY = i - camPos.y / PIXELS_PER_CELL;
				if (toCheckX >= _mapSize.x || toCheckY >= _mapSize.y || toCheckX < 0 || toCheckY < 0)
					continue;
				if (mapArray[lay][toCheckY][toCheckX] == 0)
					continue;
				int id = mapArray[lay][toCheckY][toCheckX];
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
				mapTile.setScale(SIZE_MULTIPYER, SIZE_MULTIPYER);
				mapTile.setPosition(j * PIXELS_PER_CELL + (zeroPos).x, i * PIXELS_PER_CELL + (zeroPos).y);
				window->draw(mapTile);
			}
		}
	}
	else {
		for (int i = 0; i < y; ++i)
		{
			for (int j = 0; j < x; ++j)
			{
				if (j >= _mapSize.x || i >= _mapSize.y)
					continue;
				if (mapArray[lay][i][j] == 0)
					continue;

				sf::RectangleShape rect;
				rect.setFillColor(sf::Color::Red);
				rect.setPosition(j * PIXELS_PER_CELL + (camPos + zeroPos).x, i * PIXELS_PER_CELL + (camPos + zeroPos).y);
				rect.setSize(sf::Vector2f(PIXELS_PER_CELL, PIXELS_PER_CELL));
				window->draw(rect);
			}
		}
	}
	
}

/// <summary>
/// Подгружает карту из "сырого" файла (шаблон в папке data/maps/) для работы.
/// </summary>
/// <param name="mapTitle">Название файла (только map title) {map title}.rawmap</param>
/// <returns>Успешность открытия файла.</returns>
bool loadMapFromFile(string mapTitle)
{
	string path = "data/maps/";
	path += mapTitle;
	path += ".rawmap";
	std::ifstream file(path);
	if (!file.is_open())
		return false;

	file >> _mapSize.x;
	file >> _mapSize.y;

	file >> _numOfTileSets;

	tileSetTitles = new string[_numOfTileSets];

	_tileSet = new sf::Texture[_numOfTileSets];
	_tileSetSizes = new sf::Vector2i[_numOfTileSets];
	for (int i = 0; i < _numOfTileSets; ++i)
	{
		std::string tileSetName;
		file >> tileSetName;
		tileSetTitles[i] = tileSetName;
		tileSetName = "data/textures/map/" + tileSetName + ".png";
		_tileSet[i].loadFromFile(tileSetName);
		file >> _tileSetSizes[i].x;
		file >> _tileSetSizes[i].y;
	}

	for (int k = 0; k < 3; ++k)
	{
		mapArray[k] = new int* [_mapSize.y];
		for (int i = 0; i < _mapSize.y; ++i)
		{
			mapArray[k][i] = new int[_mapSize.x];
			for (int j = 0; j < _mapSize.x; ++j)
			{
				int var;
				file >> var;
				if (var == 0)
					int g = 14;
				mapArray[k][i][j] = var;
			}

		}
	}
	file.close();
	return true;
}

/// <summary>
/// Сохраняет изменения в сыром файле карты.
/// </summary>
/// <param name="mapTitle">Название карты</param>
/// <returns>Успешность открытия файла.</returns>
bool saveMap(string mapTitle)
{
	string path = "data/maps/";
	path += mapTitle;
	path += ".rawmap";
	std::ofstream file(path);
	if (!file.is_open())
		return false;

	file << _mapSize.x << " ";
	file << _mapSize.y << "\n\n";

	file << _numOfTileSets << "\n\n";

	for (int i = 0; i < _numOfTileSets; ++i)
	{
		file << tileSetTitles[i] << " " << _tileSetSizes[i].x << " " << _tileSetSizes[i].y << "\n";
	}

	for (int k = 0; k < 3; ++k)
	{
		for (int i = 0; i < _mapSize.y; ++i)
		{
			for (int j = 0; j < _mapSize.x; ++j)
			{
				file << mapArray[k][i][j] << " ";
			}
			file << "\n";
		}
		file << "\n";
	}
	file.close();
	return true;
}

/// <summary>
/// Отрисовывает в левом меню плитки для выбора.
/// </summary>
/// <param name="page">Страница меню.</param>
/// <param name="row">Число строк плиток.</param>
/// <param name="columns">Число столбцов плиток.</param>
/// <param name="startPos">Откуда начинать отрисовыавать.</param>
/// <param name="window">Окно, на котором отобразить.</param>
void loadTiles(int page, int row, int columns, sf::Vector2f startPos, sf::RenderWindow* window) {
	sf::Sprite mapTile;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			int id = page * row * columns +columns * i + j + 1;
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
			mapTile.setScale(3,3);
			mapTile.setPosition(startPos + sf::Vector2f(j * PIXELS_PER_CELL_DEF * (3+1), i * PIXELS_PER_CELL_DEF* (3 + 1)));
			window->draw(mapTile);
		}
	}
}

/// <summary>
/// Определяет id плитки, по которой нажал пользователь в левом меню.
/// </summary>
/// <param name="mp">Позиция мыши.</param>
/// <param name="page">Номер страницы.</param>
/// <param name="row">Число строк плиток.</param>
/// <param name="columns">Число колонок плиток.</param>
/// <param name="startPos">Позиция откуда начали отрисовываться плитки.</param>
/// <returns>ID плитки, по которой нажали.</returns>
int getIdOfPlate(sf::Vector2i mp, int page, int row, int columns, sf::Vector2i startPos)
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			int id = page * row * columns + columns * i + j + 1;
			sf::Vector2i zero = startPos + sf::Vector2i(j * PIXELS_PER_CELL_DEF * (3 + 1), i * PIXELS_PER_CELL_DEF * (3 + 1));
			if (sf::IntRect(zero.x,zero.y, PIXELS_PER_CELL_DEF*3, PIXELS_PER_CELL_DEF * 3).contains(mp))
			{
				return id;
			}
		}
	}
	return 0;
}

/// <summary>
/// Заполняет карту от первой точки до второй плиткой с номером ID на слое lay.
/// </summary>
/// <param name="fP">Начальная позиция для заполнения.</param>
/// <param name="sP">Конечная позиция для заполнения.</param>
/// <param name="id">Номер плитки, которой нужно заплнить.</param>
/// <param name="lay">Слой, на котором будет отрисовка.</param>
void fillInZone(sf::Vector2i fP, sf::Vector2i sP, int id, int lay) {
	for (int i = (fP.y < sP.y ? fP.y : sP.y); i <= (fP.y > sP.y ? fP.y : sP.y); ++i)
	{
		for (int j = (fP.x < sP.x ? fP.x : sP.x); j <= (fP.x > sP.x ? fP.x : sP.x); ++j)
		{
			mapArray[lay][i][j] = id;
		}
	}
}

void Application::INIT()
{
	_applicationWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "AME");
	_applicationWindow->setFramerateLimit(MAX_FRAME_RATE);
	_applicationWindow->setKeyRepeatEnabled(false);
}

bool Application::RUN()
{
	vector<sf::RectangleShape> torender;

	bool enteringMapName = false;
	bool isMapLoaded = false;

	bool fillCursor = false;
	bool firstPosReady = false;
	sf::Vector2i firstPoint;


	bool drawGround = true;
	bool drawObjs = true;
	bool drawObstacles = false;

	int activeId = 0;

	sf::Font font;
	font.loadFromFile("data/fonts/tahoma.ttf");

	sf::Event event;

	sf::RectangleShape topPanel(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT/15));
	topPanel.setFillColor(sf::Color::Black);
	torender.push_back(topPanel);

	sf::RectangleShape leftPanel(sf::Vector2f(WINDOW_WIDTH/4, WINDOW_HEIGHT));
	leftPanel.setFillColor(sf::Color::Black);
	torender.push_back(leftPanel);

	sf::RectangleShape mapNameField(sf::Vector2f(300, 30));
	mapNameField.setFillColor(sf::Color::White);
	mapNameField.setPosition(10, 12);
	torender.push_back(mapNameField);

	sf::RectangleShape plusPage(sf::Vector2f(30, 30));
	plusPage.setFillColor(sf::Color::Red);
	plusPage.setPosition(WINDOW_WIDTH / 4 - 40, WINDOW_HEIGHT - 40);
	torender.push_back(plusPage);

	sf::RectangleShape minusPage(sf::Vector2f(30, 30));
	minusPage.setFillColor(sf::Color::Red);
	minusPage.setPosition(10, WINDOW_HEIGHT - 40);
	torender.push_back(minusPage);

	sf::Text mapName;
	mapName.setString("mapname");
	mapName.setFillColor(sf::Color::Black);
	mapName.setFont(font);
	mapName.setPosition(sf::Vector2f(12,12));
	mapName.setCharacterSize(20);

	sf::Text cursorPos;
	cursorPos.setString("0,0");
	cursorPos.setFillColor(sf::Color::White);
	cursorPos.setFont(font);
	cursorPos.setCharacterSize(20);

	sf::Text Lays;
	Lays.setString("GROUND: 1; OBJ: 1; OBS: 1");
	Lays.setFillColor(sf::Color::White);
	Lays.setFont(font);
	Lays.setCharacterSize(20);
	Lays.setPosition(sf::Vector2f(340, 12));

	sf::Text idText;
	idText.setString(to_string(activeId));
	idText.setFillColor(sf::Color::White);
	idText.setFont(font);
	idText.setCharacterSize(20);
	idText.setPosition(sf::Vector2f(740, 12));

	sf::Text fillText;
	fillText.setString("Point");
	fillText.setFillColor(sf::Color::White);
	fillText.setFont(font);
	fillText.setCharacterSize(20);
	fillText.setPosition(sf::Vector2f(940, 12));


	sf::Vector2i zeroPosition = sf::Vector2i(WINDOW_WIDTH / 4 + PIXELS_PER_CELL, PIXELS_PER_CELL + WINDOW_HEIGHT / 15);

	sf::Vector2i cameraPos(0, 0);

	int page = 0;

	sf::Clock deltaClock;
	while (_applicationWindow->isOpen())
	{
		while (_applicationWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				return false;
			// Ввод названия карты в поле названия.
			if (enteringMapName && event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode > 39)  // отсекаем не ASCII символы
					mapName.setString(mapName.getString() + static_cast<char>(event.text.unicode));
				if (event.text.unicode == 13)
				{
					enteringMapName = false;
					if (!loadMapFromFile(mapName.getString()))
					{
						mapName.setString("No such file");
					}
					else {
						isMapLoaded = true;
					}
				}
			}
			// Проверка на нажание клавиш.
			if (event.type == sf::Event::KeyPressed && !enteringMapName)
			{
				/// WASD перемещение камеры.
				if (event.key.code == sf::Keyboard::D)
				{
					_applicationWindow->setKeyRepeatEnabled(true);
					--cameraPos.x;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					_applicationWindow->setKeyRepeatEnabled(true);
					--cameraPos.y;
				}
				if (event.key.code == sf::Keyboard::A)
				{
					_applicationWindow->setKeyRepeatEnabled(true);
					++cameraPos.x;
				}
				if (event.key.code == sf::Keyboard::W)
				{
					_applicationWindow->setKeyRepeatEnabled(true);
					++cameraPos.y;
				}

				// 1-3 выкл/вкл слоев.
				if (event.key.code == sf::Keyboard::Num1)
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					drawGround = !drawGround;
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					drawObjs = !drawObjs;
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					drawObstacles = !drawObstacles;
				}

				// Приближение/отдаление карты.
				if (event.key.code == sf::Keyboard::Add)
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					SIZE_MULTIPYER += 1;
				}
				if (event.key.code == sf::Keyboard::Subtract)
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					if(SIZE_MULTIPYER > 1)
						SIZE_MULTIPYER -= 1;
				}
					 
				// Сохрание карты.
				if (event.key.code == sf::Keyboard::P && isMapLoaded)
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					saveMap(mapName.getString());
				}

				// Вкл/выкл режима заливки.
				if (event.key.code == sf::Keyboard::F && isMapLoaded)
				{
					fillCursor = !fillCursor;
					firstPosReady = false;
				}
			}

			// Нажатие мышкой.
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i mp = sf::Mouse::getPosition(*_applicationWindow);
				// mapname field
				if (sf::IntRect(12, 12, 300, 30).contains(mp))
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					enteringMapName = !enteringMapName;
					if (enteringMapName)
					{
						isMapLoaded = false;
						mapName.setString("");
					}
					if (!enteringMapName)
					{
						if (!loadMapFromFile(mapName.getString()))
						{
							mapName.setString("No such file");
						}
						else {
							isMapLoaded = true;
						}
					}
				}
				// plus page
				else if (sf::IntRect(WINDOW_WIDTH / 4 - 40, WINDOW_HEIGHT - 40, 30, 30).contains(mp))
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					++page;
				}
				// minus page
				else if (sf::IntRect(10, WINDOW_HEIGHT - 40, 30, 30).contains(mp))
				{
					_applicationWindow->setKeyRepeatEnabled(false);
					if(page >0)
						--page;
				}
				// Create zone
				else if (sf::IntRect(zeroPosition.x, zeroPosition.y, 23 * 48, 23 * 48).contains(mp))
				{
					_applicationWindow->setKeyRepeatEnabled(true);
					sf::Vector2i cellPos = (mp - zeroPosition ) / PIXELS_PER_CELL - cameraPos - sf::Vector2i(mp.x < 0 ? 1 : 0, mp.y < 0 ? 1 : 0);
					if (cellPos.x >= 0 && cellPos.y >= 0 && cellPos.x < _mapSize.x && cellPos.y < _mapSize.y && activeId > 0)
					{
						if (!fillCursor)
						{
							if (drawObstacles)
							{
								if (activeId > 1)
								{
									mapArray[2][cellPos.y][cellPos.x] = 1;
								}
								else {
									mapArray[2][cellPos.y][cellPos.x] = 0;
								}
							}
							else if (drawObjs)
							{
								if (activeId > 1)
								{
									mapArray[1][cellPos.y][cellPos.x] = activeId;
								}
								else
								{
									mapArray[1][cellPos.y][cellPos.x] = 0;
								}
							}
							else if (drawGround) {
								mapArray[0][cellPos.y][cellPos.x] = activeId;
							}
						}
						else {
							if (firstPosReady)
							{
								if (drawObstacles)
								{
									if (activeId > 1)
									{
										fillInZone(firstPoint, cellPos, 1, 2);
									}
									else {
										fillInZone(firstPoint, cellPos, 0, 2);
									}
								}
								else if (drawObjs)
								{
									if (activeId > 1)
									{
										fillInZone(firstPoint, cellPos,activeId, 1);

									}
									else
									{
										fillInZone(firstPoint, cellPos, 0, 1);

									}
								}
								else if (drawGround) {
									fillInZone(firstPoint, cellPos, activeId, 0);

								}
								firstPosReady = !firstPosReady;
							}
							else {
								firstPoint = cellPos;
								firstPosReady = true;
							}
						}
					}
				}
				// Get new id
				else {
					_applicationWindow->setKeyRepeatEnabled(false);
					int newId = getIdOfPlate(mp,page,12,6,sf::Vector2i(10,60));
					if (newId > 0)
						activeId = newId;
				}
			}
		}

		PIXELS_PER_CELL = PIXELS_PER_CELL_DEF * SIZE_MULTIPYER;

		_applicationWindow->clear(sf::Color::Blue);
		if (isMapLoaded)
		{
			if (drawGround)
				renderMap(cameraPos * PIXELS_PER_CELL, zeroPosition, 0, _mapSize.x, _mapSize.y, _applicationWindow);
			if (drawObjs)
				renderMap(cameraPos * PIXELS_PER_CELL, zeroPosition, 1, _mapSize.x, _mapSize.y, _applicationWindow);
			if (drawObstacles)
				renderMap(cameraPos * PIXELS_PER_CELL, zeroPosition, 2, _mapSize.x, _mapSize.y, _applicationWindow);
		}


		if (fillCursor)
			fillText.setString("Fill");
		else
			fillText.setString("Point");

		for (auto iterator = torender.begin(); iterator != torender.end(); ++iterator)
			_applicationWindow->draw(*iterator);
		_applicationWindow->draw(mapName);
		if (isMapLoaded)
		{
			loadTiles(page, 12, 6, sf::Vector2f(10, 60), _applicationWindow);
		}

		// Координаты курсора.
		sf::Vector2i mousePosit = sf::Mouse::getPosition(*_applicationWindow);
		cursorPos.setPosition(sf::Vector2f(mousePosit.x + 20, mousePosit.y + 20));
		mousePosit = (mousePosit - zeroPosition)/ PIXELS_PER_CELL - cameraPos - sf::Vector2i(mousePosit.x < 0 ? 1 : 0, mousePosit.y < 0 ? 1 : 0);
		cursorPos.setString(to_string(mousePosit.x) + "," + to_string(mousePosit.y));
		_applicationWindow->draw(cursorPos);

		// Какие слоя активны.
		Lays.setString("GROUND: " + to_string(drawGround) + "; OBJ: " + to_string(drawObjs) + "; OBS: " + to_string(drawObstacles));
		idText.setString(to_string(activeId));
		_applicationWindow->draw(Lays);

		_applicationWindow->draw(idText);
		_applicationWindow->draw(fillText);

		_applicationWindow->display();
	}

	return true;
}

void Application::CLOSE()
{
	_applicationWindow->close();
}
