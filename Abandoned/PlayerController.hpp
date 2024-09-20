#pragma once
#include "SFML/Graphics.hpp"

class Player;

class PlayerController
{
private:
	PlayerController() = default;
	static PlayerController* _controller;
	sf::Vector2f _aimPosition;
public:
	PlayerController(PlayerController const&) = delete;
	void operator=(PlayerController const&) = delete;
	~PlayerController();

	static PlayerController* getController();

	void controllPlayer(Player* player, float time, sf::RenderWindow* window);
};

