#pragma once

constexpr float WINDOW_HEIGHT = 720.0;
constexpr float WINDOW_WIDTH = 1280.0;
constexpr unsigned int FRAME_LIMIT = 60;

constexpr float TIME_MULTIPLIER = 7;

constexpr float SLOW_WALK_DISTANCE = 24.f;

constexpr float DEFAULT_SPEED = 15.f;
constexpr float RUN_MULTIPLIER = 1.25f;
constexpr float SLOW_WALK_MULTIPLIER = 0.5f;

constexpr float SIZE_MULTIPLIER = 3.f;

// Условная единица. Сейчас нужна лишь для отображения на экране пройденного расстояния в удобном виде.
constexpr float PIXELS_PER_METER = 100.f;
// Эту штуку вообще не менять. Если надо увеличить, то в SIZE_MULTIPLIER.
constexpr unsigned int PIXELS_PER_CELL = 16 * SIZE_MULTIPLIER;

// Радиус окружности, в которой будет считаться, что сущность достигла точки.
constexpr float POSITION_EPSILON = PIXELS_PER_CELL / 2;
// Расстояние за которое сущность должна остановиться от коллизионного элемента.
constexpr float BLOCK_EPSILON = 2.5f;