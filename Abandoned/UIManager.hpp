#include <SFML/Graphics.hpp>
#include "string"
#include "Constants.hpp"
#include <iostream>
#include <vector>
#include "Outdata.hpp"
#include <set>
#include <unordered_set>
#pragma once
#include "Object.hpp"

class Object;

class UISlot {
public:
    int slotID;
    int actionID;
    sf::Sprite sprite;
    sf::Keyboard::Key assignedKey;

   

    UISlot(int id, const sf::Texture& texture, float x, float y);

 
    UISlot(int id, const sf::Texture& texture, float x, float y, sf::Keyboard::Key key);

    void LoadMap();


   
    bool isKeyAssigned() const;
    bool isClicked(const sf::Vector2i& mousePos) const;

    void setItem(int itemID);
};

void Listen(std::vector<UISlot>& slots, sf::Event event);




//, std::unordered_map<int, Item>& items




class UIManager
{
private:
	UIManager() = default;
	static UIManager* _UIController;
    static std::vector<UISlot> BottomPanel;
    static std::vector<sf::Sprite> vectorWithIcons;
    static  sf::RectangleShape _inventoryBox;
    static sf::RectangleShape _menuBox;;


public:
    static std::vector<bool> MenuJournalInventory;
    void LoadInventory(sf::RenderWindow& window, Player& player, std::unordered_set<Object*> AllObject, sf::Event& event);

    static bool handleSlotClick(sf::Vector2i& mousePos, std::vector<UISlot> vectorok);
	UIManager(UIManager const&) = delete;
	void operator= (UIManager const&) = delete;
   static bool IsOnUIClicked(sf::RenderWindow* window, sf::Vector2i& mousePos);
	sf::RectangleShape SetMinimap(sf::View &miniMapView);
    static std::vector<UISlot> getInvConroller();
	 static UIManager* getController();
      std::vector<sf::Sprite>LoadIcons(std::vector<sf::Sprite>);
      void LoadMenu(sf::RenderWindow& window,  sf::Event& event);
     bool LoadSettings(sf::RenderWindow& window, sf::Event& event);

    
    
};




