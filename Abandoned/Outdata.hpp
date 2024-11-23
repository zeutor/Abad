#pragma once

#include "SFML/Graphics.hpp"
#include<vector>

//��, ��� ���� ��������� �� ������ � ����� ���������� ������.
namespace outdata {
	sf::Texture whiteelf_texture;
	sf::Texture ork_texture;
	sf::Texture elf_texture;
	sf::Texture tifl_texture;

	extern sf::Texture action_cell;
	extern sf::Texture magic_cell;
	extern sf::Texture empty_cell;
	extern sf::Font mainFont;

	extern sf::Texture menu_icon;
	extern sf::Texture invent_icon;
	extern sf::Texture journal_icon;
	static std::vector<sf::Sprite> vectorWithIcons;

	extern sf::Texture Download_game;
	extern sf::Texture Statistic;
	extern sf::Texture Start_game;
	extern sf::Texture Settings;
	extern sf::Texture Exit;



	static void getFiles() {
		whiteelf_texture.loadFromFile("data/textures/character/whiteElf.png");
		ork_texture.loadFromFile("data/textures/character/ork.png");
		tifl_texture.loadFromFile("data/textures/character/tifl.png");
		elf_texture.loadFromFile("data/textures/character/elf.png");

		mainFont.loadFromFile("data/fonts/tahoma.ttf");
		action_cell.loadFromFile("data/textures/cells/Action_Cell.png");
		empty_cell.loadFromFile("data/textures/cells/Empty_Cell.png");
		menu_icon.loadFromFile("data/textures/icons/menu_icon.png");
		invent_icon.loadFromFile("data/textures/icons/inventory_icon.png");
		journal_icon.loadFromFile("data/textures/icons/journal_icon.png");

		Download_game.loadFromFile("data/textures/menu_buttons/Download_game.png");
		Statistic.loadFromFile("data/textures/menu_buttons/Statistic.png");
		Start_game.loadFromFile("data/textures/menu_buttons/Start_game.png");
		Settings.loadFromFile("data/textures/menu_buttons/Settings.png");
		Exit.loadFromFile("data/textures/menu_buttons/Exit.png");

		sf::Sprite menu(outdata::menu_icon);
		sf::Sprite invent(outdata::invent_icon);
		sf::Sprite journal(outdata::journal_icon);
	
		vectorWithIcons.push_back(menu);
		vectorWithIcons.push_back(invent);
		vectorWithIcons.push_back(journal);
	}
}