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

	// IDs of characters that comm
	int _fCharacterID;
	int _sCharacterID;

	Character* _fCh;
	Character* _sCh;

	// Code of diag, that shown now.
	string _activeDialogCode;

	// Pair "answ code"-"speech code" for answ
	map<string, string> _activeAnswCodes;

	/// <summary>
	/// Get localization of speech by speech code
	/// </summary>
	/// <param name="code">Code of speech</param>
	/// <returns>Localization for speech</returns>
	string getSpeechByCode(string code);
	
	/// <summary>
	/// Get pairs "answ code"-"speech code" for bot's speech with code.
	/// </summary>
	/// <param name="code">Speech of code, for wich we need to find answs</param>
	/// <returns>Pairs "answ code"-"speech code"</returns>
	map<string, string> getAnswPairs(string code);

	// Узнает число вариантов локализация для некоторого кода (типо how_are_you может быть "Как ты?" и "Как дела?")

	/// <summary>
	/// Getting num of variations for localization of speech with this code.
	/// Example: for how_are_you localizations "how are you?" "What's up?"
	/// </summary>
	/// <param name="code">Code for wich need to get count of localizations</param>
	/// <returns>Count of speech's localization</returns>
	int getNumberOfSpeech(string code);

	bool _isPlayerCommunicate;
public:
	// ##############################################################
	// ##################CREATE/DELETE ZONE##########################
	// ##############################################################
	CharacterCommunication() = delete;
	CharacterCommunication(int firstCharacterID, int secondCharacterID, string inputCode = "defalut");
	~CharacterCommunication();

	/// <summary>
	/// Sets for dialogeText value of active speech and answs.
	/// </summary>
	void answerToPlayer();

	/// <summary>
	/// Find speech, that will be reaction on user's answID
	/// </summary>
	/// <param name="answId"> id of user's answ</param>
	void getAnswCodeFromPlayer(int answId);

	static void setDialogText(sf::Text* dialogText);
};

