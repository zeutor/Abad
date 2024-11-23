#include "Application.hpp"
#include "SFML/Graphics.hpp"
#include "Outdata.hpp"
#include "Character.hpp"
#include "Object.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include <iostream>
#include <set>
#include "UIManager.hpp"
#include "MapObject.hpp"

using namespace std;

void Application::INIT() {
	_gameWindow = new sf::RenderWindow(sf::VideoMode(1920,1080), "Abandoned", sf::Style::Fullscreen);
	_gameWindow->setFramerateLimit(FRAME_LIMIT);
}

void func1(string called)
{
	cout << "1" << endl;
}

void Application::RUN() {

	outdata::getFiles();

	// ############################################################
	// ################## MAP LOADER ZONE #########################
	// ############################################################

	MapController* mapController = MapController::getController();
	mapController->getMap("devmap5");
	mapController->loadObstacles();
	

	// ############################################################
	// ################ CHARACTER LAODING ZONE ####################
	// ############################################################

	sf::Vector2f PlayerStartPos(0.f, 0.f);
	PlayerStartPos = mapController->getPlayerStartPosition();
	Character player = Character(outdata::tifl_texture,PlayerStartPos, _gameWindow, true);

	sf::Text debugText("", outdata::mainFont, 20);

	sf::Clock deltaClock;
	sf::Clock gameClock;

	// ############################################################
	// ##################### UI LAODING ZONE ######################
	// ############################################################

	sf::View mainView = _gameWindow->getDefaultView();
	UIManager* UIController = UIManager::getController();
	UIController->setWindowToDisplay(_gameWindow);
	UIController->LoadIcons({outdata::menu_icon,  outdata::invent_icon, outdata::journal_icon}, *_gameWindow);

	std::vector<UISlot> UISlots = UIController->getInvConroller();
	int CountOfSlots = UISlots.size();
	// ############################################################
	// ##################### OBJ LAODING ZONE #####################
	// ############################################################

	Object::Load("data\\objects\\Objects.obj");
	std::unordered_set<Object*> AllObject = Object::getAllObjects();
	sf::Vector2f posOfObj(PIXELS_PER_CELL*5, PIXELS_PER_CELL*7);



// ############################################################
// ##################### MAPOBJ LAODING ZONE #####################
// ############################################################
	LoadMapObjects("data\\objects\\MapObjects.mapobj");
	std::unordered_set<MapObject*> AllMapObject = MapObject::getAllMapObjects();
	

	while (_gameWindow->isOpen() ) {
		float deltaTime = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

		//############################################################
		//################### DEBUG LINE ZONE ########################
		//############################################################

		std::string debugString;
		debugString += "Steps: " + std::to_string(player.getDistance() / PIXELS_PER_METER) + '\n';
		debugString += "FPS: " + std::to_string(1./deltaTime) + '\n';
		debugString += "Seconds: " + std::to_string((int)gameClock.getElapsedTime().asSeconds()) + '\n';
		debugString += "Player pos: " + std::to_string(Character::getPlayer()->getPosition().x) + " " + std::to_string(Character::getPlayer()->getPosition().y);

		debugText.setString(debugString);


		deltaTime *= TIME_MULTIPLIER;

		//############################################################
		//################### EVENT ZONE #############################
		//############################################################

		sf::Event event;
		while (_gameWindow->pollEvent(event) )
		{
			UIController->Listen(event, player, *_gameWindow);
			if (event.type == sf::Event::Closed)
				_gameWindow->close();
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(*_gameWindow);
	
		//############################################################
		//#################### UPDATE ZONE ###########################
		//############################################################

		UIController->Update();

	
		if(!UIController->isWindowOpen(4) && !UIController->isWindowOpen(0) && !UIController->isWindowOpen(5)){
			player.Update(deltaTime);
		}
		//############################################################
		//#################### DRAW ZONE #############################
		//############################################################

		// draw map
		mapController->drawMap(*_gameWindow, 0);
		_gameWindow->draw(player.getSprite());
		mapController->drawMap(*_gameWindow, 1);

		// draw minimap
		mapController->drawMap(*_gameWindow, 0);
		_gameWindow->draw(player.getSprite());
		mapController->drawMap(*_gameWindow, 1);
		_gameWindow->setView(mainView);

		_gameWindow->draw(debugText);
		
		UIController->LoadGameUI(event, player);


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

		_gameWindow->display();
	}
}

void Application::CLOSE() {
	if (!_gameWindow)
		delete _gameWindow;
}
