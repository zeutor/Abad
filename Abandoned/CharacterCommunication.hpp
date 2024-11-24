#pragma once
#include <string>
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>

class Character;

using namespace std;

class CharacterCommunication
{
private:
	static sf::Text* dialogeText;

	// Идентификаторы взаимодействующих персонажей
	int _fCharacterID;
	int _sCharacterID;

	Character* _fCh;
	Character* _sCh;

	string _activeDialogCode;

	map<string, string> _activeAnswCodes;

	string getSpeechByCode(string code);
	map<string, string> getAnswPairs(string code);
	int getNumberOfSpeech(string code);

	bool _isPlayerCommunicate;
public:
	// ##############################################################
	// ##################CREATE/DELETE ZONE##########################
	// ##############################################################
	CharacterCommunication() = delete;
	CharacterCommunication(int firstCharacterID, int secondCharacterID, string inputCode = "defalut");
	~CharacterCommunication();

	void answerToPlayer();
	void getAnswCodeFromPlayer(int answId);

	static void setDialogText(sf::Text* dialogText);
};

