#pragma once
#include "SFML/Graphics.hpp"
#include "Constants.hpp"

class Application
{
private:
	sf::RenderWindow* _window;
public:
	//Отвечает за размер окна и ограничение по FPS
	void INIT();
	//Отвечает за весь рабочий процесс программы:
	//1. Загрузка файлов
	//2. Обновление кадров
	//3. Синхронизация времени и процессов в программе
	//4. Загрузку первых элементов
	void RUN();
	// Просто закрыл и все. Че еще хочешь?
	void CLOSE();
};

