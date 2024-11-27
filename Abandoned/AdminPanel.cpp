#include "AdminPanel.hpp"

std::string AdminPanel::AdminProcessPanel(std::string text)
{
    std::istringstream iss(text);
    std::vector<std::string> args;
    std::string word;

    while (iss >> word) {
        args.push_back(word);
    }

    //Парсим строку по пробелам, здесь рассматриваем все случаи (в будущем дополним)
    if (args[0] == "help")
    {
        
        return Help();
    }
    else if (args[0] == "tp" )
    {
        if (args.size() < 4) {
            std::string res = "Error comand";
            res += "\n tp Character(workName) pos.x pos.y";
            return res;
        }
        return Set(args[1], args[2], args[3]);
    }
    else if (args[0] == "set")
    {
        return "NU TIPO BUDEt =))";
    }
    else if (args[0] == "spawn")
    {
        return Spawn(args[1], args[2], args[3]);
    }
    else
    {
        std::string res = "WRONG command \n";
        res += Help();
        return res;
    }


}

std::string AdminPanel::Spawn(std::string objName, std::string _pos_x, std::string _pos_y)
{
    try {
        // Парсинг аргументов
        int itemId = std::stoi(objName);
        float posX = std::stof(_pos_x);
        float posY = std::stof(_pos_y);

        std::unordered_multiset<Object*> allObj = Object::getAllObjects();

        for (auto obj : allObj)
        {
            if (obj->getItemId() == itemId)
            {
                // Создаем новый объект (конструктором копирования) изменяем особые свойства (по сути принадлежность + в инвенатре ли)
                Object* newObj = new Object(*obj); 
                newObj->setPosition(sf::Vector2f(posX, posY));
                newObj->setMasterID(-1);
                newObj->inInventoryDisable();
                return "Object " + objName + " spawned at (" + _pos_x + ", " + _pos_y + ")";
            }
        }

        return "Error: Object with itemId " + objName + " not found.";
    }
    catch (const std::exception& e) {
        return std::string("Error: Invalid input. ") + e.what();
    }
}

std::string AdminPanel::Set(std::string workName, std::string property, std::string value1, std::string value2)
{
    return "NU TIPO BUDET =)))";
}

std::string AdminPanel::Help()
{
    //вывод на экран общей информации по исопльзованию панели
    std::string res = "Spawn obj itemId pos.x pos.y";
    res += "\n Set Character(workName) (param) arg";
    res += "\n tp Character(workName) pos.x pos.y";
    return res;
}
