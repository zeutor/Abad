#pragma once
#include "Character.hpp"


using namespace sf;
class Player : public Character
{
private:
	friend class PlayerController;
	sf::RenderWindow* _playingWindow;
	PlayerController* _controller;
	
public:

	Player() = delete;
	Player(sf::Texture& texture, sf::Vector2f start_position, sf::RenderWindow& window);
	~Player();

	// Overload of character's update
	void Update(float time) override;
};

