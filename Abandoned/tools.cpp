#include "tools.hpp"
#include "Constants.hpp"

using namespace tools;

float tools::distance(sf::Vector2f firstPoint, sf::Vector2f secondPoint)
{
    return vectorLength(secondPoint - firstPoint);
}

float tools::vectorLength(sf::Vector2f vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f tools::normalize(sf::Vector2f vector)
{
    float vecLen = vectorLength(vector);
    if (vecLen == 0)
        return vector;
    return vector / vecLen;
}

sf::Vector2f tools::fromCellsToPixels(sf::Vector2i pointInCells)
{
    return sf::Vector2f(pointInCells * PIXELS_PER_CELL);
}

sf::Vector2i tools::fromPixelsToCells(sf::Vector2f pointInPixels)
{
    return sf::Vector2i(pointInPixels / (float)PIXELS_PER_CELL);
}

int tools::distanceInCells(sf::Vector2i firstCell, sf::Vector2i secondCell)
{
    sf::Vector2i def = secondCell - firstCell;
    return abs(def.x) + abs(def.y);
}