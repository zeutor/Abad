#pragma once

constexpr float WINDOW_HEIGHT = 720.0;
constexpr float WINDOW_WIDTH = 1280.0;
constexpr unsigned int FRAME_LIMIT = 60;

constexpr float TIME_MULTIPLIER = 7;

constexpr float SIZE_MULTIPLIER = 3.f;

constexpr float SLOW_WALK_DISTANCE = 24.f;

constexpr float DEFAULT_SPEED = 20.f * SIZE_MULTIPLIER;
constexpr float RUN_MULTIPLIER = 1.25f;
constexpr float SLOW_WALK_MULTIPLIER = 0.5f;

constexpr float COLLISION_MULTIPLIER = -0.1f;

constexpr int PIXELS_FOR_OBSTACLE = 8;

constexpr int ICON_SIZE = 50;
constexpr int INVENTORY_COLS = 6;
constexpr int INVENTORY_ROWS = 8;

constexpr float DEFAULT_OBJECT_SIZE = 1.f * SIZE_MULTIPLIER;
constexpr float BLOCK_PUSH_MULTIPLIER = 0.f;

// Óñëîâíàÿ åäèíèöà. Ñåé÷àñ íóæíà ëèøü äëÿ îòîáðàæåíèÿ íà ýêðàíå ïðîéäåííîãî ðàññòîÿíèÿ â óäîáíîì âèäå.
constexpr float PIXELS_PER_METER = 100.f;

// ��� ����� ������ �� ������. ���� ���� ���������, �� � SIZE_MULTIPLIER.
constexpr int PIXELS_PER_CELL = 16 * SIZE_MULTIPLIER;

// Ðàäèóñ îêðóæíîñòè, â êîòîðîé áóäåò ñ÷èòàòüñÿ, ÷òî ñóùíîñòü äîñòèãëà òî÷êè.
constexpr float POSITION_EPSILON = PIXELS_PER_CELL /1.5;
// Ðàññòîÿíèå çà êîòîðîå ñóùíîñòü äîëæíà îñòàíîâèòüñÿ îò êîëëèçèîííîãî ýëåìåíòà.
constexpr float BLOCK_EPSILON = 1.5f;

constexpr float INTERACT_DISTANSE = 20.f * SIZE_MULTIPLIER;

// Определение границ (например, для окна размером 800x600)
const float minX = 0.0f; // Минимальная X-координата
const float maxX = 800.0f; // Максимальная X-координата
const float minY = 0.0f; // Минимальная Y-координата
const float maxY = 600.0f;
