#include "UIManager.hpp"
#include "Outdata.hpp"
#include "Outdata.hpp"
#include <locale>
#include <codecvt>
#include "GameCamera.hpp"

sf::RenderWindow* UIManager::_windowToDisplay = nullptr;
UIManager* UIManager::_UIController = nullptr;

using namespace sf;
using namespace std;

bool UIManager::isWindowOpen(int winID)
{
    if (winID < _countOfWindows)
        return _isUIWindowOpen[winID];
    return false;
}

void UIManager::togleWindow(int winID)
{
    if (winID == 1 && isWindowOpen(5))
    {
        disableWindow(5);
        disableWindow(1);
    }
    else if (winID < _countOfWindows)
        _isUIWindowOpen[winID] = !_isUIWindowOpen[winID];
    
}

void UIManager::disableWindow(int winID)
{
    if (winID < _countOfWindows)
        _isUIWindowOpen[winID] = false;
}

// Is clicked on bottom-slot UI
bool UIManager::handleSlotClick(sf::Vector2i& mousePos)
{
    for (UISlot& slot : _bottomPanel) 
    {
        if (slot.isClicked(mousePos))
        {
            // Set item as useable
            return true;
        }
    }
    return false;
}

bool UIManager::IsOnUIClicked(sf::Vector2i& mousePos)
{
    if (handleSlotClick(mousePos)) 
    {
        return true; 
    }


    // is clicked on top right icons
    for (auto& button : _vectorWithIcons) 
    {
        if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) 
        {
            return true;
        }
    }


    if(isWindowOpen(1)) 
    {
        if (_inventoryBox.getGlobalBounds().contains((Vector2f)mousePos))
        {
            return true;
        }
    }
    if (isWindowOpen(0))
    {
        if (_menuBox.getGlobalBounds().contains((Vector2f)mousePos))
        {
            return true;
        }
    }

    return false;
}

bool UIManager::isAnyWindowOpen()
{
    return _isAnyWindowOpen;
}

std::vector<UISlot> UIManager::getInvConroller()
{
    if (_bottomPanel.empty())
    {
        std::vector<UISlot> interfaceSlots;

        // Коэффициенты масштабирования
        float coef_x = _windowToDisplay->getSize().x / WINDOW_WIDTH;
        float coef_y = _windowToDisplay->getSize().y / WINDOW_HEIGHT;

        // Основные параметры для слотов с очками действий/очками иного чего-нибудь (слоты заклинаний - временное название)
        int spellSlots = 5;
        sf::Sprite action_cell(outdata::action_cell);
        action_cell.setScale(3 * coef_x, 3 * coef_y);
        float spellSlotSize = action_cell.getGlobalBounds().width;
        float spellSlotHeight = action_cell.getGlobalBounds().height;

        // Основные параметры для слотов с заклинаниями 
        int itemSlots = 30;
        sf::Sprite empty_cell(outdata::empty_cell);
        empty_cell.setScale(3 * coef_x, 3 * coef_y);
        float itemSlotSize = empty_cell.getGlobalBounds().width;
        float itemSlotHeight = empty_cell.getGlobalBounds().height;

        int itemsPerRow = 10;
        int itemRows = itemSlots / itemsPerRow;

        // Расчет начальной позиции для слотов предметов
        float totalItemWidth = itemsPerRow * itemSlotSize;
        float itemStartX = (_windowToDisplay->getSize().x - totalItemWidth) / 2.0f;
        float itemStartY = _windowToDisplay->getSize().y - itemSlotHeight;

        // Создание слотов предметов
        for (int i = 0; i < itemSlots; ++i)
        {
            int row = i / itemsPerRow;
            int column = i % itemsPerRow;

            UISlot itemSlot(i + 1, empty_cell,
                itemStartX + column * itemSlotSize,
                itemStartY - row * itemSlotHeight); // Ряды поднимаются вверх
            interfaceSlots.push_back(itemSlot);
        }

        // Расчет начальной позиции для слотов заклинаний
        float totalSpellWidth = spellSlots * spellSlotSize;
        float spellStartX = (_windowToDisplay->getSize().x - totalSpellWidth) / 2.0f;
        float spellStartY = itemStartY - itemRows * itemSlotHeight; // Верхний край предметов

        // Создание слотов заклинаний
        for (int i = 0; i < spellSlots; ++i)
        {
            UISlot spellSlot(i + 1, action_cell,
                spellStartX + i * spellSlotSize,
                spellStartY); // Заклинания соприкасаются с предметами
            interfaceSlots.push_back(spellSlot);
        }

        _bottomPanel = interfaceSlots;

        return _bottomPanel;
    }
    return _bottomPanel;



}

void UIManager::Update()
{
    _isAnyWindowOpen = _isUIWindowOpen[0];
    for (int i = 1; i < _countOfWindows; ++i)
        _isAnyWindowOpen = _isAnyWindowOpen || _isUIWindowOpen[i];
}

UIManager* UIManager::getController()
{
    if (!_UIController)
    {
        _UIController = new UIManager();
        for (bool window : _UIController->_isUIWindowOpen)
            window = false;
    }
    return _UIController;
}

std::vector<sf::Sprite> UIManager::LoadIcons(std::vector<sf::Texture> navigationTextures, sf::RenderWindow& window)
{
    //Просчет коэффициентов для последующего маштабирования
    sf::Vector2f currentWindowSizes = (sf::Vector2f)window.getSize();
    float coef_x = currentWindowSizes.x / WINDOW_WIDTH;
    float coef_y = currentWindowSizes.y / WINDOW_HEIGHT;

    _vectorWithIcons.clear();
    //Подгрузка спрайтов основных иконок (меню, инвентарь, журнал)
    _vectorWithIcons.push_back(sf::Sprite(outdata::menu_icon));
    _vectorWithIcons.push_back(sf::Sprite(outdata::invent_icon));
    _vectorWithIcons.push_back(sf::Sprite(outdata::journal_icon));

    //Рассчет коэффициентов для сдвига и расположения иконок
    sf::FloatRect icon_size = _vectorWithIcons[0].getGlobalBounds();
    int VectorSize = navigationTextures.size();
    float firstIconPlace = currentWindowSizes.x - icon_size.width;
    
    //"Край экрана"
    sf::Vector2f VectorForPos(firstIconPlace, 0.05 * PIXELS_PER_CELL);


    //Расположение иконок и их маштабирование
    for (int i = 0; i < VectorSize; ++i) 
    {
        _vectorWithIcons[i].setScale((coef_x*3), (coef_y*3));

        VectorForPos.x = VectorForPos.x - (icon_size.width*3 * coef_x);

        _vectorWithIcons[i].setPosition(VectorForPos);
    }

    return _vectorWithIcons;
}

void UIManager::LoadMenu(sf::Event& event, sf::RenderWindow& window)
{
    //Просчет коэффициентов для последующего маштабирования
    sf::Vector2f currentWindowSizes = (sf::Vector2f)window.getSize();
    float coef_x = currentWindowSizes.x / WINDOW_WIDTH;
    float coef_y = currentWindowSizes.y / WINDOW_HEIGHT;

    // Создание меню, размеров и цвета задней части
    sf::RectangleShape menuBox;
    menuBox.setSize(sf::Vector2f(
        coef_x * (50 * 4 + 20 + 35),  
        coef_y * (ICON_SIZE * 4 + 20 + 45) 
    ));
    menuBox.setFillColor(sf::Color(50, 50, 50, 200));
    menuBox.setPosition(
        coef_x * (WINDOW_WIDTH - menuBox.getSize().x) / 2,
        coef_y * (WINDOW_HEIGHT - menuBox.getSize().y) / 2 - 60 /coef_y
    );

    _menuBox = menuBox;

    // Загрузка кнопок
    sf::Sprite loadSprite(outdata::Download_game);
    sf::Sprite startSprite(outdata::Start_game);
    sf::Sprite settingsSprite(outdata::Settings);
    sf::Sprite exitSprite(outdata::Exit);

    //"Скейлим" иконки на коэфициенты,для адекватного отображения на любых экранах
    loadSprite.setScale(coef_x * SIZE_MULTIPLIER * 0.5, coef_y * SIZE_MULTIPLIER * 0.5);
    startSprite.setScale(coef_x * SIZE_MULTIPLIER * 0.5, coef_y * SIZE_MULTIPLIER * 0.5);
    settingsSprite.setScale(coef_x * SIZE_MULTIPLIER * 0.5, coef_y * SIZE_MULTIPLIER * 0.5);
    exitSprite.setScale(coef_x * SIZE_MULTIPLIER * 0.5, coef_y * SIZE_MULTIPLIER * 0.5);

    //просчет центра по X и отступов между пунктами меню
    float centerX = coef_x * (WINDOW_WIDTH / 2 - loadSprite.getGlobalBounds().width / 2);
    float verticalSpacing = 60.0f * coef_y;

    sf::Vector2f menuBoxPosition = menuBox.getPosition();
    sf::Vector2f menuBoxSize = menuBox.getSize();

    //Просчет стартовых X и Y (начальные координаты, где будет отображаться меню
    float startX = menuBoxPosition.x + (menuBoxSize.x - loadSprite.getGlobalBounds().width) / 2; 
    float startY = menuBoxPosition.y + 20.0f * coef_y; 

    //Распологаем с отступами
    loadSprite.setPosition(startX, startY);
    startSprite.setPosition(startX, startY + verticalSpacing);
    settingsSprite.setPosition(startX, startY + 2 * verticalSpacing);
    exitSprite.setPosition(startX, startY + 3 * verticalSpacing);

    //Рендеринг
    _windowToDisplay->draw(menuBox);
    _windowToDisplay->draw(loadSprite);
    _windowToDisplay->draw(startSprite);
    _windowToDisplay->draw(settingsSprite);
    _windowToDisplay->draw(exitSprite);

    // Проверка нажатий
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(event.mouseButton.x * (1.0f / coef_x), event.mouseButton.y * (1.0f / coef_y));

        if (loadSprite.getGlobalBounds().contains(mousePos))
        {
            // Логика для кнопки "Загрузить"
        }
        else if (startSprite.getGlobalBounds().contains(mousePos))
        {
            // Логика для кнопки "Начать игру"
        }
        else if (settingsSprite.getGlobalBounds().contains(mousePos))
        {
            _isUIWindowOpen[3] = true;
        }
        else if (exitSprite.getGlobalBounds().contains(mousePos))
        {
            _isUIWindowOpen[0] = false;
        }
    }

    //Запуск настроек, при нажатии на соотв. кнопку
    if (_isUIWindowOpen[3])
    {
        _isUIWindowOpen[3] = _UIController->LoadSettings(*_windowToDisplay, event);
    }
}

bool UIManager::LoadSettings(sf::RenderWindow& window, sf::Event& event)
{
    // От большего говоря "Заглушка" первое окно служит основой для будущих настроек
    sf::RectangleShape SettingsBox;
    SettingsBox.setSize(sf::Vector2f(50 * 4 * 4 + 20 + 35, ICON_SIZE * 4 * 3 + 20 + 45));
    SettingsBox.setFillColor(sf::Color(50, 50, 110, 200));
    SettingsBox.setPosition(
        (WINDOW_WIDTH - SettingsBox.getSize().x) / 2,
        (WINDOW_HEIGHT - SettingsBox.getSize().y) / 2 - 20
    );

    //второе окно необходимо для "закрытия настроек"
    sf::RectangleShape crossForExit;
    crossForExit.setSize(sf::Vector2f(10* 2 + 20 , ICON_SIZE)); // Размер `menuBox`
    crossForExit.setFillColor(sf::Color(50, 50, 50, 200));
    crossForExit.setPosition(
        (WINDOW_WIDTH - crossForExit.getSize().x) / 2 + 405,
        (WINDOW_HEIGHT - crossForExit.getSize().y) / 2 - 327
    );

    // todo - в частности реализовать настройки
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (crossForExit.getGlobalBounds().contains(mousePos)) {
          
            return false;
        }
       
    }

    window.draw(SettingsBox);
    window.draw(crossForExit);
    return true;
}

void UIManager::LoadGameUI( sf::Event event, Character& player, sf::RenderWindow& window)
{
    unordered_multiset<Object*> AllObjects = Object::getAllObjects();
    unordered_set<MapObject*> AllMapObjects = MapObject::getAllMapObjects();

    // Drawing obj's out of inventory
    for (const auto& obj : AllObjects) 
    {
        if (!obj->isInInventory()) 
        {
            sf::Vector2f objsPosition = obj->getPosition();

            sf::Sprite objsSprite = obj->getSprite();
            objsSprite.setPosition(objsPosition);
            obj->setSprite(objsSprite);
            window.draw(objsSprite);
        }
    }



    // Loading bottom bar
    if (!isWindowOpen(4)) {
        int countOfSlots = _bottomPanel.size();
        for (int i = 0; i < countOfSlots; ++i)
        {
            window.draw(_bottomPanel[i].getSprite());
        }
    }
    if (isWindowOpen(4)) {
        LoadText(textToDisplay);
    }


    // Loading top right navigation icons
    int countOfIcons = _vectorWithIcons.size();
    for (int i = 0; i < countOfIcons; ++i)
    {
        window.draw(_vectorWithIcons[i]);
    }



    if (isWindowOpen(0))
        LoadMenu(event, window);

    if (isWindowOpen(1) && !isWindowOpen(0)){
        LoadInventory(player, AllObjects, event);
    }
    if (isWindowOpen(2))
    {
        //LoadJournal
    }
    //Прогрузка окна обмена предметами с сущностью (сундук, карманы противника??)
    if (isWindowOpen(5))
    {
        LoadPickupMenu(event, window, player);
    }

    if (isWindowOpen(6))
    {
        LoadAdminPanel(event, window);
    }

    if (isWindowOpen(7))
    {
        LoadTradeMenu(event, window, player);
    }

}

void UIManager::LoadPickupMenu(sf::Event& event, sf::RenderWindow& window, Character& player)
{
    std::unordered_multiset<Object*> AllObject = Object::getAllObjects();
    std::multiset<int> inventory;
    Storage* chest_stor;
   

    //Поиск Storage с которым идет обмен и его присваивание Storage (да-да опять динамическое кастование)
    if(!isWindowOpen(7)){
    std::unordered_set<MapObject*> AllMapObj = MapObject::getAllMapObjects();
    for (auto const& mapObj : AllMapObj){
        if(mapObj->getMasterID() == CommutatorID){
            chest_stor = dynamic_cast<Storage*>(mapObj);
            inventory = chest_stor->GetInventory();
        }
    }
    }
    else
    {
        UIManager* ui_manager = UIManager::getController();
        int SellerId =  ui_manager->getSellerId();
        std::unordered_map<unsigned int, Character*> AllChar = Character::getAllChar();
        auto it = AllChar.find(SellerId);
        Character* seller = it->second;
        inventory = seller->GetInventory();

    }
    sf::Vector2u windowSize = window.getSize();
    const float padding = 5.0f;
    const float boxPadding = 0.0f;

    //Просто характеристики окна со стороны дизайна (цвета, размеры и тд и тп)
    sf::RectangleShape inventoryBox;
    inventoryBox.setSize(sf::Vector2f(ICON_SIZE * INVENTORY_COLS + boxPadding + 35, ICON_SIZE * INVENTORY_ROWS + boxPadding + 45));
    inventoryBox.setFillColor(sf::Color(186, 132, 55));
    inventoryBox.setOutlineThickness(WINDOW_WIDTH * 0.01f);
    inventoryBox.setOutlineColor(sf::Color(88, 53, 3));
    inventoryBox.setPosition((windowSize.x - inventoryBox.getSize().x) / 2 - 100, (windowSize.y - inventoryBox.getSize().y) / 2);
    window.draw(inventoryBox);
    _inventoryBox = inventoryBox;

    sf::Sprite iconSprite;
    int iconCount = 0;
    static bool MouseCount = true;

    sf::Vector2f mousePos = GameCamera::getMapMousePos();


    for (int row = 0; row < INVENTORY_ROWS; ++row) {
        for (int col = 0; col < INVENTORY_COLS; ++col) {
            
            //Получение позиций для отрисовки иконок и их отрисовка (слоты фактически рисуются с нужными падингами)
            sf::RectangleShape slot(sf::Vector2f(ICON_SIZE, ICON_SIZE));
            float xPos = inventoryBox.getPosition().x + padding + col * (ICON_SIZE + padding);
            float yPos = inventoryBox.getPosition().y + padding + row * (ICON_SIZE + padding);
            slot.setPosition(xPos, yPos);
            slot.setFillColor(sf::Color(0, 143, 191, 87));
            slot.setOutlineThickness(WINDOW_WIDTH * 0.003f);
            slot.setOutlineColor(sf::Color(88, 53, 3));
            window.draw(slot);

            //Прорисовка элементов из инвентаря
            if (iconCount < inventory.size()) {
                // Получение ID предмета из инвентаря
                auto itemIter = std::next(inventory.begin(), iconCount);
                int itemID = *itemIter;

                //проходка по всем предметам и расстановка в нужные слоты 
                for (const auto& obj : AllObject) {
                    if (obj->getUniqueId() == itemID) {

                        obj->setPosition(slot.getPosition());
                        iconSprite = obj->getSprite();
                        if (iconSprite.getTexture() != nullptr) {
                            iconSprite.setPosition(slot.getPosition());
                            iconSprite.setScale(3, 3);
                            window.draw(iconSprite);

                            //Если нажата левая кнопка мыши по прдемета => используем его
                            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                                if (slot.getGlobalBounds().contains(mousePos) && MouseCount) {
                                    MouseCount = false;
                                    if (!isWindowOpen(7) && isWindowOpen(5)) {
                                        obj->Give(player);
                                    }
                                    else if (isWindowOpen(7))
                                    {
                                        obj->Sell(player);
                                    }
                                    else
                                    {
                                        obj->Give(player);
                                    }
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
    //эта штука нужна для "избежания" одновременного "многоразового" нажатия
    if (event.type == sf::Event::MouseMoved) {
        MouseCount = true;
    }
}

void UIManager::LoadAdminPanel(sf::Event& event, sf::RenderWindow& window)
{
    //коэффициенты для корректного отображения
    float coef_x = window.getSize().x / WINDOW_WIDTH;
    float coef_y = window.getSize().y / WINDOW_HEIGHT;

    sf::RectangleShape adminPanelbox;
    sf::RectangleShape textPanelbox;
    sf::RectangleShape deleteString;

    //Создаем 3 панели (сама черная панель где будем писать команды, кнопка для удаления написаного и блок с текстом)
    adminPanelbox.setFillColor(sf::Color(0, 0, 0));
    adminPanelbox.setOutlineThickness(WINDOW_HEIGHT * coef_y * 0.005f);
    adminPanelbox.setOutlineColor(sf::Color(88, 53, 3));

    textPanelbox.setFillColor(sf::Color(0, 0, 0));
    deleteString.setFillColor(sf::Color::Red);

    deleteString.setSize(sf::Vector2f(window.getSize().x * 0.3, window.getSize().y * 0.05));
    adminPanelbox.setSize(sf::Vector2f(window.getSize().x, window.getSize().y * 0.25f));
    textPanelbox.setSize(sf::Vector2f(window.getSize().x, window.getSize().y * 0.08f)); 

    deleteString.setPosition(0, 0);
    adminPanelbox.setPosition(0, 0);
    textPanelbox.setPosition(0, adminPanelbox.getGlobalBounds().height - 2*textPanelbox.getGlobalBounds().height);

    //Текст и добавление ему свойств
    sf::Font font = outdata::mainFont;
    std::string temp;
    sf::Text text;
    text.setFont(font);
    if(text.getString().isEmpty()){
        text.setString("Enter your command:");
    }
    text.setCharacterSize(24);
    text.setScale(coef_x, coef_y);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, adminPanelbox.getGlobalBounds().height - 2*textPanelbox.getGlobalBounds().height);

    static std::string inputText = "";
    static bool keyPressed = false;

    //По нажатию на красную кнопку удаляет текст
    if (event.type == sf::Event::MouseButtonReleased)
    {
       sf::Vector2f mousePos = (sf::Vector2f)Mouse::getPosition();
       if (deleteString.getGlobalBounds().contains(mousePos));
       {
           inputText = "";
       }
    }

    //Обработка текста введенного пользователем
    if (event.type == sf::Event::TextEntered) {
        if (!keyPressed) {
            if (event.text.unicode < 128) {  
                if (event.text.unicode == 8 && inputText.length() > 0) { 
                    inputText.pop_back();
                }
                else if (event.text.unicode == 13)
                {

                    inputText = AdminPanel::AdminProcessPanel(inputText);
                }
                else {
                    inputText += static_cast<char>(event.text.unicode);
                }
            }
            keyPressed = true;  
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        keyPressed = false;
    }

    if (inputText.empty()) {
        text.setString("Enter your command:");
    }
    else {
        text.setString(inputText);
    }



    
    window.draw(adminPanelbox);
    window.draw(textPanelbox);
    window.draw(deleteString);
    window.draw(text);
}



void UIManager::LoadText(const string& text, sf::RenderWindow& window)
{
    //Коэфы маштабирования
    float coef_x = window.getSize().x / WINDOW_WIDTH;
    float coef_y = window.getSize().y / WINDOW_HEIGHT;

    //загрузка шрифта, конвертация заданного текста в wchar_T для отображения русского языка
    sf::Font font = outdata::mainFont;
    sf::Text uiText;
    sf::String WcharText;
    uiText.setFont(font);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    WcharText = converter.from_bytes(text);
    uiText.setString(WcharText);
    uiText.setCharacterSize(22);
    uiText.setFillColor(sf::Color(240, 255, 255));

    //Создание текстового окошка и его раскраска
    sf::FloatRect textBounds = uiText.getGlobalBounds();
    float screenWidth = window.getSize().x;
    float screenHeight = window.getSize().y;
    sf::RectangleShape textBox(sf::Vector2f(screenWidth * 0.98f, screenHeight * 0.2f));  
    textBox.setFillColor(sf::Color(186, 132, 55)); 
    textBox.setOutlineThickness(screenHeight * 0.01f);
    textBox.setOutlineColor(sf::Color(88, 53, 3)); 

    //Расположение текста и окошек
    textBox.setPosition(screenWidth * 0.01f, screenHeight - textBox.getSize().y);
    uiText.setPosition(textBox.getPosition().x + screenWidth * 0.005f * coef_x,
    textBox.getPosition().y - screenHeight * 0.006f * coef_y);

    //Маштабирование "Шрифта"
    uiText.setScale(coef_x, coef_y);

    //Рендеринггг))
    window.draw(textBox);
    window.draw(uiText);
}


void UIManager::DrawInventorySlots(sf::RenderWindow& window, const sf::RectangleShape& box,
    const std::multiset<int>& inventory,
    const std::unordered_multiset<Object*>& allObjects) {
    int iconCount = 0;
    const float padding = 5.0f;
    for (int row = 0; row < INVENTORY_ROWS; ++row) {
        for (int col = 0; col < INVENTORY_COLS; ++col) {
            if (iconCount >= inventory.size()) return;

            sf::RectangleShape slot(sf::Vector2f(ICON_SIZE, ICON_SIZE));
            float xPos = box.getPosition().x + padding + col * (ICON_SIZE + padding);
            float yPos = box.getPosition().y + padding + row * (ICON_SIZE + padding);
            slot.setPosition(xPos, yPos);
            slot.setFillColor(sf::Color(0, 143, 191, 87));
            slot.setOutlineThickness(1.0f); // Example
            slot.setOutlineColor(sf::Color(88, 53, 3));
            window.draw(slot);

            auto itemIter = std::next(inventory.begin(), iconCount);
            int itemID = *itemIter;
            for (const auto& obj : allObjects) {
                if (obj->getUniqueId() == itemID) {
                    sf::Sprite iconSprite = obj->getSprite();
                    if (iconSprite.getTexture() != nullptr) {
                        iconSprite.setPosition(slot.getPosition());
                        iconSprite.setScale(2.0f, 2.0f);
                        window.draw(iconSprite);
                    }
                    break;
                }
            }
            ++iconCount;
        }
    }
}



void UIManager::LoadTradeMenu(sf::Event& event, sf::RenderWindow& window, Character& Player)
{
    std::unordered_multiset<Object*> AllObject = Object::getAllObjects();
    sf::Vector2u windowSize = _windowToDisplay->getSize();
    const float padding = 5.0f;
    std::unordered_map<unsigned int, Character*> AllChar = Character::getAllChar();
    auto it = AllChar.find(CharSellerID);
    Character* seller = it->second;


    // Общий бокс для окна торговли
    sf::RectangleShape tradeBox;
    tradeBox.setSize(sf::Vector2f(WINDOW_WIDTH * 0.2f, WINDOW_HEIGHT * 0.6f));
    tradeBox.setFillColor(sf::Color(186, 132, 55));
    tradeBox.setOutlineThickness(WINDOW_WIDTH * 0.01f);
    tradeBox.setOutlineColor(sf::Color(88, 53, 3));
    tradeBox.setPosition((windowSize.x - tradeBox.getSize().x) / 2 + 150, (windowSize.y - tradeBox.getSize().y) / 2);
    _windowToDisplay->draw(tradeBox);

    // Боксы для временных инвентарей
    sf::RectangleShape playerBox, merchantBox;
    playerBox.setSize(sf::Vector2f(tradeBox.getSize().x / 2 - padding * 2, tradeBox.getSize().y - padding * 2));
    playerBox.setPosition(tradeBox.getPosition().x + padding, tradeBox.getPosition().y + padding);
    playerBox.setFillColor(sf::Color(0, 143, 191, 87));
    playerBox.setOutlineThickness(WINDOW_WIDTH * 0.003f);
    playerBox.setOutlineColor(sf::Color(88, 53, 3));
    _windowToDisplay->draw(playerBox);

    merchantBox = playerBox;
    merchantBox.setPosition(playerBox.getPosition().x + playerBox.getSize().x + padding, playerBox.getPosition().y);
    _windowToDisplay->draw(merchantBox);

    // Слоты для временных инвентарей
    auto drawInventory = [&](std::multiset<int>& tempInventory, sf::RectangleShape& box) {
        int iconCount = 0;
        float iconStartX = box.getPosition().x + padding;
        float iconStartY = box.getPosition().y + padding;

        for (int row = 0; row < INVENTORY_ROWS; ++row) {
            for (int col = 0; col < INVENTORY_COLS; ++col) {
                if (iconCount >= tempInventory.size()) return;

                sf::RectangleShape slot(sf::Vector2f(ICON_SIZE, ICON_SIZE));
                float xPos = iconStartX + col * (ICON_SIZE + padding);
                float yPos = iconStartY + row * (ICON_SIZE + padding);
                slot.setPosition(xPos, yPos);
                slot.setFillColor(sf::Color(88, 53, 3));
                slot.setOutlineThickness(WINDOW_WIDTH * 0.003f);
                slot.setOutlineColor(sf::Color(186, 132, 55));
                _windowToDisplay->draw(slot);

                auto itemIter = std::next(tempInventory.begin(), iconCount);
                int itemID = *itemIter;

                // Найти объект и отрисовать
                for (const auto& obj : AllObject) {
                    if (obj->getUniqueId() == itemID) {
                        sf::Sprite iconSprite = obj->getSprite();
                        if (iconSprite.getTexture() != nullptr) {
                            iconSprite.setPosition(slot.getPosition());
                            iconSprite.setScale(3.0f, 3.0f);
                            _windowToDisplay->draw(iconSprite);

                            // ЛКМ - переместить в другой инвентарь
                            sf::Vector2f mousePos = GameCamera::getMapMousePos();
                            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left &&
                                slot.getGlobalBounds().contains(mousePos) && playerTempInventory.find(obj->getUniqueId()) == playerTempInventory.end() 
                                && merchantTempInventory.find(obj->getUniqueId()) == merchantTempInventory.end()) {

                                obj->Sell(Player);
                            }
                        }
                        break;
                    }
                }
                ++iconCount;
            }
        }
        };

    sf::Text playerMoneyText("Player Money: " + std::to_string(tempPlayerMoney), outdata::mainFont, 14);
    playerMoneyText.setPosition(playerBox.getPosition().x, playerBox.getPosition().y - 20);
    window.draw(playerMoneyText);

    sf::Text traderMoneyText("Trader Money: " + std::to_string(tempMerchantMoney), outdata::mainFont, 14);
    traderMoneyText.setPosition(merchantBox.getPosition().x, merchantBox.getPosition().y - 20);
    window.draw(traderMoneyText);


    // Рисуем оба инвентаря
    drawInventory(this->playerTempInventory, playerBox);
    drawInventory(this->merchantTempInventory, merchantBox);

    // Обработка кнопок "Принять" и "Отменить"
    sf::RectangleShape acceptButton, cancelButton;
    acceptButton.setSize(sf::Vector2f(100, 50));
    acceptButton.setPosition(tradeBox.getPosition().x + tradeBox.getSize().x - 50, tradeBox.getPosition().y + tradeBox.getSize().y - 70);
    acceptButton.setFillColor(sf::Color(0, 255, 0));
    _windowToDisplay->draw(acceptButton);

    cancelButton = acceptButton;
    cancelButton.setPosition(acceptButton.getPosition().x - 50, acceptButton.getPosition().y);
    cancelButton.setFillColor(sf::Color(255, 0, 0));
    _windowToDisplay->draw(cancelButton);

    // Логика кнопок
    sf::Vector2f mousePos = GameCamera::getMapMousePos();
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (acceptButton.getGlobalBounds().contains(mousePos)) {
            // Принять сделку
            if (Player.getMoney() - tempMerchantMoney < 0)// добавим мерчанта попозже
            {
                sf::Text traderMoneyText("NO MOMNY", outdata::mainFont, 50);
                traderMoneyText.setPosition(merchantBox.getPosition().x, merchantBox.getPosition().y - 20);
                window.draw(traderMoneyText);
            }
            else
            Object::AcceptTrade(Player);
            this->merchantTempInventory.clear();
                this->playerTempInventory.clear();
        }
        if (cancelButton.getGlobalBounds().contains(mousePos)) {
            // Отменить сделку
            tempMerchantMoney = 0;
            tempPlayerMoney = 0;

            for (auto item1 : playerTempInventory){
            Player.insertInInventory(item1);
            }

            for (auto item1 : playerTempInventory) {
                Player.insertInInventory(item1);
            }

            for (auto item1 : merchantTempInventory) {
                seller->insertInInventory(item1);
            }
            

            this->merchantTempInventory.clear();
                this->playerTempInventory.clear();
        }
    }

}

void UIManager::OpenTraideMenu(int CharacterId)
{
    this->CharSellerID = CharacterId;
    this->togleWindow(7);
    this->togleWindow(1);
    this->togleWindow(5);
}

void UIManager::ChangeText(const string& text)
{
    //Изменение отображаемого текста (DIalogs??? , предположительно, если будешь использовать такую систему для диалогов
    //то вместо стринги сверху (const string text, в функции LOAD TEXT) будет использоваться стринга в UIManager, для общего упрощения
    textToDisplay = text;
}

void UIManager::LoadInventory(Character& player, std::unordered_multiset<Object*> AllObject, sf::Event& event)
{
    std::multiset<int> inventory = player.GetInventory();
    sf::Vector2u windowSize = _windowToDisplay->getSize();

    const float padding = 5.0f;
    const float boxPadding = 0.0f;

    // Характеристики со стороны дизайна (цвет, позиция, размеры и тд и тп)
    sf::RectangleShape inventoryBox;
    float expandedWidth = ICON_SIZE * INVENTORY_COLS * 2.5f + boxPadding + 35;
    inventoryBox.setSize(sf::Vector2f(expandedWidth, ICON_SIZE * INVENTORY_ROWS + boxPadding + 45));
    inventoryBox.setFillColor(sf::Color(186, 132, 55));
    inventoryBox.setOutlineThickness(WINDOW_WIDTH * 0.01f);
    inventoryBox.setOutlineColor(sf::Color(88, 53, 3));
    inventoryBox.setPosition((windowSize.x - inventoryBox.getSize().x) / 2 + 200, (windowSize.y - inventoryBox.getSize().y) / 2);
    _inventoryBox = inventoryBox;
    _windowToDisplay->draw(_inventoryBox);

    //Часть в разработке - расположение в будущем иконо со слотами для брони/оружия
    /*std::vector<sf::Vector2f> slotPositions;
    float leftX = inventoryBox.getPosition().x*1.3f; 
    float extraVerticalSpacing = 20.0f; 
    slotPositions.push_back({ leftX, inventoryBox.getPosition().y + padding + 0 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Head
    slotPositions.push_back({ leftX, inventoryBox.getPosition().y + padding + 1 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Cloak
    slotPositions.push_back({ leftX, inventoryBox.getPosition().y + padding + 2 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Body Armor
    slotPositions.push_back({ leftX, inventoryBox.getPosition().y + padding + 3 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Gloves
    slotPositions.push_back({ leftX, inventoryBox.getPosition().y + padding + 4 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Legs
    slotPositions.push_back({ leftX, inventoryBox.getPosition().y + padding + 5 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Pants
    slotPositions.push_back({ leftX + ICON_SIZE + padding, inventoryBox.getPosition().y + padding + 0 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Ring 1
    slotPositions.push_back({ leftX + ICON_SIZE + padding, inventoryBox.getPosition().y + padding + 1 * (ICON_SIZE + padding + extraVerticalSpacing) }); // Ring 2


    // Тут якобы оружия будут
    for (int i = 0; i < 4; ++i) {
        slotPositions.push_back({ leftX - 0.2f* inventoryBox.getPosition().x + i*3*extraVerticalSpacing, inventoryBox.getPosition().y + padding + 5 * (ICON_SIZE + padding + extraVerticalSpacing) });
    }

    // Отрисовка слотов экипировки
    for (const auto& pos : slotPositions) {
        sf::RectangleShape slot(sf::Vector2f(ICON_SIZE, ICON_SIZE));
        slot.setPosition(pos);
        slot.setFillColor(sf::Color(0, 143, 191, 87));
        slot.setOutlineThickness(WINDOW_WIDTH * 0.003f);
        slot.setOutlineColor(sf::Color(88, 53, 3));
        _windowToDisplay->draw(slot);
    }
    */

    // Отрисовка слотов инвентаря
    float iconStartX = inventoryBox.getPosition().x + expandedWidth - (ICON_SIZE * INVENTORY_COLS) - padding;
    sf::Vector2f mousePos = GameCamera::getMapMousePos();
    sf::Sprite iconSprite;
    int iconCount = 0;
    static bool MouseCount = true;

    for (int row = 0; row < INVENTORY_ROWS; ++row)
    {
        for (int col = 0; col < INVENTORY_COLS; ++col)
        {
            //Сторона дизайна, функционала не несет
            sf::RectangleShape slot(sf::Vector2f(ICON_SIZE, ICON_SIZE));
            float xPos = iconStartX + col * (ICON_SIZE + padding) - (WINDOW_WIDTH * 0.017f);
            float yPos = inventoryBox.getPosition().y + padding + row * (ICON_SIZE + padding);
            slot.setPosition(xPos, yPos);
            slot.setFillColor(sf::Color(0, 143, 191, 87));
            slot.setOutlineThickness(WINDOW_WIDTH * 0.003f);
            slot.setOutlineColor(sf::Color(88, 53, 3));
            _windowToDisplay->draw(slot);

            if (iconCount < inventory.size())
            {
                //Проходка по всем предметам в инвенатаре
                auto itemIter = std::next(inventory.begin(), iconCount);
                int itemID = *itemIter;

                //Проходка по объектам и их отрисовка + скалирование (setScale) + размещение
                for (const auto& obj : AllObject)
                {
                    if (obj->getUniqueId() == itemID)
                    {
                        obj->setPosition(slot.getPosition());

                        iconSprite = obj->getSprite();
                        if (iconSprite.getTexture() != nullptr)
                        {
                            iconSprite.setPosition(slot.getPosition());
                            iconSprite.setScale(3, 3);
                            _windowToDisplay->draw(iconSprite);

                            //Использование/передача по нажатию ЛКМ
                            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                            {
                                if (slot.getGlobalBounds().contains(mousePos) && MouseCount)
                                {
                                    MouseCount = false;
                                    if (!isWindowOpen(7) && isWindowOpen(5)) {
                                        obj->Give(player);
                                    }
                                    else if (isWindowOpen(7))
                                    {
                                        obj->Sell(player);
                                    }
                                    else
                                    {
                                        obj->Use(player);
                                    }
                                }
                            }

                            //Выбрасывание по нажатию ПКМ
                            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
                            {
                                if (slot.getGlobalBounds().contains(mousePos) && MouseCount)
                                {
                                    MouseCount = false;

                                    player.RemoveFromInventory(itemID);

                                    //Новые координаты предмета рядом с персонажем (после выброса по сути)
                                    sf::Vector2f newPosition = player.getPosition();
                                    newPosition.x += player.getSprite().getGlobalBounds().width + 10;

                                    obj->setPosition(newPosition);
                                    obj->setMasterID(-1);
                                    obj->togleIsInventory();
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

    //Для обеспечения однократного нажатия
    if (event.type == sf::Event::MouseMoved) {
        MouseCount = true;
    }
}



UISlot::UISlot(int id, sf::Sprite sprite, float x, float y, sf::Keyboard::Key key)
    : _slotID(id), _actionID(-1), _assignedKey(key), _sprite(sprite)
{
    _sprite.setPosition(x, y);
}

bool UISlot::isKeyAssigned() const {
    return _assignedKey != sf::Keyboard::Unknown;
}

bool UISlot::isClicked(const sf::Vector2i& mousePos) const
{
    return _sprite.getGlobalBounds().contains((sf::Vector2f)mousePos);
}

bool UISlot::checkHotKey(sf::Keyboard::Key inpKey)
{
    return inpKey == _assignedKey;
}

void UISlot::setActionID(int ID)
{
    _actionID = ID;
}

void UISlot::setSprite(sf::Texture texture)
{
    _sprite.setTexture(texture);
}

void UISlot::setAssignedKey(sf::Keyboard::Key key)
{
    _assignedKey = key;
}

sf::Sprite UISlot::getSprite()
{
    return _sprite;
}
 

void UIManager::Listen(sf::Event event, Character& player, sf::RenderWindow& window)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Tilde)
        {
            togleWindow(6);
        }
        else if(!isWindowOpen(6)){
        if (event.key.code == sf::Keyboard::I)
        {
            _UIController->togleWindow(1);
        }
        else if (event.key.code == sf::Keyboard::Escape && _UIController->isWindowOpen(4))
        {
            _UIController->disableWindow(4);
        }
        else if (event.key.code == sf::Keyboard::Escape && _UIController->isWindowOpen(5))
        {
            _UIController->disableWindow(5);
            _UIController->disableWindow(1);
        }
        else if (event.key.code == sf::Keyboard::Escape)
        {
            _UIController->togleWindow(0);
        }
        else
        {
            for (auto& slot : _bottomPanel)
            {
                if (slot.isKeyAssigned() && slot.checkHotKey(event.key.code))
                {
                    // slot.useItem();
                }
            }
        }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        if (_vectorWithIcons[1].getGlobalBounds().contains(mousePos) && !isWindowOpen(0)) {
            togleWindow(1);
        }
        else if (_vectorWithIcons[0].getGlobalBounds().contains(mousePos))
        {
            togleWindow(0);
        }
    }
    //if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
    //    isMouseReleased = true;
    //}
}

void UIManager::setWindowToDisplay(sf::RenderWindow* window)
{
    _windowToDisplay = window;
}

sf::RenderWindow* UIManager::getWindow()
{
    return _windowToDisplay;
}

int UIManager::getCommutatorID()
{
    return CommutatorID;
}

void UIManager::setCommutatorID(int commutatorID)
{
    CommutatorID = commutatorID;
}

void UIManager::insertItemToPlayerTemp(int id)
{
    playerTempInventory.insert(id);
}

void UIManager::insertItemToMechantTemp(int id)
{
    merchantTempInventory.insert(id);
}

void UIManager::DeleteteItemInMerchantTemp(int id)
{
    auto it = merchantTempInventory.find(id);

    if (it != merchantTempInventory.end()) {
        merchantTempInventory.erase(it);
    }
}

void UIManager::DeleteteItemToPlayerTemp(int id)
{
    auto it = playerTempInventory.find(id);

    if (it != playerTempInventory.end()) {
        playerTempInventory.erase(it);
    }
}

void UIManager::addMoney_TempPlayer(int money)
{
    tempPlayerMoney += money;
}

void UIManager::loseMoney_TempPlayer(int money)
{
    tempPlayerMoney -= money;
}

int UIManager::getTempPlayerMoney()
{
    return tempPlayerMoney;
}

int UIManager::getTempMerchantMoney()
{
    return tempMerchantMoney;
}

std::multiset<int> UIManager::GetplayerTempInventory()
{
    return playerTempInventory;
}

std::multiset<int> UIManager::GetmerchantTempInventory()
{
    return merchantTempInventory;
}

// Проверяет наличие элемента в playerTempInventory

 bool UIManager::findInPlayerInventory(int value) {
    return playerTempInventory.find(value) != playerTempInventory.end();
}


// Проверяет наличие элемента в merchantTempInventory

 bool UIManager::findInMerchantInventory(int value) {
    return merchantTempInventory.find(value) != merchantTempInventory.end();
}

void UIManager::addMoney_TempMerchant(int money)
{
    tempMerchantMoney += money;
}

void UIManager::loseMoney_TempMerchant(int money)
{
    tempMerchantMoney -= money;
}

int UIManager::getSellerId()
{
    return CharSellerID;
}

void UIManager::setSellerId(int commutatorID)
{
    CharSellerID = commutatorID;
}
