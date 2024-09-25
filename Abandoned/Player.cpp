#include "Player.hpp"
#include "PlayerController.hpp"
#include "Constants.hpp"



Player::Player(sf::Texture& texture, sf::Vector2f start_pos, sf::RenderWindow& playingWindow)
{
	
	_position = start_pos;
	_speed = DEFAULT_SPEED;
	_controller = PlayerController::getController();
	_sprite.setTexture(texture);
	_size = sf::Vector2f(_sprite.getTextureRect().width, _sprite.getTextureRect().height);
	_playingWindow = &playingWindow;
	_sprite.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
	_distance = 0;
}

Player::~Player()
{
}



void Player::Update(float time)
{
	_state = State::IDLE;
	_controller->controllPlayer(this, time, _playingWindow);

	_sprite.setPosition(_position);
}