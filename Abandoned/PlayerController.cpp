﻿#include "PlayerController.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "SFML/Graphics.hpp"
#include "Application.hpp"
#include "Character.hpp"
#include <vector>
#include "GameCamera.hpp"
#include "UIManager.hpp"
#include "Object.hpp"
#include "tools.hpp"

using namespace sf;
using namespace std;

PlayerController* PlayerController::_controller = nullptr;

static unsigned int frameCounter;

PlayerController::~PlayerController() {
    delete _controller;
} 

PlayerController* PlayerController::getController() {
    if (!_controller)
        _controller = new PlayerController();

    return _controller;
}


vector<AStar::sNode*> path;
size_t currentTargetIndex = 0;

void PlayerController::controllPlayer(Character& player, float time, sf::RenderWindow* window) {
    static float totalDistance = 0; // Для хранения пройденного расстояния
    static bool isMouseHeld = false; // Флаг для отслеживания состояния мыши
    sf::Vector2f mousePosition = GameCamera::getMapMousePos();
   

    UIManager* UIController = UIManager::getController();
    std::vector<UISlot> Slots = UIController->getInvConroller();
    Slots[9].setActionID(2); //вектор с UI элементами, ниже HandleClick - для проверки клика на UI Элементы

    std::unordered_multiset<Object*> AllObject = Object::getAllObjects();
    std::unordered_set<MapObject*> AllMapObj = MapObject::getAllMapObjects();

    MapController* mapController = MapController::getController();
    sf::Vector2i mousepostrue(mousePosition.x, mousePosition.y);

    // Просто ходьба, если не разговаривает.
    if (!player._isInDialog)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            MouseTake(player, AllObject, (sf::Vector2f)mousepostrue);
        }

        // Если нажата левая кнопка мыши (однократное нажатие)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseHeld) {
            isMouseHeld = true;
            frameCounter = 0;
            // Получаем координаты мыши
            
            sf::Vector2f mousePos = GameCamera::getMapMousePos();
            ActivateMapObj(player, (sf::Vector2f)mousepostrue, AllMapObj);
            // Ref of character on mouse pos (without player).
            unsigned int chIDByWhomClicked = Character::getCharacterIDByPoint(mousePos, new Character*(&player), 1);

            // If enough distance, clicked on person and clicked not by player.
            if (tools::distance(player._position, mousePos) < INTERACT_DISTANSE && chIDByWhomClicked && player._ID != chIDByWhomClicked)
            {
                player.openCommunicationWindow(chIDByWhomClicked);
                player._isInDialog = true;
            }
            else if (!mapController->checkCollision(-1, (sf::Vector2f)mousePos))
            {
                // Устанавливаем новую цель для поиска пути
                player._astar.setEnd(mousePos.x / PIXELS_FOR_OBSTACLE, mousePos.y / PIXELS_FOR_OBSTACLE);
                player._astar.setStart(player.getPosition().x / PIXELS_FOR_OBSTACLE, player.getPosition().y / PIXELS_FOR_OBSTACLE);
                player._astar.Solve_AStar();
                path = player._astar.getPath();
                currentTargetIndex = 0;
            }
        }

        // Херабора считает количетсво кадров для измерения времени. Если какое-то время мышь зажата, то перемещение к мыши, а не по пути
        if (frameCounter < FRAME_LIMIT)
            ++frameCounter;


        // Если мышь отпущена, сбрасываем флаг
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isMouseHeld = false;
        }

        // Перемещение персонажа по пути
        if (!path.empty() && currentTargetIndex < path.size()) {

            sf::Vector2f targetPosition = sf::Vector2f(path[currentTargetIndex]->x * PIXELS_FOR_OBSTACLE, path[currentTargetIndex]->y * PIXELS_FOR_OBSTACLE);
            player.moveTo(targetPosition, time);

            if (sqrt(pow(player.getPosition().x - targetPosition.x, 2) +
                pow(player.getPosition().y - targetPosition.y, 2)) < POSITION_EPSILON) {
                currentTargetIndex++;
            }
        }

        // Если мышь зажата, обновляем путь каждую итерацию, чтобы двигаться за мышкой, если пришло время.
        if (isMouseHeld && frameCounter > FRAME_LIMIT / 6) {
            path = vector<AStar::sNode*>();
            player.moveTo(mousePosition, time);
            // Обновляем конечную позицию на текущую позицию мыши
        }
    }
    else {
        if(keyCode != 0)
        if (keyCode >= 27 && keyCode <= 35)
            player._communicationChanael->getAnswCodeFromPlayer(keyCode - 26);
    }
}
