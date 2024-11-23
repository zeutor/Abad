#pragma once
#include "SFML/Graphics.hpp"

class Character;

//Сингелтон класс. Отвечает за нахождение пути игровым персонажем и реакцией его на нажатия на экран.
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

	void controllPlayer(Character& player, float time, sf::RenderWindow* window);
};

