#pragma once
#include "SFML/Graphics.hpp"
#include "AStar.hpp"
#include "CharacterCommunication.hpp"
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>

class Object;


class PlayerController;

class Character
{
protected:
	friend class PlayerController;

	// ##############################################################
	// ##################STATIC ZONE#################################
	// ##############################################################

	static unsigned int _characterCount;
	static Character* _controlledCharacter;
	static std::unordered_map<unsigned int, Character*> _allCharactersRefs;


	// ##############################################################
	// #####################SPRITE PARAMS############################
	// ##############################################################

	sf::Vector2f _size;
	// Storage in pixels
	sf::Vector2f _position;
	sf::Sprite _sprite;

	// ##############################################################
	// ####################CHARACTER PARAMS##########################
	// ##############################################################

	float _speed;
	float _health;
	float _distance;
	unsigned int _ID;
	int _money;
	//std::unordered_set<int> _effectsID;
	//std::unordered_multiset<int> _inventory;


	// ##############################################################
	// ##################MOVE AND CONTROL ZONE#######################
	// ##############################################################

	AStar _astar;
	PlayerController* _controller;

	//Вынесли переменные просчета AStar отдельно от PlayerController
	std::vector<AStar::sNode*> _path;
	size_t _currentTargetIndex;


	// ##############################################################
	// ##########################FLAGS ZONE##########################
	// ##############################################################

	bool _isUnderControl;
	bool _isInDialog;

	// ##############################################################
	// ##########################OTHER ZONE##########################
	// ##############################################################

	sf::RenderWindow* _playingWindow;
	CharacterCommunication *_communicationChanael;
	std::multiset<int> _inventory;

public:

	// ##############################################################
	// ##################CREATE/DELETE ZONE##########################
	// ##############################################################

	Character() = delete;
	Character(sf::Texture& texture, sf::Vector2f& start_position, sf::RenderWindow* window, bool isUnderControl = false);
	virtual ~Character();


	// ##############################################################
	// ##########################SETTERS ZONE########################
	// ##############################################################

	// Set character's position with center on the left upper corner
	void setGlobalPosition(sf::Vector2f& position);
	// Set character's position in sprite center
	void setPosition(sf::Vector2f& position);


	// ##############################################################
	// ##########################GETTERS ZONE########################
	// ##############################################################

	sf::Vector2f getSize() const;
	// Get character's position with center on the left upper corner
	sf::Vector2f getGlobalPosition() const;
	// Get character's position in sprite center
	sf::Vector2f getPosition() const;
	sf::Sprite getSprite() const;
	float getSpeed() const;
	float getDistance() const;
	sf::Vector2i getCell() const;
	unsigned int getID() const;


	// ##############################################################
	// ###################COMMUNICATION ZONE#########################
	// ##############################################################

	void openCommunicationWindow(int otherChId);
	bool isReadyToCommunicate(int senderID);
	void connectToCommunicationWindow(CharacterCommunication* _cWindow);
	void waitingForPlayerAnswer();
	void closeCommunicationWindow();

	// ##############################################################
	// ######################INVENTORY ZONE##########################
	// ##############################################################

	void insertInInventory(int objId);
	void RemoveFromInventory(int objId);
	std::multiset<int> GetInventory();

	// ##############################################################
	// ##########################OTHER ZONE##########################
	// ##############################################################

	void Update(float time);
	void moveTo(const sf::Vector2f& targetPosition, float deltaTime);
	bool isPointOnPerson(sf::Vector2f position);

	// ##############################################################
	// #########################STATIC ZONE##########################
	// ##############################################################
	
	/// <summary>
	/// Return ref on preson, that has this point in his sprite.
	/// </summary>
	/// <param name="point">Point to check</param>
	/// <returns>Ref on character or nullptr if this cell empty</returns>
	static Character* getCharacterByPoint(sf::Vector2f point);

	/// <summary>
	/// Returns id of character, that stay in point. Can check ignore list.
	/// </summary>
	/// <param name="point">Check this point.</param>
	/// <param name="ignoreList">Array of characters, that should be ignored.</param>
	/// <param name="ignoreListLen">Len of array</param>
	/// <returns>0 if noone in this point or id of character.</returns>
	static unsigned int getCharacterIDByPoint(sf::Vector2f point, Character** ignoreList = nullptr, int ignoreListLen = -1);
	static Character* getPlayer();
	static Character* getCharacter(unsigned int ID);
};

