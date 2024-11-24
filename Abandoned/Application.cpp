#include "Application.hpp"
#include "SFML/Graphics.hpp"
#include "Outdata.hpp"
#include "Character.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "PlayerController.hpp"
#include "GameCamera.hpp"
#include "Object.hpp"
#include "UIManager.hpp"
#include "MapObject.hpp"


void Application::INIT() {
	_gameWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Abandoned");
	_gameWindow->setFramerateLimit(FRAME_LIMIT);
}

void Application::RUN() {
	outdata::getFiles();

	// ��������� �����
	MapController* mapController = MapController::getController();
	mapController->getMap("devmap3");
	mapController->loadObstacles();
	sf::Vector2f PlayerStartPos = mapController->getPlayerStartPosition();
	
	// ��������� ������
	GameCamera::INIT(_gameWindow);
	GameCamera gameCamera;
	gameCamera.setAsMain();
	gameCamera.setBorders(sf::FloatRect(0, 0, mapController->getMapSize().x * PIXELS_PER_CELL, mapController->getMapSize().y * PIXELS_PER_CELL));
	gameCamera.setSpeed(100);

	Character player = Character(outdata::tifl_texture,PlayerStartPos, _gameWindow, true);

	sf::Text debugText("", outdata::mainFont, 20);

	sf::Clock deltaClock;
	sf::Clock gameClock;
	

	
	sf::View mainView = _gameWindow->getDefaultView();
	UIManager* UIController = UIManager::getController();
	UIController->setWindowToDisplay(_gameWindow);
	UIController->LoadIcons({ outdata::menu_icon,  outdata::invent_icon, outdata::journal_icon }, *_gameWindow);

	std::vector<UISlot> UISlots = UIController->getInvConroller();
	int CountOfSlots = UISlots.size();

	Object::Load("data\\objects\\Objects.obj");
	std::unordered_set<Object*> AllObject = Object::getAllObjects();
	sf::Vector2f posOfObj(PIXELS_PER_CELL*5, PIXELS_PER_CELL*7);

	LoadMapObjects("data\\objects\\MapObjects.mapobj");
	std::unordered_set<MapObject*> AllMapObject = MapObject::getAllMapObjects();

	bool isMouseReleased = true;

	while (_gameWindow->isOpen()) {
		float deltaTime = deltaClock.restart().asSeconds();
		deltaClock.restart();

		std::string debugString;
		debugString += "Steps: " + std::to_string(player.getDistance() / PIXELS_PER_METER) + '\n';
		debugString += "FPS: " + std::to_string(1./deltaTime) + '\n';
		debugString += "Seconds: " + std::to_string((int)gameClock.getElapsedTime().asSeconds()) + '\n';
		debugString += "Player pos: " + std::to_string(Character::getPlayer()->getPosition().x) + " " + std::to_string(Character::getPlayer()->getPosition().y);

		debugText.setString(debugString);

		deltaTime *= TIME_MULTIPLIER;

		sf::Event event;
		while (_gameWindow->pollEvent(event))
		{
			UIController->Listen(event, player, *_gameWindow);
			if (event.type == sf::Event::Closed)
				_gameWindow->close();
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::W: {
					gameCamera.move({ 0, -1 }, deltaTime);
					break;
				}
				case sf::Keyboard::D: {
					gameCamera.move({ 1, 0 }, deltaTime);
					break;
				}
				case sf::Keyboard::S: {
					gameCamera.move({ 0, 1 }, deltaTime);
					break;
				}
				case sf::Keyboard::A: {
					gameCamera.move({ -1, 0 }, deltaTime);
					break;
				}
				default:
					break;
				}
			}
		}
		
		GameCamera::updateView();

		UIController->Update();

		if (!UIController->isWindowOpen(4) && !UIController->isWindowOpen(0) && !UIController->isWindowOpen(5)) {
			player.Update(deltaTime);
		}
		
		_gameWindow->clear(sf::Color::Black);

		mapController->drawMap(*_gameWindow, 0);
		_gameWindow->draw(player.getSprite());
		mapController->drawMap(*_gameWindow, 1);

		// ��c���������, ���� ���������� ����������� ���������
		
		//vector<Vector2f> obst = AStar::getObstacles();
		//int len = obst.size();
		//for (int i = 0; i < len; ++i)
		//{
		//	sf::CircleShape circ;
		//	circ.setFillColor(Color::Red);
		//	circ.setRadius(1);
		//	circ.setPosition(obst[i].x, obst[i].y);
		//	_gameWindow->draw(circ);
		//}

		_gameWindow->draw(debugText);
		UIController->LoadGameUI(event, player);
		_gameWindow->display();
	}
}

void Application::CLOSE() {
	if (!_gameWindow)
		delete _gameWindow;
}
