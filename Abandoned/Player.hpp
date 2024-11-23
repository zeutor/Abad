#pragma once
#include "Character.hpp"

class PlayerController;

class Player : public Character
{
private:
	sf::RenderWindow* _playingWindow;
	PlayerController* _controller;

public:
	float _distance;

	Player() = delete;
	Player(sf::Texture& texture, sf::Vector2f start_position, sf::RenderWindow& window);
	~Player();

	//Проверка всех состояний сущности каждый такт
	void Update(float time) override;
};

