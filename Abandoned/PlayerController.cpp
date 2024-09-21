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
#include <queue>

const int HEIGHT_MAP = 10;
const int WIDTH_MAP = 20;

struct Node {
    sf::Vector2i position;
    float gCost;
    float hCost;
    Node* parent;

    Node(sf::Vector2i pos, float g = 0, float h = 0, Node* p = nullptr)
        : position(pos), gCost(g), hCost(h), parent(p) {}

    float getFCost() const {
        return gCost + hCost;
    }

    bool operator==(const Node& other) const {
        return position == other.position;
    }

    bool operator<(const Node& other) const {
        return this->getFCost() < other.getFCost();
    }
};

struct CompareVector2i {
    bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
        return lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x;
    }
};

struct Vector2iHash {
    std::size_t operator()(const sf::Vector2i& vec) const {
        return std::hash<int>()(vec.x) ^ (std::hash<int>()(vec.y) << 1); // XOR с побитовым сдвигом
    }
};


struct CompareNode {
    bool operator()(Node* lhs, Node* rhs) const {
        return lhs->getFCost() > rhs->getFCost(); // Сравниваем через указатели
    }
};

float calculateHeuristic(sf::Vector2i start, sf::Vector2i end) {
    return std::abs(start.x - end.x) + std::abs(start.y - end.y);
}

bool isObstacle(sf::Vector2i pos) {
    return mapObjCode[pos.y][pos.x] == 'b';
}

float calculateDirectionalHeuristic(sf::Vector2i start, sf::Vector2i end) {
    int dx = std::abs(start.x - end.x);
    int dy = std::abs(start.y - end.y);
    // Prioritize downward movement by adding a weight
    return dx + dy + (start.y < end.y ? 0 : 1); // Give a penalty for moving up
}

std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i goal) {
    std::vector<sf::Vector2i> path;

    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet; // Используем указатели
    std::unordered_set<sf::Vector2i, Vector2iHash> closedSet;

    Node* startNode = new Node(start, 0, calculateDirectionalHeuristic(start, goal));
    openSet.push(startNode);

    while (!openSet.empty()) {
        Node* currentNode = openSet.top();
        openSet.pop();

        // Если достигли цели, восстанавливаем путь
        if (currentNode->position == goal) {
            Node* current = currentNode;
            while (current != nullptr) {
                path.push_back(current->position);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(currentNode->position);

        // Порядок соседей по приоритету
        sf::Vector2i neighbors[4] = {
            sf::Vector2i(0, 1),  // вниз
            sf::Vector2i(-1, 0), // влево
            sf::Vector2i(1, 0),  // вправо
            sf::Vector2i(0, -1)  // вверх
        };

        for (auto& offset : neighbors) {
            sf::Vector2i neighborPos = currentNode->position + offset;

            if (neighborPos.x < 0 || neighborPos.x >= WIDTH_MAP ||
                neighborPos.y < 0 || neighborPos.y >= HEIGHT_MAP)
                continue;

            if (isObstacle(neighborPos) || closedSet.find(neighborPos) != closedSet.end())
                continue;

            float tentativeG = currentNode->gCost + 1;

            bool nodeInOpenSet = false;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> tempQueue = openSet;

            while (!tempQueue.empty()) {
                Node* node = tempQueue.top();
                tempQueue.pop();
                if (node->position == neighborPos) {
                    nodeInOpenSet = true;
                    if (tentativeG < node->gCost) {
                        node->gCost = tentativeG;
                        node->parent = currentNode; // Указываем на текущий узел
                    }
                    break;
                }
            }

            if (!nodeInOpenSet) {
                float hCost = calculateDirectionalHeuristic(neighborPos, goal);
                Node* neighborNode = new Node(neighborPos, tentativeG, hCost, currentNode);
                openSet.push(neighborNode);
            }
        }
    }

    return path; // Путь не найден
}



PlayerController* PlayerController::_controller = nullptr;

PlayerController::~PlayerController() {
    delete _controller;
}

PlayerController* PlayerController::getController() {
    if (!_controller)
        _controller = new PlayerController();

    return _controller;
}
void PlayerController::controllPlayer(Player* player, float time, sf::RenderWindow* window) {
    static float totalDistance = 0; // Для хранения пройденного расстояния
    static bool isMousePressed = false; // Флаг для отслеживания состояния мыши
    sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));

    // Проверяем состояние мыши
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!isMousePressed) {
            // Сохранить состояние нажатия мыши
            isMousePressed = true;

            // Если это одиночный клик, ищем путь
            sf::Vector2i start(static_cast<int>(player->getPosition().x) / PIXELS_PER_CELL,
                static_cast<int>(player->getPosition().y) / PIXELS_PER_CELL);
            sf::Vector2i goal(static_cast<int>(mousePosition.x) / PIXELS_PER_CELL,
                static_cast<int>(mousePosition.y) / PIXELS_PER_CELL);
            _path = findPath(start, goal); // Найти путь
            _currentPathIndex = 0; // Сброс индекса на начало нового пути
        }

        // Если мышка зажата, просто движемся к позиции мыши
        sf::Vector2i newGoal(static_cast<int>(mousePosition.x) / PIXELS_PER_CELL,
            static_cast<int>(mousePosition.y) / PIXELS_PER_CELL);
        if (_path.empty() || _path.back() != newGoal) {
            // Если путь пуст или текущая цель не совпадает с курсором, обновляем цель
            _aimPosition = mousePosition;
            _path.clear(); // Очищаем путь
            _currentPathIndex = 0; // Сброс индекса
        }

        // Перемещение игрока к позиции мыши
        sf::Vector2f playerPos = player->getPosition();
        sf::Vector2f direction = mousePosition - playerPos;

        // Проверка на нулевую длину направления
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 0) {
            direction /= distance; // Нормализация направления
            float deltaX = direction.x * player->getSpeed() * time;
            float deltaY = direction.y * player->getSpeed() * time;

            // Обновляем общее пройденное расстояние
            totalDistance += std::sqrt(deltaX * deltaX + deltaY * deltaY); // Добавляем пройденное расстояние
            player->_distance = totalDistance;

            // Перемещение игрока
            playerPos += sf::Vector2f(deltaX, deltaY);

            // Проверка границ карты
            if (playerPos.x < 0) playerPos.x = 0;
            if (playerPos.x > WINDOW_WIDTH) playerPos.x = WINDOW_WIDTH;
            if (playerPos.y < 0) playerPos.y = 0;
            if (playerPos.y > WINDOW_HEIGHT) playerPos.y = WINDOW_HEIGHT;

            player->setPosition(playerPos);
        }

        return; // Выходим из функции, чтобы не обрабатывать путь
    }
    else {
        // Сброс флага, когда мышь отпущена
        isMousePressed = false;
    }

    // Если путь не пуст, продолжаем движение
    if (!_path.empty() && _currentPathIndex < _path.size()) {
        // Получение следующей целевой позиции по пути
        sf::Vector2f targetPos(_path[_currentPathIndex].x * PIXELS_PER_CELL,
            _path[_currentPathIndex].y * PIXELS_PER_CELL);
        sf::Vector2f playerPos = player->getPosition();

        // Рассчитываем направление и дистанцию до следующей точки
        sf::Vector2f direction = targetPos - playerPos;

        // Проверка коллизий по y
        if (direction.y < 0 && mapController->checkCollision(0, player->getPosition()))
            direction.y = 0;
        else if (direction.y > 0 && mapController->checkCollision(2, player->getPosition()))
            direction.y = 0;

        // Проверка коллизий по x   
        if (direction.x < 0 && mapController->checkCollision(3, player->getPosition()))
            direction.x = 0;
        else if (direction.x > 0 && mapController->checkCollision(1, player->getPosition()))
            direction.x = 0;

        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Если расстояние до следующей точки пути больше минимального значения, продолжаем движение
        if (distance > POSITION_EPSILON) {
            direction /= distance; // Нормализация направления
            sf::Vector2f updatedPos = playerPos + direction * player->getSpeed() * time * (distance < SLOW_WALK_DISTANCE ? SLOW_WALK_MULTIPLIER : 1);

            // Обновляем общее пройденное расстояние
            totalDistance += player->getSpeed() * time; // Добавляем пройденное расстояние
            player->_distance = totalDistance;

            // Проверка границ карты
            if (updatedPos.x < 0) updatedPos.x = 0;
            if (updatedPos.x > WINDOW_WIDTH) updatedPos.x = WINDOW_WIDTH;
            if (updatedPos.y < 0) updatedPos.y = 0;
            if (updatedPos.y > WINDOW_HEIGHT) updatedPos.y = WINDOW_HEIGHT;

            // Перемещаем игрока к следующей точке
            player->setPosition(updatedPos);
        }
        else {
            // Когда достигли текущей точки на пути, переходим к следующей
            _currentPathIndex++;

            // Если достигли конца пути, сбрасываем путь
            if (_currentPathIndex >= _path.size()) {
                _path.clear(); // Сброс пути
                _currentPathIndex = 0; // Сброс индекса
            }
        }
    }
}
