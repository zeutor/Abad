#pragma once

#include "SFML/Graphics.hpp"


//��, ��� ���� ��������� �� ������ � ����� ���������� ������.
namespace outdata {
	extern sf::Texture player_texture;
	extern sf::Texture action_cell;
	extern sf::Texture magic_cell;
	extern sf::Texture empty_cell;
	extern sf::Font mainFont;

	static void getFiles() {
		player_texture.loadFromFile("data/textures/character/whiteElf.png");
		mainFont.loadFromFile("data/fonts/tahoma.ttf");
		action_cell.loadFromFile("data/textures/cells/Action_Cell.png");
		empty_cell.loadFromFile("data/textures/cells/Empty_Cell.png");

	}
}