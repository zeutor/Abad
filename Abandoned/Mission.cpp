#include "Mission.hpp"
#include "Character.hpp"

Mission::Mission(Character* employer, std::string missionCode, int rewar, int repMult)
{
    _employer = employer;
    _missionCode = missionCode;
    _reward = rewar;
    _reputationMult = repMult;
    if (_missionCode == "def_bring")
    {
        bringTask* subTask = new bringTask("bring_simple", this);
        subTask->addObjForCharacter(1001, _employer);
        _tasks.insert(subTask);
    }
}

void Mission::setMerc(Character* merc)
{
    _mercenary = merc;
}

Character* Mission::getMerc() const
{
    return _mercenary;
}

Character* Mission::getEmployer()
{
    return _employer;
}

bool Mission::isReady()
{
    for (auto oneTask : _tasks)
        if (!oneTask->isReady())
            return false;
    return true;
}

std::string Mission::getTaskTitle()
{
    ifstream localizationFile("data/localization/missions.txt");
    string gettedLocalizationCode;
    string mess = "";
    do {
        localizationFile >> gettedLocalizationCode;
    } while (gettedLocalizationCode != _missionCode);
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

Mission::bringTask::bringTask(std::string taskCode, Mission* parentMission)
{
    _taskCode = taskCode;
    _parentMission = parentMission;
}

void Mission::bringTask::addObjForCharacter(int objId, Character* aimCharacter)
{
    _objForCharacter[objId] = aimCharacter;
}

bool Mission::bringTask::isReady()
{
    for (auto objChPair : _objForCharacter)
        if (!(objChPair.second)->isInInventory(objChPair.first))
            return false;
    return true;
}
