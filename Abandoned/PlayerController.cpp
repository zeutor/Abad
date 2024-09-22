#include "PlayerController.hpp"
#include "Player.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "SFML/Graphics.hpp"
#include <set>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "AStar.hpp"
#include <queue>
#include "Application.hpp"


using namespace sf;
using namespace std;
//std::vector<sf::Vector2i> _path;
//int _currentPathIndex = 0;

PlayerController* PlayerController::_controller = nullptr;

PlayerController::~PlayerController() {
    delete _controller;
}

//Vector2f lerp(const Vector2f& start, const Vector2f& end, float t) {
//    return start + (end - start) * t;
//}

PlayerController* PlayerController::getController() {
    if (!_controller)
        _controller = new PlayerController();

    return _controller;
}


vector<AStar::sNode*> path;
size_t currentTargetIndex = 0;

void PlayerController::controllPlayer(Player* player, float time, sf::RenderWindow* window) {
    static AStar astar;
    static bool isMouseHeld = false; // Флаг для отслеживания зажатия мыши

    // Если нажата левая кнопка мыши (однократное нажатие)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseHeld) {
        isMouseHeld = true;

        // Получаем координаты мыши
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        // Устанавливаем новую цель для поиска пути
        astar.setEnd(mousePos.x / 10, mousePos.y / 10);
        astar.setStart(player->getPosition().x / 10, player->getPosition().y / 10);
        astar.Solve_AStar();
        path = astar.getPath();
        currentTargetIndex = 0;
    }

    // Если мышь отпущена, сбрасываем флаг
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isMouseHeld = false;
    }

    // Если правая кнопка мыши нажата, добавляем препятствие
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        astar.setObstacle(mousePos.x / 10, mousePos.y / 10);
    }

    // Перемещение персонажа по пути
    if (!path.empty() && currentTargetIndex < path.size()) {
        sf::Vector2f targetPosition(path[currentTargetIndex]->x * 10, path[currentTargetIndex]->y * 10);
        player->moveTo(targetPosition, 35.0f, 0.1f);

        float reachThreshold = 5.0f;
        if (sqrt(pow(player->getPosition().x - targetPosition.x, 2) +
            pow(player->getPosition().y - targetPosition.y, 2)) < reachThreshold) {
            currentTargetIndex++;
        }
    }

    // Если мышь зажата, обновляем путь каждую итерацию, чтобы двигаться за мышкой
    if (isMouseHeld) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        sf::Vector2f goTo(mousePos.x, mousePos.y);
        player->moveTo(goTo, 35.0f, 0.1f);
        // Обновляем конечную позицию на текущую позицию мыши
    }
}
