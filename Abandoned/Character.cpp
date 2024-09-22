#include "Character.hpp"
#include "Constants.hpp"

Character::~Character() {};


void Character::setGlobalPosition(sf::Vector2f& position)
{
	_position = position;
}

void Character::setPosition(sf::Vector2f& position) {
	_position.x = position.x - PIXELS_PER_CELL / 2;
	_position.y = position.y - PIXELS_PER_CELL / 2;
}

void Character::setState(State state)
{
	_state = state;
}

State Character::getState() const
{
	return _state;
}

sf::Vector2f Character::getSize() const
{
	return _size;
}

sf::Vector2f Character::getGlobalPosition() const
{
	sf::Vector2f pos(_position.x + PIXELS_PER_CELL / 2, _position.y + PIXELS_PER_CELL / 2);
	return pos;
}

sf::Vector2f Character::getPosition() const
{
	sf::Vector2f pos(_position.x + PIXELS_PER_CELL/2, _position.y + PIXELS_PER_CELL/2);
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

