#include "PlayerController.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "SFML/Graphics.hpp"
#include "Application.hpp"
#include "Character.hpp"
#include <vector>
#include <thread>
#include <chrono>
#include "GameCamera.hpp"

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


void PlayerController::controllPlayer(Character& player, float time, sf::RenderWindow* window) {
    static bool isMouseHeld = false; // Флаг для отслеживания зажатия мыши

    MapController* mapController = MapController::getController();
    sf::Vector2f playerPos = player.getPosition();

    // Управление мышью
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseHeld) {
        isMouseHeld = true;
        frameCounter = 0;
        sf::Vector2f mousePos = GameCamera::getMapMousePos();

        if (!mapController->checkCollision(-1, mousePos)) {
            player._astar.setEnd(mousePos.x / PIXELS_FOR_OBSTACLE, mousePos.y / PIXELS_FOR_OBSTACLE);
            player._astar.setStart(playerPos.x / PIXELS_FOR_OBSTACLE, playerPos.y / PIXELS_FOR_OBSTACLE);
            player._astar.Solve_AStar();
            player._path = player._astar.getPath();
            player._currentTargetIndex = 0;
        }
    }

    if (frameCounter < FRAME_LIMIT)
        ++frameCounter;

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isMouseHeld = false;
    }


    // Перемещение персонажа по пути
    if (!player._path.empty() && player._currentTargetIndex < player._path.size()) {
        sf::Vector2f targetPosition = sf::Vector2f(player._path[player._currentTargetIndex]->x * PIXELS_FOR_OBSTACLE, player._path[player._currentTargetIndex]->y * PIXELS_FOR_OBSTACLE);
        player.moveTo(targetPosition, time);

        if (sqrt(pow(player.getPosition().x - targetPosition.x, 2) +
            pow(player.getPosition().y - targetPosition.y, 2)) < POSITION_EPSILON) {
            player._currentTargetIndex++;
        }
    }

    // Обновление пути при движении за мышкой
    if (isMouseHeld && frameCounter > FRAME_LIMIT / 6) {
        player._path.clear();
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        sf::Vector2f goTo(mousePos.x, mousePos.y);
        player.moveTo(goTo, time);
    }
}
