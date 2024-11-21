#pragma once
#include "SFML/Graphics.hpp"


class GameCamera
{
private:
	static GameCamera* _mainCamera;
	static sf::RenderWindow* _window;

	bool _isMainCamera;
	
	sf::Vector2f _position;
	sf::View _view;
public:
	/// <summary>
	/// Инициализирует ссылку на окно, на которое будет отображено зрение камеры.
	/// </summary>
	static void INIT(sf::RenderWindow* window);
	
	/// <summary>
	/// Возвращает положение курсора относительно верхнего левого угла карты, а не экрана.
	/// </summary>
	static sf::Vector2f getMapMousePos();
	static void moveMainCamera(sf::Vector2f step);
	static void updateView();

	GameCamera();
	GameCamera(sf::Vector2f position);

	void setAsMain();

	void move(sf::Vector2f step);
	
	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition() const;

	static GameCamera* getMainCamera();
};

