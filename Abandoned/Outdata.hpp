#pragma once

#include "SFML/Graphics.hpp"

//То, что надо выгрузить из файлов и нужен постоянный доступ.
namespace outdata {
	sf::Texture whiteelf_texture;
	sf::Texture ork_texture;
	sf::Texture elf_texture;
	sf::Texture tifl_texture;

	sf::Font mainFont;

	static void getFiles() {
		whiteelf_texture.loadFromFile("data/textures/character/whiteElf.png");
		ork_texture.loadFromFile("data/textures/character/ork.png");
		tifl_texture.loadFromFile("data/textures/character/tifl.png");
		elf_texture.loadFromFile("data/textures/character/elf.png");

		mainFont.loadFromFile("data/fonts/tahoma.ttf");
	}
}