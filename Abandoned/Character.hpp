#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_set>

enum State {
	IDLE,
	WALK,
	RUN
};


class Character
{
protected:
	static unsigned int characterCount;
	float _speed;
	float _health;
	sf::Vector2f _size;
	sf::Vector2f _position;
	sf::Sprite _sprite;
	State _state;
	//unsigned int _ID;
	//std::unordered_set<int> _effectsID;
	//std::unordered_multiset<int> _inventory;
public:

	virtual ~Character();
	//ѕроверка всех состо€ний сущности каждый такт
	virtual void Update(float time) = 0;

	//”станавливает координаты относительно верхнего левого кра€
	void setGlobalPosition(sf::Vector2f& position);
	//”станавливает координаты относительно центра
	void setPosition(sf::Vector2f& position);
	void setState(State state);

	State getState() const;
	sf::Vector2f getSize() const;
	//ѕолучает координаты относительно верхнего левого кра€
	sf::Vector2f getGlobalPosition() const;
	//ѕолучает координаты относительно центра
	sf::Vector2f getPosition() const;
	sf::Sprite getSprite() const;
	float getSpeed() const;
};

