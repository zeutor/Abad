#pragma once
#include "Character.hpp"

class PlayerController;
using namespace sf;
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

	void moveTo(const sf::Vector2f& targetPosition, float speed, float deltaTime);


	//Ïðîâåðêà âñåõ ñîñòîÿíèé ñóùíîñòè êàæäûé òàêò
	void Update(float time) override;
};

