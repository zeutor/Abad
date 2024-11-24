#include "CharacterCommunication.hpp"
#include "Character.hpp"

sf::Text* CharacterCommunication::dialogeText = nullptr;



string CharacterCommunication::getSpeechByCode(string code)
{
	ifstream localizationFile("data/localization/dialogues.txt");
	string gettedLocalizationCode;
	string mess = "";
	do {
		localizationFile >> gettedLocalizationCode;
	} while (gettedLocalizationCode != code);
	string word = "";
	localizationFile >> word;
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
	map<string, string> answCodes;

	string gettedDialogueCode = "";
	do {
		dialogueFile >> gettedDialogueCode;
	} while (gettedDialogueCode != code);

	pair<string, string> answ_nextDialoguePair = make_pair("", "");
	string gettedAnswCode;
	dialogueFile >> gettedAnswCode;

	while (gettedAnswCode != "$")
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
	srand((unsigned int)time);
	int speechCode = rand() % getNumberOfSpeech(_activeDialogCode);

	string textToShow = "";

	textToShow += getSpeechByCode(_activeDialogCode + "_" + to_string(speechCode)) + "\n\n";

	_activeAnswCodes = getAnswPairs(_activeDialogCode);
	int answerNumericIndex = 1;

	for (auto iterator = _activeAnswCodes.begin(); iterator != _activeAnswCodes.end(); ++iterator, ++answerNumericIndex)
		textToShow += to_string(answerNumericIndex) + ") " + getSpeechByCode((*iterator).first) + "\n";

	dialogeText->setString(textToShow);
}

void CharacterCommunication::getAnswCodeFromPlayer(int answId)
{
	int answerNumericIndex = 1;
	for (auto iterator = _activeAnswCodes.begin(); iterator != _activeAnswCodes.end(); ++iterator, ++answerNumericIndex)
		if (answerNumericIndex == answId)
		{
			_activeDialogCode = (*iterator).second;
		}
	answerToPlayer();
}

void CharacterCommunication::setDialogText(sf::Text* gettedDialogeText)
{
	dialogeText = gettedDialogeText;
}
