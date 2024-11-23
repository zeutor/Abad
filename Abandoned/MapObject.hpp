#pragma once
#include "SFML/Graphics.hpp"
#include "set"
#include "unordered_set"
#include <iostream>
#include "Character.hpp"
#include "UIManager.hpp"
#include <sstream>

class UIManager;
class Character;

class MapObject
{
protected:
    float interactionRange;
    int masterId;
    sf::Vector2f _position;

    

public:

    virtual void Use() = 0;
    static std::unordered_set<MapObject*> _allMapObjects;

    MapObject(sf::Vector2f position) : _position(position), interactionRange(0), masterId(-100){}
    MapObject(sf::Vector2f position, int masterIDD) : _position(position), interactionRange(0), masterId(masterIDD) {}
    static std::unordered_set<MapObject*> getAllMapObjects();

    //рядом ли персик
    bool IsPlayerInRange(Character& player)
    {
       
        float range = 100.0f; 

        sf::FloatRect certainedBounds(
            _position.x - 60,
            _position.y - 60,
            166, 
            166
        );

        return certainedBounds.contains(player.getPosition());
    }

    // Проверка, был ли клик по табличке
    bool IsMouseOverTable(sf::Vector2f mousePos)
    {

       
        sf::FloatRect certainedBounds(
            _position.x,
            _position.y,
            46,
            46
        );
        
        return certainedBounds.contains(mousePos);
    }

    int getMasterID();

    
  

    void setPosition(const sf::Vector2f& position)
    {
        _position = position;
    }


    sf::Vector2f getPosition() const
    {
        return _position;
    }


};


class Storage : public MapObject
{
private:

   std::multiset<int> storage_inventory;
public:
    Storage(sf::Vector2f position, std::multiset<int> stor_inv, int masterIDD)
        : storage_inventory(stor_inv), MapObject(position, masterIDD) {}

    //Открытие сундука/хранилища с предметами и инвентаря для обмена
   virtual void Use() override;
   //Добавить/удалить предмет
   void insertItem(int itemID);
   void DeleteItem(int itemId);

   std::multiset<int> GetInventory();

   //Вывести все элементы Storage
   void printAllItems();
   

};

class Table : public MapObject
{
private:
    std::string text;



   

public:

    Table(sf::Vector2f position, const std::string& tableText)
        : text(tableText), MapObject(position) {}

 virtual void Use() override
    {
        //мейби еще чет, там триггер какой-нибудь для квеста и тыды
        //снизу это контекст мнню для диалога
        ShowContextMenu();
    }
    // Отображение контекстного меню с текстом
    void ShowContextMenu();

   
};
//Загрузка всех MapObjects
void LoadMapObjects(const std::string& filePath);

//Функция для активации MapObjects (проверяет рядом ли персонаж и нажата ли  ЛКМ туда (при отходе выключается)
void ActivateMapObj(Character& player, sf::Vector2f mousePos, std::unordered_set<MapObject*> _allMapObjects);