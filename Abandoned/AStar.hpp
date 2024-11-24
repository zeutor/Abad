#pragma once
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
    //Убрали все статические функции, адаптировали под AStar для отдельных игроков
    void INIT();

    AStar();

    // Установка начальной позиции для A* алгоритма
    void setStart(int x, int y);

    // Установка конечной позиции для A* алгоритма
    void setEnd(int x, int y);

    // Установка препятствия на карте
    void setObstacle(int x, int y);

    // Удаление препятствия с карты
    void unsetObstacle(int x, int y);

    // Удаление всех препятствий с карты
     void removeAllObstacle();

    // Возвращает список всех препятствий
     vector<Vector2f> getObstacles();

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

    //убрали все статические переменные 
    sNode* nodes = nullptr;      // Массив узлов карты
    int nMapWidth = WINDOW_WIDTH;
    int nMapHeight = WINDOW_HEIGHT; // Размеры карты
    vector<Vector2f> obstacles; // Препятствия // Список препятствий

    sNode* nodeStart = nullptr;  // Начальный узел
    sNode* nodeEnd = nullptr;    // Конечный узел

    // Функция для получения пути от начальной до конечной точки
    vector<sNode*> getPath();

    // Алгоритм поиска пути A*
    void Solve_AStar();

    // Восстанавливаем путь от конечного узла к стартовому, помечая его как пройденный
    void path();
};
