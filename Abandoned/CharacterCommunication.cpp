#include "CharacterCommunication.hpp"
#include "Character.hpp"
#include "Mission.hpp"
sf::Text* CharacterCommunication::dialogeText = nullptr;



string CharacterCommunication::getSpeechByCode(string code)
{
	ifstream localizationFile("data/localization/dialogues.txt");
	string gettedLocalizationCode;
	string mess = "";
	// Getting position where localization placed.
	do {
		// If end of file and dont already found just return code.
		if (localizationFile.eof())
			return code;
		localizationFile >> gettedLocalizationCode;
	} while (gettedLocalizationCode != code);
	string word = "";
	localizationFile >> word;
	// $ is break char
	while (word != "$")
	{
		mess += word + ' ';
		localizationFile >> word;
	}
	localizationFile.close();
	return mess;
}

map<string, string> CharacterCommunication::getAnswPairs(string code)
{
	ifstream dialogueFile("data/dialogues/default.txt");
	// Structure "answ code" - "dialog to go"
	map<string, string> answCodes;
	// Is in "dialog block" (between $ $ in file).
	bool isEnteredInDialogSubStr = false;
	string gettedDialogueCode = "";
	// Finding block of file with answs
	do {
		dialogueFile >> gettedDialogueCode;
		if (gettedDialogueCode == "$")
			isEnteredInDialogSubStr = !isEnteredInDialogSubStr;
	} while (gettedDialogueCode != code || isEnteredInDialogSubStr && !dialogueFile.eof());

	pair<string, string> answ_nextDialoguePair = make_pair("", "");
	string gettedAnswCode;
	dialogueFile >> gettedDialogueCode >>gettedAnswCode;
	
	// Reading answs
	while (gettedAnswCode != "$" && !dialogueFile.eof())
	{
		answ_nextDialoguePair.first = gettedAnswCode;
		dialogueFile >> gettedAnswCode;
		answ_nextDialoguePair.second = gettedAnswCode;
		answCodes.insert(answ_nextDialoguePair);
		dialogueFile >> gettedAnswCode;
	}
	dialogueFile.close();
	return answCodes;
}

int CharacterCommunication::getNumberOfSpeech(string code)
{
	ifstream dialogueNumsFile("data/dialogues/dialoguesNums.txt");
	string word = "";
	// Find block of file with info.
	while (word != code)
		dialogueNumsFile >> word;
	dialogueNumsFile >> word;
	dialogueNumsFile.close();
	return stoi(word);
}

CharacterCommunication::CharacterCommunication(int firstCharacterID, int secondCharacterID, string inputCode)
{
	_fCharacterID = firstCharacterID;
	_sCharacterID = secondCharacterID;

	// Create a connection
	_fCh = Character::getCharacter(_fCharacterID);
	_sCh = Character::getCharacter(_sCharacterID);
	if (!_sCh->isReadyToCommunicate(firstCharacterID))
		_sCh->closeCommunicationWindow();
	_sCh->connectToCommunicationWindow(this);

	_isPlayerCommunicate = (Character::getPlayer() == _fCh || Character::getPlayer() == _sCh);

	if (_isPlayerCommunicate)
	{
		// Just default. Later some logic will here.
		_activeDialogCode = "default_called_calm";
		answerToPlayer();
	}
}

CharacterCommunication::~CharacterCommunication()
{
	_fCh->closeCommunicationWindow();
	_sCh->closeCommunicationWindow();
}

void CharacterCommunication::answerToPlayer()
{
	// End communication.
	if (_activeDialogCode == "exit")
	{
		dialogeText->setString("");
		delete this;
	}
	// Give list of missions for player
	else if (_activeDialogCode == "mission")
	{
		std::vector<Mission*> missions = _sCh->getMissionsToGiveList();
		// If no missions
		if (missions.size() == 0)
		{
			_activeDialogCode = "default_no_missions_calm";

			// Random for speech variations
			srand((unsigned int)time(0));
			int speechCode = rand() % getNumberOfSpeech(_activeDialogCode);

			string textToShow = "";

			textToShow += getSpeechByCode(_activeDialogCode + "_" + to_string(speechCode)) + "\n\n";

			_activeAnswCodes = getAnswPairs(_activeDialogCode);
			int answerNumericIndex = 1;

			for (auto iterator = _activeAnswCodes.begin(); iterator != _activeAnswCodes.end(); ++iterator, ++answerNumericIndex)
				textToShow += to_string(answerNumericIndex) + ") " + getSpeechByCode((*iterator).first) + "\n";

			dialogeText->setString(textToShow);
			return;
		}
		// If have missions.
		_activeDialogCode = "defalul_mission_calm";

		// Random for speech variations
		srand((unsigned int)time(0));
		int speechCode = rand() % getNumberOfSpeech(_activeDialogCode);

		string textToShow = "";

		textToShow += getSpeechByCode(_activeDialogCode + "_" + to_string(speechCode));
		int counter = 1;
		for (auto it = missions.begin(); it != missions.end(); ++it, ++counter)
			textToShow += "\n" + to_string(counter) + ") " + (*it)->getTaskTitle();
		dialogeText->setString(textToShow);
		_activeDialogCode = "mission";
	}
	// To reject or finish mission.
	else if (_activeDialogCode == "about_mission")
	{
		std::vector<Mission*> missions = _fCh->getMissionsGivedByCharacter(_sCh);

		// Random for speech variations
		srand((unsigned int)time(0));
		int speechCode = rand() % getNumberOfSpeech(_activeDialogCode);

		string textToShow = "";

		textToShow += getSpeechByCode(_activeDialogCode + "_" + to_string(speechCode));

		int counter = 1;
		for (auto it = missions.begin(); it != missions.end(); ++it, ++counter)
			textToShow += "\n" + to_string(counter) + ") " + (*it)->getTaskTitle();
		// Exit variant
		textToShow += "\n" + to_string(counter + 1) + ") " + getSpeechByCode("about_mission_exit");
		dialogeText->setString(textToShow);
	}
	// When just speech without any events.
	else {
		srand((unsigned int)time(0));
		int speechCode = rand() % getNumberOfSpeech(_activeDialogCode);

		string textToShow = "";

		textToShow += getSpeechByCode(_activeDialogCode + "_" + to_string(speechCode)) + "\n\n";

		_activeAnswCodes = getAnswPairs(_activeDialogCode);

		if (_sCh->isMissionForPlayer())
			_activeAnswCodes["answ_about_mission"] = "about_mission";

		int answerNumericIndex = 1;

		for (auto iterator = _activeAnswCodes.begin(); iterator != _activeAnswCodes.end(); ++iterator, ++answerNumericIndex)
			textToShow += to_string(answerNumericIndex) + ") " + getSpeechByCode((*iterator).first) + "\n";

		dialogeText->setString(textToShow);
	}
}

void CharacterCommunication::getAnswCodeFromPlayer(int answId)
{
	// If mission dialog, need to check wich mission player would take.
	if (_activeDialogCode == "mission")
	{
		std::vector<Mission*> missions = _sCh->getMissionsToGiveList();

		if (answId > missions.size())
			return;

		// Here should be block with description of mission.
		_fCh->takeMission(missions[answId - 1]);
		_sCh->setMissionForPlayer(true);

		_activeDialogCode = "default_mission_take";
		answerToPlayer();
		return;
	}
	// To reject or finish mission.
	if (_activeDialogCode == "about_mission")
	{

	}
	// If pressed wrong key.
	if (answId > _activeAnswCodes.size())
		return;

	int answerNumericIndex = 1;
	for (auto iterator = _activeAnswCodes.begin(); iterator != _activeAnswCodes.end(); ++iterator, ++answerNumericIndex)
		if (answerNumericIndex == answId)
			_activeDialogCode = (*iterator).second;
	answerToPlayer();

}

void CharacterCommunication::setDialogText(sf::Text* gettedDialogeText)
{
	dialogeText = gettedDialogeText;
}
