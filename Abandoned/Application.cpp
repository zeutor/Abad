#include "Application.hpp"
#include "SFML/Graphics.hpp"
#include "Player.hpp"
#include "Object.hpp"

#include "Constants.hpp"
#include "MapController.hpp"
#include <iostream>
#include <set>
#include "UIManager.hpp"
#include "Outdata.hpp"
#include "MapObject.hpp"

void Application::INIT() {
	_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Abandoned");
	_window->setFramerateLimit(FRAME_LIMIT);
}

void Application::RUN() {
	outdata::getFiles();

	Player* player = new Player(outdata::player_texture, sf::Vector2f(0, 0), *_window);

	MapController* mapController = MapController::getController();

	mapController->getMap("devmap1");
	MapController::getInfoFromFile();


	sf::Text text("", outdata::mainFont, 20);

	sf::Clock deltaClock;
	sf::Clock gameClock;

	
	sf::View mainView = _window->getDefaultView();
	UIManager* UIController = UIManager::getController();
	UIController->setWindowToDisplay(_window);
	UIController->LoadIcons({ outdata::menu_icon,  outdata::invent_icon, outdata::journal_icon }, *_window);

	std::vector<UISlot> UISlots = UIController->getInvConroller();
	int CountOfSlots = UISlots.size();

	Object::Load("data\\objects\\Objects.obj");
	std::unordered_set<Object*> AllObject = Object::getAllObjects();
	sf::Vector2f posOfObj(PIXELS_PER_CELL*5, PIXELS_PER_CELL*7);

	LoadMapObjects("data\\objects\\MapObjects.mapobj");
	std::unordered_set<MapObject*> AllMapObject = MapObject::getAllMapObjects();

	bool isMouseReleased = true;

	while (_window->isOpen() ) {
		float deltaTime = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

		deltaTime *= TIME_MULTIPLIER;

		sf::Event event;
		while (_window->pollEvent(event) )
		{
			UIController->Listen(event, *player, *_window);
			if (event.type == sf::Event::Closed)
				_window->close();
		}




		text.setString(std::to_string(player->_distance / PIXELS_PER_METER));

		sf::Vector2i mousePos = sf::Mouse::getPosition(*_window);
	
		UIController->Update();

		if (!UIController->isWindowOpen(4) && !UIController->isWindowOpen(0) && !UIController->isWindowOpen(5)) {
			player->Update(deltaTime);
		}
	
		


		_window->setView(mainView);
		mapController->drawMap(*_window, 0);
		_window->draw(player->getSprite());
		mapController->drawMap(*_window, 1);

		_window->setView(mainView);
		_window->draw(text);
		UIController->LoadGameUI(event, *player);
		_window->display();
	}
}

void Application::CLOSE() {
	if (!_window)
		delete _window;
}
