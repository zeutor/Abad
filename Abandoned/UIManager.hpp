#pragma once
#include "SFML/Graphics.hpp"
#include "Constants.hpp"
#include "AdminPanel.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include "MapObject.hpp"
#include "Object.hpp"

class Object;

using namespace std;

class UISlot {
private:
    int _slotID;
    // ID of thing that will done.
    int _actionID;

    sf::Sprite _sprite;

    sf::Keyboard::Key _assignedKey;
public:
    UISlot(int id, sf::Sprite sprite, float x, float y, sf::Keyboard::Key key = sf::Keyboard::Unknown);

    // #######################################################
    // ################### CHECKERS ZONE #####################
    // #######################################################

    bool isKeyAssigned() const;

    bool isClicked(const sf::Vector2i& mousePos) const;
    bool checkHotKey(sf::Keyboard::Key);

    // #######################################################
    // ################## SET/GET ZONE #######################
    // #######################################################

    void setActionID(int ID);
    void setSprite(sf::Texture);
    void setAssignedKey(sf::Keyboard::Key);
    sf::Sprite getSprite();
};

//, std::unordered_map<int, Item>& items

class UIManager
{
private:
	UIManager() = default;
	static UIManager* _UIController;
    std::string textToDisplay;
    std::vector<UISlot> _bottomPanel;

    // Storage icons of meny, inventory and etc.
    std::vector<sf::Sprite> _vectorWithIcons;

    sf::RectangleShape _inventoryBox;
    sf::RectangleShape _menuBox;
    sf::RectangleShape _dialogueBox;
    //Id предмета с которым сейчас идет обмен предметами (карманы противника, сундук и тд)
    int CommutatorID;
    static sf::RenderWindow* _windowToDisplay;

    // 0 - menu; 1 - inventory; 2 - journal; 3 - settings; 4 - dialogs/table_text; 5 - chestWindow; 6 - adminPanel =)
    // chestWIndow эта короче грудь сундук (то есть будет открываться при подъеме предметов с грудей пероснажей или открытии сундуков/мб воровства???)
    bool _isUIWindowOpen[7];
    const int _countOfWindows = 7;

    bool _isAnyWindowOpen;
public:

    // #######################################################
    // ################# CONSTRUCORS ZONE ####################
    // #######################################################

    UIManager(UIManager const&) = delete;
    void operator= (UIManager const&) = delete;

    // Singelton getter
    static UIManager* getController();

    // #######################################################
    // ################### CHECKERS ZONE #####################
    // #######################################################

    // 0 - menu; 1 - inventory; 2 - journal; 3 - settings; 4 - dialogs/table_text; 5 - chestWindow; 6 - adminPanel =)
    bool isWindowOpen(int winID);

    // 0 - menu; 1 - inventory; 2 - journal; 3 - settings; 4 - dialogs/table_text; 5 - chestWindow; 6 - adminPanel =)
    void togleWindow(int winID);

    //Off window;
    void disableWindow(int winID);

    // Is clicked on bottom-slot UI
    bool handleSlotClick(sf::Vector2i& mousePos);

    // Is clicked on any element of UI
    bool IsOnUIClicked(sf::Vector2i& mousePos);

    bool isAnyWindowOpen();

   
    // #######################################################
    // ################### LOADERS ZONE ######################
    // #######################################################

    //Загрузка инвентаря с некоторыми иконками для предметов //TODO
    void LoadInventory(Character& player, std::unordered_multiset<Object*> AllObject, sf::Event& event);

    // Load vector of top right icons of navigation
    std::vector<sf::Sprite>LoadIcons(std::vector<sf::Texture>, sf::RenderWindow& window);

    //Загрузка меню игры (основные кнопки)
    void LoadMenu(sf::Event& event, sf::RenderWindow& window);

    //Загрузка настроек игры //TODO 
    bool LoadSettings(sf::RenderWindow& window, sf::Event& event); 

    //Загрузка UI игры (в том числе вызов всех LOAD функций)
    void LoadGameUI( sf::Event event, Character& player, sf::RenderWindow& window = *_windowToDisplay);

    //Загрузка окна с предметами у Storage/во время подбора вещей с противника
    void LoadPickupMenu(sf::Event& event, sf::RenderWindow& window, Character& player);

    //Загрузка панели админа для дебага внутриигрового
    void LoadAdminPanel(sf::Event& event, sf::RenderWindow& window);

    //Загрузка текста для табличек и диалогов??
    void LoadText(const string& text, sf::RenderWindow& window = *_windowToDisplay);

    //Функция изменяющаяя текст в классе UIManager (для вывода на экран)
    void ChangeText(const string& text);


    // #######################################################
    // #################### OTHERS ZONE ######################
    // #######################################################

    //Listener - фактически прослушка нажатия кнопок, так называемые "Бинды"
    void Listen(sf::Event event, Character& player, sf::RenderWindow& window);

   //Геттеры сеттеры для игрового окна, а также CommutatorID (для обмена вещами)
    static void setWindowToDisplay(sf::RenderWindow* window);
    sf::RenderWindow* getWindow();
    int getCommutatorID();
    void setCommutatorID(int commutatorID);

    // Returning vector of the bottom panel
    std::vector<UISlot> getInvConroller();
    

    void Update();
};




