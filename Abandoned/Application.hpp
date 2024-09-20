#pragma once
#include "SFML/Graphics.hpp"
#include "Constants.hpp"

class Application
{
private:
	sf::RenderWindow* _window;
public:
	void INIT();
	void RUN();
	void CLOSE();
};

