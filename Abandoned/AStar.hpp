#include "SFML/Graphics.hpp"
#include "Constants.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <list>

using namespace sf;
using namespace std;

class AStar {
public:
    static void INIT();

    AStar();

    // Установка начальной позиции для A* алгоритма
    void setStart(int x, int y);

    // Установка конечной позиции для A* алгоритма
    void setEnd(int x, int y);

    // Установка препятствия на карте
    static void setObstacle(int x, int y);

    // Удаление препятствия с карты
    static void unsetObstacle(int x, int y);

    // Удаление всех препятствий с карты
    static void removeAllObstacle();

    // Возвращает список всех препятствий
    static vector<Vector2f> getObstacles();

    // Структура узла карты
    struct sNode {
        bool isParent = false;  // Флаг, указывающий, что этот узел является частью пути
        bool bObstacle = false; // Является ли узел препятствием
        bool bVisited = false;  // Был ли узел посещён
        float fGlobalGoal;      // Глобальная оценка (включает эвристику)
        float fLocalGoal;       // Локальная оценка (расстояние от старта)
        int x, y;               // Координаты узла на карте
        vector<sNode*> vecNeighbours; // Соседние узлы
        sNode* parent;          // Родительский узел, используемый для восстановления пути
    };

    static sNode* nodes;      // Массив узлов карты
    static int nMapWidth;         // Ширина карты
    static int nMapHeight;         // Высота карты
    static vector<Vector2f> obstacles;  // Список препятствий

    sNode* nodeStart = nullptr;  // Начальный узел
    sNode* nodeEnd = nullptr;    // Конечный узел

    // Функция для получения пути от начальной до конечной точки
    vector<sNode*> getPath();

    // Алгоритм поиска пути A*
    void Solve_AStar();

    // Восстанавливаем путь от конечного узла к стартовому, помечая его как пройденный
    void path();
};
