#pragma once

#include "SFML/Graphics.hpp"


//��, ��� ���� ��������� �� ������ � ����� ���������� ������.
namespace outdata {
	extern sf::Texture player_texture;
	extern sf::Texture action_cell;
	extern sf::Texture magic_cell;
	extern sf::Texture empty_cell;
	extern sf::Font mainFont;
	extern sf::Texture menu_icon;
	extern sf::Texture invent_icon;
	extern sf::Texture journal_icon;
	

	static void getFiles() {
		player_texture.loadFromFile("data/textures/character/whiteElf.png");
		mainFont.loadFromFile("data/fonts/tahoma.ttf");
		action_cell.loadFromFile("data/textures/cells/Action_Cell.png");
		empty_cell.loadFromFile("data/textures/cells/Empty_Cell.png");
		menu_icon.loadFromFile("data/textures/icons/menu_icon.png");
		invent_icon.loadFromFile("data/textures/icons/inventory_icon.png");
		journal_icon.loadFromFile("data/textures/icons/journal_icon.png");
	}
}