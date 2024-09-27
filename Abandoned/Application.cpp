#include "Application.hpp"
#include "SFML/Graphics.hpp"
#include "Outdata.hpp"
#include "Player.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "PlayerController.hpp"


void Application::INIT() {
	_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Abandoned");
	_window->setFramerateLimit(FRAME_LIMIT);
}

void Application::RUN() {
	outdata::getFiles();

	Player* player = new Player(outdata::player_texture, sf::Vector2f(0,0), *_window);

	// Подгрузка карты
	MapController* mapController = MapController::getController();
	mapController->getMap("devmap2");
	mapController->loadObstacles();

	player->setPosition(sf::Vector2f(mapController->getPlayerStartPosition().x, mapController->getPlayerStartPosition().y));

	sf::Text debugText("", outdata::mainFont, 20);

	sf::Clock deltaClock;
	sf::Clock gameClock;
	while (_window->isOpen()) {
		float deltaTime = deltaClock.restart().asSeconds();
		deltaClock.restart();

		std::string debugString;
		debugString += "Steps: " + std::to_string(player->getDistance() / PIXELS_PER_METER) + '\n';
		debugString += "FPS: " + std::to_string(1./deltaTime) + '\n';
		debugString += "Seconds: " + std::to_string((int)gameClock.getElapsedTime().asSeconds());

		debugText.setString(debugString);

		deltaTime *= TIME_MULTIPLIER;

		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_window->close();
		}
		


		player->Update(deltaTime);

		_window->clear(sf::Color::Black);


		mapController->drawMap(*_window, 0);
		_window->draw(player->getSprite());
		mapController->drawMap(*_window, 1);
		


		// разкоментить, если необходимо отображение обстаклей
		
		//vector<Vector2f> obst = AStar::getObstacles();
		//int len = obst.size();
		//for (int i = 0; i < len; ++i)
		//{
		//	sf::CircleShape circ;
		//	circ.setFillColor(Color::Red);
		//	circ.setRadius(1);
		//	circ.setPosition(obst[i].x, obst[i].y);
		//	_window->draw(circ);
		//}


		_window->draw(debugText);
		_window->display();
	}
}

void Application::CLOSE() {
	if (!_window)
		delete _window;
}
