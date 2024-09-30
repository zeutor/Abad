#include <SFML/Graphics.hpp>
#include "unordered_set"
#include "Player.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include "Constants.hpp"
#pragma once
#include "UIManager.hpp"

class Character;



class Object
{
public:

    enum class ItemType {
        Weapon, // оружие
        Armor, //броня
        Consumables, // расходники(зелья, свитки)
        Miscellaneous // разное(книги, квест предметы и тд)
    };

    private:
        ItemType itemType;
        int itemID;
        bool isInInventory;
        sf::Vector2f position;
        sf::Sprite sprite;

        int MasterId;
        std::unordered_set<int> effectIDs;
        static std::unordered_set<Object*> allObjects;

    public:
      
        static std::unordered_set<Object*>& getAllObjects();
            
        static void Load(const std::string& filename);

        Object(ItemType type, const sf::Vector2f& pos, const sf::Sprite& sprite, bool IsInInv, int itemID, std::unordered_set<int> effectIds)
            : itemType(type), isInInventory(IsInInv), position(pos),  sprite(sprite), itemID(itemID), effectIDs(effectIds) {}
  
         void Save(const std::string& filename);
         std::string getEffectIDsAsString();

         ItemType getItemType() const { return itemType;}
        bool getIsInInventory() const;
        void setIsInInventory(bool inInventory);

        void setItemId(int ItemId);
        int getItemId();

        const sf::Vector2f& getPosition() const;
        void setPosition(const sf::Vector2f& newPos);

        const sf::Sprite& getSprite() const;
        void setSprite(const sf::Sprite& newSprite);

 

        const std::unordered_set<int>& getEffectIDs() const;
        void addEffectID(int effectID);
        void removeEffectID(int effectID);
        void setMasterID(int MasterID);
        const int getMasterID() const;
        virtual void Use(Character& Entity) {};

        virtual ~Object() = default;

};

void MouseTake(Player& player,  std::unordered_set<Object*>& objects, sf::Vector2f mousePosition);

class Item : Object
{
    virtual void Use(Character &Entity)
    {


    }



};


void saveAllObjects(const std::string& filename);