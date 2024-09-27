#include "AStar.hpp"

AStar::sNode* AStar::nodes = nullptr;      // Массив узлов карты
int AStar::nMapWidth = WINDOW_WIDTH;         // Ширина карты
int AStar::nMapHeight = WINDOW_HEIGHT;         // Высота карты
vector<Vector2f> AStar::obstacles = vector<Vector2f>();

AStar::AStar() {
    OnUserCreate();  // Инициализация карты
}

// Установка начальной позиции для A* алгоритма
void AStar::setStart(int x, int y) {
    nodeStart = &nodes[y * nMapWidth + x];
}

// Установка конечной позиции для A* алгоритма
void AStar::setEnd(int x, int y) {
    nodeEnd = &nodes[y * nMapWidth + x];
}

// Установка препятствия на карте
void AStar::setObstacle(int x, int y) {
    nodes[y * nMapWidth + x].bObstacle = true;
    obstacles.push_back(Vector2f(x * PIXELS_FOR_OBSTACLE, y * PIXELS_FOR_OBSTACLE));  // Добавление координат препятствия
}

// Удаление препятствия с карты
void AStar::unsetObstacle(int x, int y) {
    nodes[y * nMapWidth + x].bObstacle = false;

    // Удаление координат препятствия из списка
    obstacles.erase(remove_if(obstacles.begin(), obstacles.end(),
        [=](Vector2f pos) { return pos.x == x * PIXELS_FOR_OBSTACLE && pos.y == y * PIXELS_FOR_OBSTACLE; }), obstacles.end());
}

void AStar::removeAllObstacle()
{
    obstacles = std::vector<sf::Vector2f>();
}



// Возвращает список всех препятствий
vector<Vector2f> AStar::getObstacles() {
    return obstacles;
}


// Функция для получения пути от начальной до конечной точки
vector<AStar::sNode*> AStar::getPath() {
    vector<sNode*> path;
    if (nodeEnd != nullptr) {
        sNode* p = nodeEnd;
        // Восстанавливаем путь от конечной до начальной точки
        while (p->parent != nullptr) {
            path.push_back(p);
            p = p->parent;
        }
        reverse(path.begin(), path.end()); // Переворачиваем путь, чтобы он был от старта до конца
    }
    return path;
}

/* Инициализация карты узлов :
*   Создаются узлы, которые не препятствия
*   Установление их базовых характеристик
*   Установка соседей для узлов
*/
void AStar::OnUserCreate() {
    nodes = new sNode[nMapWidth * nMapHeight];
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++) {
            nodes[y * nMapWidth + x].x = x;
            nodes[y * nMapWidth + x].y = y;
            nodes[y * nMapWidth + x].bObstacle = false;  // По умолчанию узлы не являются препятствиями
            nodes[y * nMapWidth + x].parent = nullptr;   // Нет родительских узлов на старте
            nodes[y * nMapWidth + x].bVisited = false;   // Узел ещё не посещался
        }

    // Установка соседей для каждого узла
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++) {
            if (y > 0)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + x]);  // Верхний сосед
            if (y < nMapHeight - 1)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + x]);  // Нижний сосед
            if (x > 0)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[y * nMapWidth + (x - 1)]);  // Левый сосед
            if (x < nMapWidth - 1)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[y * nMapWidth + (x + 1)]);  // Правый сосед
        }

    nodeStart = &nodes[0];   // Стартовый узел по умолчанию
    nodeEnd = &nodes[12];    // Конечный узел по умолчанию
}

// Алгоритм поиска пути A*
void AStar::Solve_AStar() {
    // Сброс всех узлов для нового поиска
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++) {
            nodes[y * nMapWidth + x].isParent = false;
            nodes[y * nMapWidth + x].bVisited = false;
            nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
            nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
            nodes[y * nMapWidth + x].parent = nullptr;
        }

    // Функция для вычисления расстояния между двумя узлами
    auto distance = [](sNode* a, sNode* b) {
        return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
        };

    sNode* nodeCurrent = nodeStart;  // Начинаем с начального узла
    nodeStart->fLocalGoal = 0.0f;    // Локальная цель для начального узла — 0
    nodeStart->fGlobalGoal = distance(nodeStart, nodeEnd);  // Глобальная цель — эвристика до конца

    list<sNode*> listNotTestedNodes;
    listNotTestedNodes.push_back(nodeStart);  // Добавляем стартовый узел в список

    // Основной цикл A* алгоритма
    while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) {
        // Сортировка узлов по глобальной оценке
        listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

        // Удаляем узлы, которые уже были посещены
        while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
            listNotTestedNodes.pop_front();

        if (listNotTestedNodes.empty())
            break;

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->bVisited = true;  // Отмечаем узел как посещённый

        // Обрабатываем всех соседей текущего узла
        for (auto nodeNeighbour : nodeCurrent->vecNeighbours) {
            if (!nodeNeighbour->bVisited && !nodeNeighbour->bObstacle)
                listNotTestedNodes.push_back(nodeNeighbour);

            // Вычисляем потенциально более короткий путь через текущий узел
            float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

            // Если найден более короткий путь, обновляем оценки узла
            if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;
                nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + distance(nodeNeighbour, nodeEnd);
            }
        }
    }
}

// Восстанавливаем путь от конечного узла к стартовому, помечая его как пройденный
void AStar::path() {
    if (nodeEnd != nullptr) {
        sNode* p = nodeEnd;
        while (p->parent != nullptr) {
            nodes[p->y * nMapWidth + p->x].isParent = true;
            p = p->parent;
        }
    }
}
