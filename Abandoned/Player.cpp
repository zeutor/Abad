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

void Player::moveTo(const sf::Vector2f& targetPosition, float speed, float deltaTime) {
    sf::Vector2f currentPosition = Player::getPosition();

    
    sf::Vector2f direction = targetPosition - currentPosition;

   
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);


    if (distance > 0) {
     
        sf::Vector2f normalizedDirection = direction / distance;

        
        sf::Vector2f movement = normalizedDirection * speed * deltaTime;

      
        if (distance > 0.5) {
            sf::Vector2f c = currentPosition + movement;
            setPosition(c);
        }
        else {
            
       
        }
    }
}

void Player::Update(float time)
{
	_state = State::IDLE;
	_controller->controllPlayer(this, time, _playingWindow);

	_sprite.setPosition(_position);
}