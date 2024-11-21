#pragma once
#include "SFML/Graphics.hpp"

class Object;

namespace tools {

	/// <summary>
	/// Returns distance between points.
	/// </summary>
	/// <returns>Float value of distance between first and second points.</returns>
	float distance(sf::Vector2f firstPoint, sf::Vector2f secondPoint);

	/// <summary>
	/// Returns length of vector.
	/// </summary>
	/// <returns>Float value of vector's length.</returns>
	float vectorLength(sf::Vector2f vector);

	/// <summary>
	/// Returns normalized ver. of vector.
	/// </summary>
	/// <returns>Vector that has direction as "vector", but len = 1.</returns>
	sf::Vector2f normalize(sf::Vector2f vector);

	/// <summary>
	/// Returns top left pixel of cell.
	/// </summary>
	/// <param name="pointInCells"> - (x,y) of cell.</param>
	/// <returns> - (x,y) of top left pixel of inp cell.</returns>
	sf::Vector2f fromCellsToPixels(sf::Vector2i pointInCells);

	/// <summary>
	/// Returns (x,y) of cell, that contains this point.
	/// </summary>
	/// <param name="pointInCells"> - (x,y) of point.</param>
	/// <returns> - (x,y) of cell, that contains point.</returns>
	sf::Vector2i fromPixelsToCells(sf::Vector2f pointInPixels);
	
	/// <summary>
	/// Returns how match cells you need to pass from first to second cell.
	/// </summary>
	/// <returns>Int value of distance between first cell to second cell.</returns>
	int distanceInCells(sf::Vector2i firstCell, sf::Vector2i secondCell);
}