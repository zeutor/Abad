#include "Application.hpp"
#include "SFML/Graphics.hpp"
#include "Player.hpp"
#include "Object.hpp"

#include "Constants.hpp"
#include "MapController.hpp"
#include <iostream>
#include "UIManager.hpp"

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
	sf::View miniMapView;
	sf::RectangleShape miniMapBorder = UIController->SetMinimap(miniMapView);
	std::vector<UISlot> UISlots = UIManager::getInvConroller();
	int CountOfSlots = UISlots.size();

	Object::Load("data\\objects\\Objects.obj");
	std::unordered_set<Object*> AllObject = Object::getAllObjects();
	int sizeOfobjectSet = AllObject.size();
	std::cout << sizeOfobjectSet;
	




	while (_window->isOpen()) {
		float deltaTime = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

		deltaTime *= TIME_MULTIPLIER;

		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_window->close();
		}


		text.setString(std::to_string(player->_distance / PIXELS_PER_METER));

		sf::Vector2i mousePos = sf::Mouse::getPosition(*_window);
		/*if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !handleSlotClick(mousePos, k)){*/
			player->Update(deltaTime);
			Listen(UISlots);
		

		_window->setView(mainView);
		mapController->drawMap(*_window, 0); 
		_window->draw(player->getSprite());
		mapController->drawMap(*_window, 1);

	
		_window->setView(miniMapView);
		mapController->drawMap(*_window, 0); 
		_window->draw(player->getSprite());
		mapController->drawMap(*_window, 1); 

		_window->setView(mainView);
		_window->draw(text);
		_window->draw(miniMapBorder);
		

		// Якобы LoadUI функция


		for (const auto& obj : AllObject) {

			if(!obj->getIsInInventory()){
			_window->draw(obj->getSprite());
			}

		}
		
		for (int i = 0; i < CountOfSlots; ++i)
		{
			_window->draw(UISlots[i].sprite);
		}

		
		_window->display();
	}
}

void Application::CLOSE() {
	if (!_window)
		delete _window;
}
