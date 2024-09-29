#include "Application.hpp"
#include "SFML/Graphics.hpp"
#include "Outdata.hpp"
#include "Character.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "PlayerController.hpp"


void Application::INIT() {
	_gameWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Abandoned");
	_gameWindow->setFramerateLimit(FRAME_LIMIT);
}

void Application::RUN() {
	outdata::getFiles();
	sf::Vector2f PlayerStartPos(0.f, 0.f);

	// Подгрузка карты
	MapController* mapController = MapController::getController();
	mapController->getMap("devmap2");
	mapController->loadObstacles();

	PlayerStartPos = mapController->getPlayerStartPosition();
	Character player = Character(outdata::tifl_texture,PlayerStartPos, _gameWindow, true);

	sf::Text debugText("", outdata::mainFont, 20);

	sf::Clock deltaClock;
	sf::Clock gameClock;
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
			if (event.type == sf::Event::Closed)
				_gameWindow->close();
		}
		


		player.Update(deltaTime);

		_gameWindow->clear(sf::Color::Black);


		mapController->drawMap(*_gameWindow, 0);
		_gameWindow->draw(player.getSprite());
		mapController->drawMap(*_gameWindow, 1);
		


		// разкоментить, если необходимо отображение обстаклей
		
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
		_gameWindow->display();
	}
}

void Application::CLOSE() {
	if (!_gameWindow)
		delete _gameWindow;
}
