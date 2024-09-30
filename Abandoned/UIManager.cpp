#include "UIManager.hpp"

UIManager* UIManager::_UIController = nullptr;
std::vector<UISlot> UIManager::BottomPanel;
std::vector<sf::Sprite> UIManager::vectorWithIcons;
sf::RectangleShape UIManager::_inventoryBox;
bool UIManager::InventoryOpen = false;

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
    if(InventoryOpen){
    if (_inventoryBox.getGlobalBounds().contains(mouseIntPos))
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

        // Параметры заклинаний
        int spellSlots = 5;
        float spellSlotSize = 50.0f;
        float spellSlotPadding = 0.0f;

        // Вычисляем центр для заклинаний
        float totalSpellWidth = spellSlots * (spellSlotSize + spellSlotPadding) - spellSlotPadding;
        float spellStartX = (WINDOW_WIDTH - totalSpellWidth) / 2.0f;  // Центрируем заклинания

        // Отрисовка заклинаний (без идентификаторов)
        for (int i = 0; i < spellSlots-1; ++i)
        {
            UISlot spellSlot(i + 1, outdata::action_cell, spellStartX + i * (spellSlotSize + spellSlotPadding), WINDOW_HEIGHT - 200.0f);
            interfaceSlots.push_back(spellSlot);
        }
        UISlot spellSlot(spellSlots + 1, outdata::action_cell, spellStartX + spellSlots * (spellSlotSize + spellSlotPadding) - 50, WINDOW_HEIGHT - 200.0f, sf::Keyboard::Z);
        interfaceSlots.push_back(spellSlot);

        // Параметры предметов
        int itemSlots = 40;
        float itemSlotSize = 50.0f;
        float itemSlotPadding = 0.0f;
        int itemsPerRow = 10;
        int itemRows = itemSlots / itemsPerRow;

        // Вычисляем центр для предметов
        float totalItemWidth = itemsPerRow * (itemSlotSize + itemSlotPadding) - itemSlotPadding;
        float itemStartX = (WINDOW_WIDTH - totalItemWidth) / 2.0f;  // Центрируем предметы

        // Отрисовка предметов с ID
        for (int i = 0; i < itemSlots; ++i)
        {
            int row = i / itemsPerRow;  // Ряд
            int column = i % itemsPerRow;  // Колонка

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


//, std::unordered_map<int, Item>& items



 UISlot::UISlot(int id, const sf::Texture& texture, float x, float y)
    : slotID(id), actionID(-1), assignedKey(sf::Keyboard::Unknown)  // -1 означает, что в ячейке нет предмета
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

     sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)); // Получаем позицию мыши

     // Цикл для отображения слотов и иконок предметов
     for (int row = 0; row < INVENTORY_ROWS; ++row) {
         for (int col = 0; col < INVENTORY_COLS; ++col) {
             sf::RectangleShape slot(sf::Vector2f(ICON_SIZE, ICON_SIZE));
             float xPos = inventoryBox.getPosition().x + padding + col * (ICON_SIZE + padding);
             float yPos = inventoryBox.getPosition().y + padding + row * (ICON_SIZE + padding);
             slot.setPosition(xPos, yPos);
             slot.setFillColor(sf::Color(100, 100, 100));
             window.draw(slot);

             // Проверка наличия предмета в инвентаре
             if (iconCount < inventory.size()) {
                 // Поиск предмета по itemID и привязка его к слоту
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
 
void Listen(std::vector<UISlot>& slots)
{
    

        for (auto& slot : slots) {
            if (slot.isKeyAssigned() && sf::Keyboard::isKeyPressed(slot.assignedKey)) {
                // slot.useItem();
            }
        }
    

}
