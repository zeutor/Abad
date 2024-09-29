#include "Character.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "PlayerController.hpp"

unsigned int Character::_StatCharacterCount = 0;
Character* Character::_StatControlledCharacter;

Character::Character(sf::Texture& texture, sf::Vector2f& start_position, sf::RenderWindow* window, bool isUnderControl)
{
	_StatCharacterCount += 1;

	_isUnderControl = isUnderControl;
	if (isUnderControl)
		_StatControlledCharacter = this;

	_position = start_position;
	_speed = DEFAULT_SPEED;
	_controller = PlayerController::getController();
	_sprite.setTexture(texture);
	_size = sf::Vector2f(_sprite.getTextureRect().width, _sprite.getTextureRect().height);
	_playingWindow = window;
	_sprite.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
	_distance = 0;
	_ID = _StatCharacterCount;
}

Character::~Character() 
{
	_StatCharacterCount -= 1;
}

void Character::Update(float time)
{
	_state = State::IDLE;
	if (_isUnderControl)
	{
		_controller->controllPlayer(*this, time, _playingWindow);
	}
	_sprite.setPosition(_position);
}

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

Character* Character::getPlayer() {
	return _StatControlledCharacter;
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
