#pragma once
#include <SFML/Graphics.hpp>
class Application
{
	sf::RenderWindow* _applicationWindow;
public:
	void INIT();
	bool RUN();
	void CLOSE();
};

