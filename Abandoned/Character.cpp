#include "Character.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "PlayerController.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "tools.hpp"

using namespace std;

unsigned int Character::_characterCount = 0;
Character* Character::_controlledCharacter;
std::unordered_map<unsigned int, Character*> Character::_allCharactersRefs;

// ##############################################################
// ##################CREATE/DELETE ZONE##########################
// ##############################################################

Character::Character(sf::Texture& texture, sf::Vector2f& start_position, sf::RenderWindow* window, bool isUnderControl)
{

	// Setting charaster as player
	_isUnderControl = isUnderControl;
	if (isUnderControl)
	{
		if(_controlledCharacter)
			_controlledCharacter->_isUnderControl = false;
		_controlledCharacter = this;
	}

	// Picture settings
	_position = start_position;
	_sprite.setTexture(texture);
	_size = sf::Vector2f(_sprite.getTextureRect().width, _sprite.getTextureRect().height);
	_sprite.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);


	// Character setting
	_speed = DEFAULT_SPEED;
	_distance = 0;
	// +2 to get IDs from 1.
	_ID = _allCharactersRefs.size() + 2;
	_money = 10;
	_health = 100;

	// Adding ability to control by mouse.
	_controller = PlayerController::getController();

	// Adding in global list of Character's
	_allCharactersRefs[_ID] = this;

	_playingWindow = window;
	_communicationChanael = nullptr;
	_isInDialog = false;

	_astar = AStar();
	MapController* mapManager = MapController::getController();
	mapManager->loadObstacles(_astar);
}

Character::~Character() 
{
	_characterCount -= 1;
}



// ##############################################################
// ##########################SETTERS ZONE########################
// ##############################################################

void Character::setGlobalPosition(sf::Vector2f& position)
{
	_position = position;
}

void Character::setPosition(sf::Vector2f& position) {
	_position.x = position.x - PIXELS_PER_CELL / 2;
	_position.y = position.y - PIXELS_PER_CELL / 2;
}

// ##############################################################
// ##########################GETTERS ZONE########################
// ##############################################################

sf::Vector2f Character::getSize() const
{
	return _size;
}

sf::Vector2f Character::getGlobalPosition() const
{
	return _position;
}

sf::Vector2f Character::getPosition() const
{
	sf::Vector2f pos(_position.x + PIXELS_PER_CELL / 2, _position.y + PIXELS_PER_CELL / 2);
	return pos;
}

sf::Sprite Character::getSprite() const
{
	return _sprite;
}

float Character::getSpeed() const
{
	return _speed;
}

float Character::getDistance() const
{
	return _distance;
}

sf::Vector2i Character::getCell() const
{
	return sf::Vector2i(_position.x / PIXELS_PER_CELL, _position.y / PIXELS_PER_CELL);
}

unsigned int Character::getID() const
{
	return _ID;
}


// ##############################################################
// ###################COMMUNICATION ZONE#########################
// ##############################################################

void Character::openCommunicationWindow(int otherChId)
{
	_communicationChanael = new CharacterCommunication(_ID, otherChId);
}

bool Character::isReadyToCommunicate(int senderID)
{
	// Hier we need to check conditions to speek. Now just "ok" if not in dialoge.
	return !_isInDialog;
}

void Character::connectToCommunicationWindow(CharacterCommunication* _cWindow)
{
	_communicationChanael = _cWindow;
	_isInDialog = true;
}

void Character::waitingForPlayerAnswer()
{
	
}


void Character::closeCommunicationWindow()
{
	_communicationChanael = nullptr;
	_isInDialog = false;
}

void Character::insertInInventory(int objId)
{
	_inventory.insert(objId);
}

void Character::RemoveFromInventory(int objId)
{
	auto it = _inventory.find(objId);

	if (it != _inventory.end()) {
		_inventory.erase(it);
	}
}

std::multiset<int> Character::GetInventory()
{
	return _inventory;
}


// ##############################################################
// ##########################OTHER ZONE##########################
// ##############################################################

void Character::Update(float time)
{
	// If controlled, he wait player's command
	if (_isUnderControl)
	{
		_controller->controllPlayer(*this, time, _playingWindow);
	}
	else
	{
		// Here will be very very good AI (not sure)
	}

	_sprite.setPosition(_position);
}

void Character::moveTo(const sf::Vector2f& targetPosition, float deltaTime) {

	MapController* mapController = MapController::getController();

	sf::Vector2f currentPosition = getPosition();
	sf::Vector2f direction = targetPosition - currentPosition;
	float fullDist = tools::vectorLength(direction);	
	direction /= fullDist;
	if (fullDist > POSITION_EPSILON)
	{
		if (direction.y < 0 && mapController->checkCollision(0, getPosition()))
			direction.y *= COLLISION_MULTIPLIER;
		else if (direction.y > 0 && mapController->checkCollision(2, getPosition()))
			direction.y *= COLLISION_MULTIPLIER;

		if (direction.x < 0 && mapController->checkCollision(3, getPosition()))
			direction.x *= COLLISION_MULTIPLIER;
		else if (direction.x > 0 && mapController->checkCollision(1, getPosition()))
			direction.x *= COLLISION_MULTIPLIER;

		sf::Vector2f delta = direction * _speed * deltaTime * (fullDist < SLOW_WALK_DISTANCE ? SLOW_WALK_MULTIPLIER : 1);
		_distance += tools::vectorLength(delta);		
		currentPosition += delta;
		setPosition(currentPosition);
	}
}

bool Character::isPointOnPerson(sf::Vector2f position)
{
	return _sprite.getGlobalBounds().contains(position);
}

// ##############################################################
// #########################STATIC ZONE##########################
// ##############################################################

Character* Character::getCharacter(unsigned int ID)
{
	if (_allCharactersRefs.count(ID) != 0)
		return _allCharactersRefs[ID];
	return nullptr;
}

Character* Character::getCharacterByPoint(sf::Vector2f point)
{
	for (pair<unsigned int, Character*> ch : _allCharactersRefs)
		if (ch.second->getSprite().getGlobalBounds().contains(point))
			return ch.second;
	return nullptr;
}

unsigned int Character::getCharacterIDByPoint(sf::Vector2f point, Character** ignoreList, int ignoreListLen)
{
	bool needCheckIgnoreList = ignoreList != nullptr && ignoreListLen != -1;
	for (pair<unsigned int, Character*> ch : _allCharactersRefs)
		if (ch.second->getSprite().getGlobalBounds().contains(point))
			if (!needCheckIgnoreList || !tools::isInArray<Character*>(ignoreList, ch.second, ignoreListLen))
				return ch.first;
	return 0;
}

Character* Character::getPlayer() {
	return _controlledCharacter;
}