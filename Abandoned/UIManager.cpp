#include "UIManager.hpp"
#include "Outdata.hpp"
#include "Outdata.hpp"
#include <locale>
#include <codecvt>

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
    unordered_set<Object*> AllObjects = Object::getAllObjects();
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
}

void UIManager::LoadPickupMenu(sf::Event& event, sf::RenderWindow& window, Character& player)
{
    std::unordered_set<Object*> AllObject = Object::getAllObjects();
    std::multiset<int> inventory;
    Storage* chest_stor;

    //Поиск Storage с которым идет обмен и его присваивание Storage (да-да опять динамическое кастование)
    std::unordered_set<MapObject*> AllMapObj = MapObject::getAllMapObjects();
    for (auto const& mapObj : AllMapObj){
        if(mapObj->getMasterID() == CommutatorID){
            chest_stor = dynamic_cast<Storage*>(mapObj);
            inventory = chest_stor->GetInventory();
        }
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

    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));


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
                    if (obj->getItemId() == itemID) {

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
                                    obj->Use(player);
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

void UIManager::ChangeText(const string& text)
{
    //Изменение отображаемого текста (DIalogs??? , предположительно, если будешь использовать такую систему для диалогов
    //то вместо стринги сверху (const string text, в функции LOAD TEXT) будет использоваться стринга в UIManager, для общего упрощения
    textToDisplay = text;
}

void UIManager::LoadInventory(Character& player, std::unordered_set<Object*> AllObject, sf::Event& event)
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
    std::vector<sf::Vector2f> slotPositions;
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

    // Отрисовка слотов инвентаря
    float iconStartX = inventoryBox.getPosition().x + expandedWidth - (ICON_SIZE * INVENTORY_COLS) - padding;
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_windowToDisplay));
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
                    if (obj->getItemId() == itemID)
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
                                    obj->Use(player);
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
 

void UIManager::Listen(sf::Event event, Player& player, sf::RenderWindow& window)
{
    //Листенер, не знаю, что описывать, по необходимым кнопкам делает соответственные реакции (код очевидно проглядывается)
    if (event.type == sf::Event::KeyPressed) 
    {
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
