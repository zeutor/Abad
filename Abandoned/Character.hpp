#pragma once
#include "SFML/Graphics.hpp"
#include "AStar.hpp"


enum State {
	IDLE,
	WALK,
	RUN
};


class Character
{
protected:
	friend class PlayerController;

	static unsigned int _StatCharacterCount;
	static Character* _StatControlledCharacter;

	float _speed;
	float _health;
	sf::Vector2f _size;
	// Storage pixels
	sf::Vector2f _position;
	sf::Sprite _sprite;
	State _state;
	float _distance;
	unsigned int _ID;

	AStar _astar;

	sf::RenderWindow* _playingWindow;
	PlayerController* _controller;

	//std::unordered_set<int> _effectsID;
	//std::unordered_multiset<int> _inventory;

	bool _isUnderControl;
public:

	Character() = delete;
	Character(sf::Texture& texture, sf::Vector2f& start_position, sf::RenderWindow* window, bool isUnderControl = false);


	virtual ~Character();
	// Update character params every frame
	void Update(float time);

	void moveTo(const sf::Vector2f& targetPosition, float deltaTime);

	// Set character's position with center on the left upper corner
	void setGlobalPosition(sf::Vector2f& position);
	// Set character's position in sprite center
	void setPosition(sf::Vector2f& position);
	void setState(State state);

	static Character* getPlayer();

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

