#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_set>
#include "AStar.hpp"

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
	float _distance;

	AStar astar;
	//unsigned int _ID;
	//std::unordered_set<int> _effectsID;
	//std::unordered_multiset<int> _inventory;
public:

	Character();

	virtual ~Character();
	// Update character params every frame
	virtual void Update(float time) = 0;

	void moveTo(const sf::Vector2f& targetPosition, float deltaTime);

	// Set character's position with center on the left upper corner
	void setGlobalPosition(sf::Vector2f& position);
	// Set character's position in sprite center
	void setPosition(sf::Vector2f position);
	void setState(State state);

	State getState() const;
	sf::Vector2f getSize() const;
	// Get character's position with center on the left upper corner
	sf::Vector2f getGlobalPosition() const;
	// Get character's position in sprite center
	sf::Vector2f getPosition() const;
	sf::Sprite getSprite() const;
	float getSpeed() const;
	float getDistance() const;

};

