
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
        int uniqueId;
        std::unordered_set<int> _effectIDs;

        static std::unordered_multiset<Object*> _allObjects;
    public:

        Object(const Object& other)
            : _itemType(other._itemType),    
            _itemID(other._itemID),            
            _isInInventory(other._isInInventory),
            _position(other._position),        
            _sprite(other._sprite),    
            _masterId(other._masterId),     
            _effectIDs(other._effectIDs)   
        {
            Object::ObjCount = 1+ Object::ObjCount;
            uniqueId = Object::ObjCount;
            _allObjects.insert(this);
            std::cout << Object::ObjCount;
        }


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
        void inInventoryDisable();
        int getItemId();
        int getUniqueId();

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
        static int ObjCount;
        static std::unordered_multiset<Object*>& getAllObjects();
};

void MouseTake(Character& player,  std::unordered_multiset<Object*>& objects, sf::Vector2f mousePosition);

//class Item : Object
//{
//    virtual void Use(Character &Entity)
//    {
//
//
//    }
//};


