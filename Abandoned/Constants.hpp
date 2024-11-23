#pragma once

// app constants:

constexpr float WINDOW_HEIGHT = 720.0;
constexpr float WINDOW_WIDTH = 1280.0;
constexpr unsigned int FRAME_LIMIT = 60;


// time constants: 

constexpr float TIME_MULTIPLIER = 7;


// surraunding constants:

constexpr float SIZE_MULTIPLIER = 3.f;
constexpr float COLLISION_MULTIPLIER = 0;
constexpr int PIXELS_FOR_OBSTACLE = 8;

constexpr int PIXELS_PER_CELL = 16 * SIZE_MULTIPLIER;

constexpr float BLOCK_EPSILON = 1.5f;
constexpr float POSITION_EPSILON = PIXELS_PER_CELL / 1.5;


// speed constants:

constexpr float SLOW_WALK_DISTANCE = 24.f;

constexpr float DEFAULT_SPEED = 20.f * SIZE_MULTIPLIER;
constexpr float RUN_MULTIPLIER = 1.25f;
constexpr float SLOW_WALK_MULTIPLIER = 0.5f;


// minimap constants:

constexpr float miniMapWidth = 0.25;
constexpr float miniMapHeight = 0.25;
constexpr float miniMapLeft = 0.75;
constexpr float miniMapTop = 0;


// UI constatns:

constexpr int ICON_SIZE = 50;
constexpr int INVENTORY_COLS = 6;
constexpr int INVENTORY_ROWS = 8;

// Obj constatns:

constexpr float DEFAULT_OBJECT_SIZE = (SIZE_MULTIPLIER - 1) + (float)(PIXELS_PER_CELL - SIZE_MULTIPLIER * 2) / (float)PIXELS_PER_CELL;

// other constants:

constexpr float PIXELS_PER_METER = 100.f;


