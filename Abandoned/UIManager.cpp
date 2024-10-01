#include "UIManager.hpp"

UIManager* UIManager::_UIController = nullptr;
std::vector<UISlot> UIManager::BottomPanel;
std::vector<sf::Sprite> UIManager::vectorWithIcons;
sf::RectangleShape UIManager::_inventoryBox;
sf::RectangleShape UIManager::_menuBox;
std::vector<bool> UIManager::MenuJournalInventory;


bool UIManager::handleSlotClick(sf::Vector2i& mousePos, std::vector<UISlot> vectorok)
{
    
        for (UISlot& slot : vectorok) {
            if (slot.isClicked(mousePos) ) {
                //auto it = items.find(slot.itemID);   && slot.itemID != -1
                //if (it != items.end()) {
             
                //   // it->second.use();  
                //}
                return true;
            }
        }
   
    return false;
    
}

bool UIManager::IsOnUIClicked(sf::RenderWindow* window,  sf::Vector2i& mousePos)
{
    if (handleSlotClick(mousePos, BottomPanel)) {
        return true; 
    }


    sf::FloatRect miniMapBounds(WINDOW_WIDTH * miniMapLeft, WINDOW_HEIGHT * miniMapTop,
        WINDOW_WIDTH * miniMapWidth, WINDOW_HEIGHT * miniMapHeight);
    if (miniMapBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        return true; 
    }


    for (auto& button : vectorWithIcons) {
        if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            return true;
        }
    }
    sf::Vector2f mouseIntPos(mousePos.x, mousePos.y) ;
    if(MenuJournalInventory[1]) {
    if (_inventoryBox.getGlobalBounds().contains(mouseIntPos))
    {
        return true;
    }
    }
    if (MenuJournalInventory[0]) {
        if (_menuBox.getGlobalBounds().contains(mouseIntPos))
        {
            return true;

        }
    }

    return false;
}

sf::RectangleShape UIManager::SetMinimap( sf::View &miniMapView)
{
    sf::FloatRect minimapSizes(miniMapLeft, miniMapTop, miniMapWidth, miniMapHeight);
    miniMapView.setViewport(minimapSizes);
    sf::RectangleShape miniMapBorder;
    miniMapBorder.setSize(sf::Vector2f(WINDOW_WIDTH * miniMapWidth, WINDOW_HEIGHT * miniMapHeight));
    miniMapBorder.setPosition(WINDOW_WIDTH * miniMapLeft, WINDOW_HEIGHT * miniMapTop);
    miniMapBorder.setFillColor(sf::Color::Transparent);
    miniMapBorder.setOutlineColor(sf::Color::Black);
    miniMapBorder.setOutlineThickness(3.f);
    return miniMapBorder;
}

std::vector<UISlot> UIManager::getInvConroller()
{
    if (_UIController->BottomPanel.empty())
    { 
        std::vector<UISlot> interfaceSlots;

        int spellSlots = 5;
        float spellSlotSize = 50.0f;
        float spellSlotPadding = 0.0f;


        float totalSpellWidth = spellSlots * (spellSlotSize + spellSlotPadding) - spellSlotPadding;
        float spellStartX = (WINDOW_WIDTH - totalSpellWidth) / 2.0f; 


        for (int i = 0; i < spellSlots-1; ++i)
        {
            UISlot spellSlot(i + 1, outdata::action_cell, spellStartX + i * (spellSlotSize + spellSlotPadding), WINDOW_HEIGHT - 200.0f);
            interfaceSlots.push_back(spellSlot);
        }
        UISlot spellSlot(spellSlots + 1, outdata::action_cell, spellStartX + spellSlots * (spellSlotSize + spellSlotPadding) - 50, WINDOW_HEIGHT - 200.0f, sf::Keyboard::Z);
        interfaceSlots.push_back(spellSlot);

        int itemSlots = 40;
        float itemSlotSize = 50.0f;
        float itemSlotPadding = 0.0f;
        int itemsPerRow = 10;
        int itemRows = itemSlots / itemsPerRow;


        float totalItemWidth = itemsPerRow * (itemSlotSize + itemSlotPadding) - itemSlotPadding;
        float itemStartX = (WINDOW_WIDTH - totalItemWidth) / 2.0f;  

        for (int i = 0; i < itemSlots; ++i)
        {
            int row = i / itemsPerRow; 
            int column = i % itemsPerRow; 

            UISlot itemSlot(i + 1, outdata::empty_cell, itemStartX + column * (itemSlotSize + itemSlotPadding), WINDOW_HEIGHT - 100.0f + row * (itemSlotSize + itemSlotPadding) - 50.0f);
            interfaceSlots.push_back(itemSlot);
        }
        BottomPanel = interfaceSlots;
        return BottomPanel;
    }
    return _UIController->BottomPanel;
}

UIManager* UIManager::getController()
{
    if (!_UIController)
        MenuJournalInventory.push_back(0);
    MenuJournalInventory.push_back(0);
    MenuJournalInventory.push_back(0);
        _UIController = new UIManager();
    return _UIController;
}

std::vector<sf::Sprite> UIManager::LoadIcons(std::vector<sf::Sprite> b)
{
    int VectorSize = b.size();
    sf::Vector2f VectorForPos(20 * PIXELS_PER_CELL, 0.05 * PIXELS_PER_CELL);
    for (int i = 0; i < VectorSize; ++i) {
        b[i].setScale(3, 3);
        VectorForPos.x = VectorForPos.x - 50;
        b[i].setPosition(VectorForPos);
    }
    vectorWithIcons = b;
    return b;

}

void UIManager::LoadMenu(sf::RenderWindow& window, sf::Event& event)
{
    UIManager* _UIController = UIManager::getController();
    static bool SettingsOpen;
    sf::RectangleShape menuBox;
    menuBox.setSize(sf::Vector2f(50 * 4 + 20 + 35, ICON_SIZE * 4 + 20 + 45)); // Размер `menuBox`
    menuBox.setFillColor(sf::Color(50, 50, 50, 200));
    menuBox.setPosition(
        (WINDOW_WIDTH - menuBox.getSize().x) / 2,
        (WINDOW_HEIGHT - menuBox.getSize().y) / 2 - 60
    );

    _menuBox = menuBox;

    sf::Sprite downloadSprite(outdata::Download_game);
    sf::Sprite startSprite(outdata::Start_game);
    sf::Sprite settingsSprite(outdata::Settings);
    sf::Sprite exitSprite(outdata::Exit);

    downloadSprite.setScale(SIZE_MULTIPLIER*0.5, SIZE_MULTIPLIER*0.5);
    
    startSprite.setScale(SIZE_MULTIPLIER*0.5, SIZE_MULTIPLIER*0.5);
    settingsSprite.setScale(SIZE_MULTIPLIER*0.5, SIZE_MULTIPLIER*0.5);
    exitSprite.setScale(SIZE_MULTIPLIER*0.5, SIZE_MULTIPLIER*0.5);





    float centerX = WINDOW_WIDTH / 2 - downloadSprite.getGlobalBounds().width / 2;
    float verticalSpacing = 60.0f;
    downloadSprite.setPosition(centerX, 180);
    startSprite.setPosition(centerX, 180 + verticalSpacing);

    
    settingsSprite.setPosition(centerX, 180 + 2 * verticalSpacing);
    exitSprite.setPosition(centerX, 180 + 3 * verticalSpacing);



    MenuJournalInventory[0] = true;

    if (MenuJournalInventory[0]) {
             if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (downloadSprite.getGlobalBounds().contains(mousePos)) {
           
                }
               
                else if (startSprite.getGlobalBounds().contains(mousePos)) {
               
                }
                else if (settingsSprite.getGlobalBounds().contains(mousePos)) {
              
                    SettingsOpen = true;
                }
                else if (exitSprite.getGlobalBounds().contains(mousePos)) {

                    MenuJournalInventory[0] = false;
                }
            }
        

            
        window.draw(menuBox);
        window.draw(downloadSprite);
        window.draw(startSprite);
        window.draw(settingsSprite);
        window.draw(exitSprite);
        if (SettingsOpen)

        {
            SettingsOpen = _UIController->LoadSettings(window, event);
        }

    }
}

bool UIManager::LoadSettings(sf::RenderWindow& window, sf::Event& event)
{
    sf::RectangleShape SettingsBox;
    sf::RectangleShape SettingsBox1;
    SettingsBox.setSize(sf::Vector2f(50 * 4 * 4 + 20 + 35, ICON_SIZE * 4 * 3 + 20 + 45));
    SettingsBox.setFillColor(sf::Color(50, 50, 110, 200));
    SettingsBox.setPosition(
        (WINDOW_WIDTH - SettingsBox.getSize().x) / 2,
        (WINDOW_HEIGHT - SettingsBox.getSize().y) / 2 - 20
    );
    SettingsBox1.setSize(sf::Vector2f(10* 2 + 20 , ICON_SIZE)); // Размер `menuBox`
    SettingsBox1.setFillColor(sf::Color(50, 50, 50, 200));
    SettingsBox1.setPosition(
        (WINDOW_WIDTH - SettingsBox1.getSize().x) / 2 + 405,
        (WINDOW_HEIGHT - SettingsBox1.getSize().y) / 2 - 327
    );

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (SettingsBox1.getGlobalBounds().contains(mousePos)) {
          
            return false;
        }
       
    }
    window.draw(SettingsBox);
    window.draw(SettingsBox1);
    return true;
}







 UISlot::UISlot(int id, const sf::Texture& texture, float x, float y)
    : slotID(id), actionID(-1), assignedKey(sf::Keyboard::Unknown)  
{
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(50.0f / texture.getSize().x, 50.0f / texture.getSize().y);
}

UISlot::UISlot(int id, const sf::Texture& texture, float x, float y, sf::Keyboard::Key key)
    : slotID(id), actionID(-1), assignedKey(key) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(50.0f / texture.getSize().x, 50.0f / texture.getSize().y);
}

 void UISlot::LoadMap()
{
    //
}

 void UIManager::LoadInventory(sf::RenderWindow& window, Player& player, std::unordered_set<Object*> AllObject, sf::Event& event)
 {
     std::multiset<int> inventory = player.GetInventory();
     sf::Vector2u windowSize = window.getSize();
     const float padding = 5.0f;
     const float boxPadding = 0.0f;

     sf::RectangleShape inventoryBox;
     inventoryBox.setSize(sf::Vector2f(ICON_SIZE * INVENTORY_COLS + boxPadding + 35, ICON_SIZE * INVENTORY_ROWS + boxPadding + 45));
     inventoryBox.setFillColor(sf::Color(50, 50, 50, 200));
     inventoryBox.setPosition((windowSize.x - inventoryBox.getSize().x) / 2 + 200, (windowSize.y - inventoryBox.getSize().y) / 2);
     window.draw(inventoryBox);
     _inventoryBox = inventoryBox;

     sf::Sprite iconSprite;
     int iconCount = 0;
     static bool MouseCount = true;

     sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));


     for (int row = 0; row < INVENTORY_ROWS; ++row) {
         for (int col = 0; col < INVENTORY_COLS; ++col) {
             sf::RectangleShape slot(sf::Vector2f(ICON_SIZE, ICON_SIZE));
             float xPos = inventoryBox.getPosition().x + padding + col * (ICON_SIZE + padding);
             float yPos = inventoryBox.getPosition().y + padding + row * (ICON_SIZE + padding);
             slot.setPosition(xPos, yPos);
             slot.setFillColor(sf::Color(100, 100, 100));
             window.draw(slot);


             if (iconCount < inventory.size()) {
    
                 auto itemIter = std::next(inventory.begin(), iconCount);
                 int itemID = *itemIter;

                 for (const auto& obj : AllObject) {
                     if (obj->getItemId() == itemID) {
                       
                         obj->setPosition(slot.getPosition());

               
                         iconSprite = obj->getSprite();
                         if (iconSprite.getTexture() != nullptr) {
                             iconSprite.setPosition(slot.getPosition());
                             iconSprite.setScale(3, 3);
                             window.draw(iconSprite);

                        
                             if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                                 if (slot.getGlobalBounds().contains(mousePos) && MouseCount) {
                                   
                                     MouseCount = false;
                                     std::cout << "Left Clicked Item ID: " << obj->getItemId() << std::endl;
                                 }
                             }

                          
                             if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                                 if (slot.getGlobalBounds().contains(mousePos) && MouseCount) {
                                     MouseCount = false;

                                     
                                     player.RemoveFromInventory(itemID);

                                   
                                     sf::Vector2f newPosition = player.getPosition();
                                     newPosition.x += player.getSprite().getGlobalBounds().width + 10;
                                     obj->setPosition(newPosition);
                                     obj->setMasterID(-1);
                                     obj->setIsInInventory(false);

                                   
                                 }
                             }
                         }
                         break; 
                     }
                 }
             }
             ++iconCount;
         }
     }

     if (event.type == sf::Event::MouseMoved) {
         MouseCount = true;
     }
 }




bool UISlot::isKeyAssigned() const {
    return assignedKey != sf::Keyboard::Unknown;
}

bool UISlot::isClicked(const sf::Vector2i& mousePos) const
{
    {
        return sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }
}

 void UISlot::setItem(int itemID) {
    this->actionID = itemID;
}
 
void Listen(std::vector<UISlot>& slots, sf::Event event)
{

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::I) {
            UIManager::MenuJournalInventory[1] = !UIManager::MenuJournalInventory[1];

        }
        else if (event.key.code == sf::Keyboard::Escape) {
            UIManager::MenuJournalInventory[0] = !UIManager::MenuJournalInventory[0];
        }
        else{
            for (auto& slot : slots) {
                if (slot.isKeyAssigned() && event.key.code ==  slot.assignedKey) {

                    // slot.useItem();
                }
            }
        }
    }


       
    

}
