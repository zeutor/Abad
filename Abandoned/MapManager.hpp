#pragma once
#include "SFML/Graphics.hpp"
#include "Constants.hpp"

namespace mapManager {
	const int HEIGHT_MAP = 10;
	const int WIDTH_MAP = 20;
	sf::String mapGroundCode[10] = {
	"gfgggggGgggGgggggggg",
	"gggggggfgggggggggggg",
	"gggGggggggggggfggggg",
	"gggfgggfgggGgggggggg",
	"gggggggggggggfggGggg",
	"gggggggfgggggggggggg",
	"gggggggggggggggggfgg",
	"gfgGggggggGggggggggg",
	"ggggggggggfggGggfggg",
	"ggggggGggggggggggggg",
	};
	sf::String mapObjCode[10] = {
	"     b             ",
	"     b             ",
	"     b             ",
	"bbbb    bbbb       ",
	"                   ",
	"     b             ",
	"                   ",
	"                   ",
	"                   ",
	"      b            ",
	};
	sf::Texture imgSource;

	/*directions: 0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT*/
	/*bool checkCollision(int direction, sf::Vector2f objPosition) {
		switch (direction)
		{
		case 0:
			if (mapObjCode[((int)objPosition.x) / 32][((int)objPosition.y - BLOCK_EPSILON) / 32] == 'b')
				return false;
			break;
		case 1:
			if (mapObjCode[((int)(objPosition.x + BLOCK_EPSILON)) / 32 + 1][((int)objPosition.y) / 32] == 'b')
				return false;
			break;
		case 2:
			if (mapObjCode[((int)objPosition.x) / 32][((int)(objPosition.y + BLOCK_EPSILON)) / 32 + 1] == 'b')
				return false;
			break;
		case 3:
			if (mapObjCode[((int)(objPosition.y - BLOCK_EPSILON)) / 32][((int)objPosition.y) / 32] == 'b')
				return false;
			break;
		default:
			break;
		}
		return true;
	}*/

	void drawMap(sf::RenderWindow& window)
	{
		imgSource.loadFromFile("./textures/map/tilemap_packed.png");
		sf::Sprite mapTile;
		mapTile.setTexture(imgSource);
		for (int i = 0; i < HEIGHT_MAP; ++i)
		{
			for (int j = 0; j < WIDTH_MAP; ++j)
			{
				switch (mapGroundCode[i][j])
				{
				case 'g':
					mapTile.setTextureRect(sf::IntRect(0, 0, 16, 16));
					break;
				case 'f':
					mapTile.setTextureRect(sf::IntRect(32, 0, 16, 16));
					break;
				case 'G':
					mapTile.setTextureRect(sf::IntRect(16, 0, 16, 16));
					break;
				}
				mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
				mapTile.setPosition(j * PIXELS_PER_CELL, i * PIXELS_PER_CELL);
				window.draw(mapTile);
			}
		}
		for (int i = 0; i < HEIGHT_MAP; ++i)
		{
			for (int j = 0; j < WIDTH_MAP; ++j)
			{
				switch (mapObjCode[i][j])
				{
				case 'b':
					mapTile.setTextureRect(sf::IntRect(80, 0, 16, 16));
					break;
				case 'g':
					mapTile.setTextureRect(sf::IntRect(80, 16, 16, 16));
					break;
				case 'm':
					mapTile.setTextureRect(sf::IntRect(80, 32, 16, 16));
					break;
				default:
					continue;
				}
				mapTile.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);
				mapTile.setPosition(j * PIXELS_PER_CELL, i * PIXELS_PER_CELL);
				window.draw(mapTile);
			}
		}
	}
}

