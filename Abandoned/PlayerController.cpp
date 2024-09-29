#include "PlayerController.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "SFML/Graphics.hpp"
#include "Application.hpp"
#include "Character.hpp"
#include <vector>

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
    static bool isMouseHeld = false; // Флаг для отслеживания зажатия мыши

    MapController* mapContorller = MapController::getController();

    // Если нажата левая кнопка мыши (однократное нажатие)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseHeld) {
        isMouseHeld = true;
        frameCounter = 0;
        // Получаем координаты мыши
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        
        if (!mapContorller->isCollisionObjOnPos(mousePos / PIXELS_PER_CELL))
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
    if(frameCounter < FRAME_LIMIT)
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
    if (isMouseHeld && frameCounter > FRAME_LIMIT/6) {
        path = vector<AStar::sNode*>();
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        sf::Vector2f goTo(mousePos.x, mousePos.y);
        player.moveTo(goTo, time);
        // Обновляем конечную позицию на текущую позицию мыши
    }
}
