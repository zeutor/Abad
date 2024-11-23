
#pragma once
#include "SFML/Graphics.hpp"

#include "Constants.hpp"
#include "Character.hpp"

#include <unordered_set>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
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
        ItemType _itemType;

        int _itemID;

        bool _isInInventory;

        sf::Vector2f _position;
        sf::Sprite _sprite;

        int _masterId;

        std::unordered_set<int> _effectIDs;

        static std::unordered_set<Object*> _allObjects;
    public:

        Object(ItemType type, const sf::Vector2f& pos, const sf::Sprite& sprite, bool IsInInv, int itemID, std::unordered_set<int> effectIds);
        
        // #######################################################
        // ################ LOAD/SAVE ZONE #######################
        // #######################################################

        static void Load(const std::string& filename);
        void Save(const std::string& filename);
        void saveAllObjects(const std::string& filename);

        // #######################################################
        // ################## SET/GET ZONE #######################
        // #######################################################

        ItemType getItemType() const;

        bool isInInventory() const;
        void togleIsInventory();

        void setItemId(int ItemId);
        int getItemId();

        const sf::Vector2f& getPosition() const;
        void setPosition(const sf::Vector2f& newPos);

        const sf::Sprite& getSprite() const;
        void setSprite(const sf::Sprite& newSprite);

        void setMasterID(int MasterID);
        const int getMasterID() const;


        // #######################################################
        // ################## OTHERS ZONE #######################
        // #######################################################

        const std::unordered_set<int>& getEffectIDs() const;
        void addEffectID(int effectID);
        void removeEffectID(int effectID);

        std::string getEffectIDsAsString();

        virtual void Use(Character& Entity);

        virtual ~Object() = default;

        static std::unordered_set<Object*>& getAllObjects();
};

void MouseTake(Character& player,  std::unordered_set<Object*>& objects, sf::Vector2f mousePosition);

//class Item : Object
//{
//    virtual void Use(Character &Entity)
//    {
//
//
//    }
//};


