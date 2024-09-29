#include <SFML/Graphics.hpp>
#include "unordered_set"
#include "Player.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include "Constants.hpp"

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

        ItemType getItemType() const { return itemType; }
        bool getIsInInventory() const { return isInInventory; }
        void setIsInInventory(bool inInventory) { isInInventory = inInventory; }

        void setItemId(int ItemId) { itemID = ItemId; }
        int getItemId() { return itemID; }

        const sf::Vector2f& getPosition() const { return position; }
        void setPosition(const sf::Vector2f& newPos) { position = newPos; }

        const sf::Sprite& getSprite() const { return sprite; }
        void setSprite(const sf::Sprite& newSprite) { sprite = newSprite; }

 

        const std::unordered_set<int>& getEffectIDs() const { return effectIDs; }
        void addEffectID(int effectID) { effectIDs.insert(effectID); }
        void removeEffectID(int effectID) { effectIDs.erase(effectID); }
        void setMasterID(int MasterID) { MasterId = MasterID; }
        const int getMasterID() const { return MasterId; }
        virtual void Use(Character& Entity) {};

        virtual ~Object() = default;

};

void MouseTake(Player& player, const std::unordered_set<Object*>& objects, sf::Vector2f mousePosition);

class Item : Object
{
    virtual void Use(Character &Entity)
    {


    }



};


void saveAllObjects(const std::string& filename);