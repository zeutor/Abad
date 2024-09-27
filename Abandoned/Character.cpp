#include "Character.hpp"
#include "Constants.hpp"
#include "MapController.hpp"

unsigned int Character::characterCount = 0;

Character::Character()
{
	characterCount += 1;
}

Character::~Character() 
{
	characterCount -= 1;
};


void Character::setGlobalPosition(sf::Vector2f& position)
{
	_position = position;
}

void Character::setPosition(sf::Vector2f position) {
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

float Character::getDistance() const
{
	return _distance;
}

void Character::moveTo(const sf::Vector2f& targetPosition, float deltaTime) {

	MapController* mapController = MapController::getController();

	sf::Vector2f currentPosition = getPosition();
	sf::Vector2f direction = targetPosition - currentPosition;
	float fullDist = sqrt(direction.x * direction.x + direction.y * direction.y);
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
		_distance += sqrt(delta.x * delta.x + delta.y * delta.y);
		currentPosition += delta;
		setPosition(currentPosition);
	}
}
