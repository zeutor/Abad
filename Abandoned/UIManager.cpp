#include "UIManager.hpp"

UIManager* UIManager::_UIController = nullptr;
std::vector<InventorySlot> UIManager::BottomPanel;


bool UIManager::handleSlotClick(sf::Vector2i& mousePos, std::vector<InventorySlot> vectorok)
{
    
        for (InventorySlot& slot : vectorok) {
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

bool UIManager::IsOnUIClicked(sf::RenderWindow* window)
{
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);


            sf::FloatRect miniMapBounds(
                WINDOW_WIDTH * miniMapLeft,
                WINDOW_HEIGHT * miniMapTop,
                WINDOW_WIDTH * miniMapWidth,
                WINDOW_HEIGHT * miniMapHeight
            );


            if (miniMapBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {

                return true;

            }

            return false;
        }

    }
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

std::vector<InventorySlot> UIManager::getInvConroller()
{
    if (_UIController->BottomPanel.empty())
    { 
        std::vector<InventorySlot> interfaceSlots;

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
            InventorySlot spellSlot(i + 1, outdata::action_cell, spellStartX + i * (spellSlotSize + spellSlotPadding), WINDOW_HEIGHT - 200.0f);
            interfaceSlots.push_back(spellSlot);
        }
        InventorySlot spellSlot(spellSlots + 1, outdata::action_cell, spellStartX + spellSlots * (spellSlotSize + spellSlotPadding) - 50, WINDOW_HEIGHT - 200.0f, sf::Keyboard::Z);
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

            InventorySlot itemSlot(i + 1, outdata::empty_cell, itemStartX + column * (itemSlotSize + itemSlotPadding), WINDOW_HEIGHT - 100.0f + row * (itemSlotSize + itemSlotPadding) - 50.0f);
            interfaceSlots.push_back(itemSlot);
        }
        return interfaceSlots;
    }
    return _UIController->BottomPanel;
}

UIManager* UIManager::getController()
{
    if (!_UIController)
        _UIController = new UIManager();
    return _UIController;
}


//, std::unordered_map<int, Item>& items



 InventorySlot::InventorySlot(int id, const sf::Texture& texture, float x, float y)
    : slotID(id), itemID(-1), assignedKey(sf::Keyboard::Unknown)  // -1 означает, что в ячейке нет предмета
{
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(50.0f / texture.getSize().x, 50.0f / texture.getSize().y);
}

InventorySlot::InventorySlot(int id, const sf::Texture& texture, float x, float y, sf::Keyboard::Key key)
    : slotID(id), itemID(-1), assignedKey(key) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(50.0f / texture.getSize().x, 50.0f / texture.getSize().y);
}

 void InventorySlot::LoadMap()
{
    //
}

 void InventorySlot::LoadInventory()
{
    //
}

bool InventorySlot::isKeyAssigned() const {
    return assignedKey != sf::Keyboard::Unknown;
}

bool InventorySlot::isClicked(const sf::Vector2i& mousePos) const
{
    {
        return sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }
}

 void InventorySlot::setItem(int itemID) {
    this->itemID = itemID;
}
 
void Listen(std::vector<InventorySlot>& slots)
{
    

        for (auto& slot : slots) {
            if (slot.isKeyAssigned() && sf::Keyboard::isKeyPressed(slot.assignedKey)) {
                // slot.useItem();
                std::cout << "YESS YOU CLICKED A BUBTON";
            }
        }
    

}
