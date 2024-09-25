#include <SFML/Graphics.hpp>
#include "string"
#include "Constants.hpp"
#include <iostream>
#include <vector>
#include "Outdata.hpp"

class InventorySlot {
public:
    int slotID;
    int itemID;
    sf::Sprite sprite;
    sf::Keyboard::Key assignedKey;

   

    InventorySlot(int id, const sf::Texture& texture, float x, float y);

 
    InventorySlot(int id, const sf::Texture& texture, float x, float y, sf::Keyboard::Key key);

    void LoadMap();

    void LoadInventory();
    bool isKeyAssigned() const;
    bool isClicked(const sf::Vector2i& mousePos) const;

    void setItem(int itemID);





};

void Listen(std::vector<InventorySlot>& slots);




//, std::unordered_map<int, Item>& items




class UIManager
{
private:
	UIManager() = default;
	static UIManager* _UIController;
    static std::vector<InventorySlot> BottomPanel;

    

public:
    static bool handleSlotClick(sf::Vector2i& mousePos, std::vector<InventorySlot> vectorok);
	UIManager(UIManager const&) = delete;
	void operator= (UIManager const&) = delete;
	bool IsOnUIClicked(sf::RenderWindow* window);
	sf::RectangleShape SetMinimap(sf::View &miniMapView);
    static std::vector<InventorySlot> getInvConroller();
	 static UIManager* getController();
    
};




