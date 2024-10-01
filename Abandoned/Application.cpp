#include "Application.hpp"
#include "SFML/Graphics.hpp"
#include "Player.hpp"
#include "Object.hpp"

#include "Constants.hpp"
#include "MapController.hpp"
#include <iostream>
#include <set>
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
	sf::Vector2f posOfObj(PIXELS_PER_CELL*5, PIXELS_PER_CELL*7);

	

	
	outdata::vectorWithIcons = UIController->LoadIcons(outdata::vectorWithIcons);

	bool isMouseReleased = true;

	while (_window->isOpen() ) {
		float deltaTime = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

		deltaTime *= TIME_MULTIPLIER;

		sf::Event event;
		while (_window->pollEvent(event) )
		{
			Listen(UISlots, event);
			if (event.type == sf::Event::Closed)
				_window->close();
		}




		text.setString(std::to_string(player->_distance / PIXELS_PER_METER));

		sf::Vector2i mousePos = sf::Mouse::getPosition(*_window);
	
	
		if(!UIManager::MenuJournalInventory[0])
		{ 
		player->Update(deltaTime);
		}
	
		


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

			if (!obj->getIsInInventory()) {
				sf::Vector2f i = obj->getPosition();

				sf::Sprite i1 = obj->getSprite();
				i1.setPosition(i);
				obj->setSprite(i1);
				_window->draw(i1);
			}

		}

		for (int i = 0; i < CountOfSlots; ++i)
		{
			_window->draw(UISlots[i].sprite);
		}
		for (int i = 0; i < 3; ++i)
		{
			_window->draw(outdata::vectorWithIcons[i]);
		}

		// реакция инветаря на нажатие по иконке.
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
			if (outdata::vectorWithIcons[1].getGlobalBounds().contains(mousePos) && isMouseReleased && !UIManager::MenuJournalInventory[0]) {
				UIManager::MenuJournalInventory[1] = !UIManager::MenuJournalInventory[1];
				isMouseReleased = false;
			}
			else if (outdata::vectorWithIcons[0].getGlobalBounds().contains(mousePos) && isMouseReleased)
			{
				UIManager::MenuJournalInventory[0] = !UIManager::MenuJournalInventory[0];
				isMouseReleased = false;
			}


		}
		if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			isMouseReleased = true;  
		}
		if (UIManager::MenuJournalInventory[0])
		{
			
			UIController->LoadMenu(*_window, event);
		}
		if (UIManager::MenuJournalInventory[1] && !UIManager::MenuJournalInventory[0])
			UIController->LoadInventory(*_window, *player, AllObject, event);
		if (UIManager::MenuJournalInventory[2])
		{
			//LoadJournal
		}
		
		_window->display();
	}
}

void Application::CLOSE() {
	if (!_window)
		delete _window;
}
