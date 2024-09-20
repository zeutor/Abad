#include "Character.hpp"

Character::~Character() {};

void Character::setPosition(sf::Vector2f& position) {
	_position = position;
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

sf::Vector2f Character::getPosition() const
{
	return _position;
}

sf::Sprite Character::getSprite() const
{
	return _sprite;
}

float Character::getSpeed() const
{
	return _speed;
}

