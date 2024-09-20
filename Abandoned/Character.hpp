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
	virtual void Update(float time) = 0;

	void setPosition(sf::Vector2f& position);
	void setState(State state);

	State getState() const;
	sf::Vector2f getSize() const;
	sf::Vector2f getPosition() const;
	sf::Sprite getSprite() const;
	float getSpeed() const;
};

