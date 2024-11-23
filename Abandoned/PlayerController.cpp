#include "PlayerController.hpp"
#include "Constants.hpp"
#include "MapController.hpp"
#include "SFML/Graphics.hpp"
#include "Application.hpp"
#include "Character.hpp"
#include <vector>

using namespace sf;
using namespace std;
#include <set>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include "UIManager.hpp"
#include "Object.hpp"



std::vector<sf::Vector2i> _path;
int _currentPathIndex = 0;

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
    MapController* mc = MapController::getController();
    return mc->isCollisionObjOnPos(pos);
}

float calculateDirectionalHeuristic(sf::Vector2i start, sf::Vector2i end) {
    int dx = std::abs(start.x - end.x);
    int dy = std::abs(start.y - end.y);
    return dx + dy + (start.y < end.y ? 0 : 1); 
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

        MapController* mc = MapController::getController();

        for (auto& offset : neighbors) {
            sf::Vector2i neighborPos = currentNode->position + offset;

            if (neighborPos.x < 0 || neighborPos.x >= mc->getMapSize().x ||
                neighborPos.y < 0 || neighborPos.y >= mc->getMapSize().y)
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

    std::unordered_set<MapObject*> allMapObject = MapObject::getAllMapObjects();

    UIManager* UIController = UIManager::getController();
    std::vector<UISlot> Slots = UIController->getInvConroller();
    Slots[9].setActionID(2); //вектор с UI элементами, ниже HandleClick - для проверки клика на UI Элементы

    std::unordered_set<Object*> AllObject = Object::getAllObjects();
     //Проверка на нажатие на объекты, если нажал, то появится в инвентаре

    MapController* mapController = MapController::getController();
    sf::Vector2i mousepostrue = sf::Mouse::getPosition(*window);
    // Проверяем состояние мыши

    

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        MouseTake(player, AllObject, (sf::Vector2f)mousepostrue);
    }


    
    // Если нажата левая кнопка мыши (однократное нажатие)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseHeld && !UIController->IsOnUIClicked(mousepostrue)) {

        ActivateMapObj(player, (sf::Vector2f)mousepostrue, allMapObject);
    

        
        isMouseHeld = true;
        frameCounter = 0;
        // Получаем координаты мыши
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        
        if (!mapController->isCollisionObjOnPos(mousePos / PIXELS_PER_CELL))
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
