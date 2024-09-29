#include <SFML/Graphics.hpp>
#include "string"
#include "Constants.hpp"
#include <iostream>
#include <vector>
#include "Outdata.hpp"

class UISlot {
public:
    int slotID;
    int actionID;
    sf::Sprite sprite;
    sf::Keyboard::Key assignedKey;

   

    UISlot(int id, const sf::Texture& texture, float x, float y);

 
    UISlot(int id, const sf::Texture& texture, float x, float y, sf::Keyboard::Key key);

    void LoadMap();

    void LoadInventory();
    bool isKeyAssigned() const;
    bool isClicked(const sf::Vector2i& mousePos) const;

    void setItem(int itemID);





};

void Listen(std::vector<UISlot>& slots);




//, std::unordered_map<int, Item>& items




class UIManager
{
private:
	UIManager() = default;
	static UIManager* _UIController;
    static std::vector<UISlot> BottomPanel;

    

public:
    static bool handleSlotClick(sf::Vector2i& mousePos, std::vector<UISlot> vectorok);
	UIManager(UIManager const&) = delete;
	void operator= (UIManager const&) = delete;
	bool IsOnUIClicked(sf::RenderWindow* window);
	sf::RectangleShape SetMinimap(sf::View &miniMapView);
    static std::vector<UISlot> getInvConroller();
	 static UIManager* getController();
    
};




