#pragma once
#include "MapObject.hpp"
#include "Object.hpp"
#include "Character.hpp"

class AdminPanel
{
public:
	//„исто статический класс с методами работы с игрой
	static std::string AdminProcessPanel(std::string text);
private:
	static std::string Spawn(std::string objName, std::string _pos_x, std::string _pos_y);
	static std::string Set(std::string workName, std::string property, std::string value1, std::string value2 = "");
	static std::string Help();


};