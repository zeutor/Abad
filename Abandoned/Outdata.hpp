#pragma once

#include "SFML/Graphics.hpp"

namespace outdata {
	sf::Texture player_texture;

	sf::Font mainFont;

	static void getFiles() {
		player_texture.loadFromFile("./textures/character/whiteElf.png");
		mainFont.loadFromFile("./fonts/tahoma.ttf");
	}
}