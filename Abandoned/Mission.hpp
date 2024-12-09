#pragma once
#include <string>
#include <map>
#include <set>

class Character;
class Object;

class Mission
{
private:
	class task {
	protected:
		Mission* _parentMission;
		std::string _taskCode;

	public:
		virtual bool isReady() = 0;
	};
	class bringTask : public task
	{
		std::map<int, Character*> _objForCharacter;

	public:
		bringTask(std::string taskCode, Mission* parentMission);

		void addObjForCharacter(int objId, Character* aimCharacter);

		bool isReady() override;
	};

	Character* _employer;
	Character* _mercenary;
	std::string _missionCode;
	std::set<task*> _tasks;

	int _reward;
	int _reputationMult;

public:
	Mission(Character* employer, std::string missionCode, int rewar, int repMult = 1);

	void setMerc(Character* merc);
	Character* getMerc() const;

	Character* getEmployer();

	bool isReady();

	std::string getTaskTitle();

	void closeTask();
};



