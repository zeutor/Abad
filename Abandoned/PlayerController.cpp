#include "PlayerController.hpp"
#include "Player.hpp"
#include "Constants.hpp"
#include "MapController.hpp"

PlayerController* PlayerController::_controller = nullptr;

PlayerController::~PlayerController()
{
    delete _controller;
}

PlayerController* PlayerController::getController() {
    if (!_controller)
        _controller = new PlayerController();

    return _controller;
}

void PlayerController::controllPlayer(Player* player, float time, sf::RenderWindow* window) {
    sf::Vector2f updated_pos = player->getPosition();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        _aimPosition = sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
    }
    if (abs(player->getPosition().x - _aimPosition.x) > POSITION_EPSILON || abs(player->getPosition().y - _aimPosition.y) > POSITION_EPSILON)
    {
        MapController* mapController = MapController::getController();

        float deltaX = _aimPosition.x - player->getPosition().x + 24;
        float deltaY = _aimPosition.y - player->getPosition().y + 24;

        float sqSum = deltaX * deltaX + deltaY * deltaY;

        deltaX = deltaX / abs(deltaX) * sqrt((deltaX * deltaX) / sqSum);
        deltaY = deltaY / abs(deltaY) * sqrt((deltaY * deltaY) / sqSum);

        deltaX = deltaX * player->getSpeed() * time * (sqrt(sqSum) < SLOW_WALK_DISTANCE ? SLOW_WALK_MULTIPLIER : 1);
        deltaY = deltaY * player->getSpeed() * time;

        if (deltaY < 0 && mapController->checkCollision(0, player->getPosition()))
            deltaY = 0;
        else if (deltaY > 0 && mapController->checkCollision(2, player->getPosition()))
            deltaY = 0;

        if (deltaX < 0 && mapController->checkCollision(3, player->getPosition()))
            deltaX = 0;
        else if (deltaX > 0 && mapController->checkCollision(1, player->getPosition()))
            deltaX = 0;

        player->_distance += sqrt(deltaX * deltaX + deltaY * deltaY);

        updated_pos.x += deltaX;
        updated_pos.y += deltaY;

    }
    player->setPosition(updated_pos);
}