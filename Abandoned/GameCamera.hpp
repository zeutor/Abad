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

	// �������, � ������� ������ ����� ���������.
	sf::FloatRect _boxOfMotion;

	float _speed;
public:
	/// <summary>
	/// �������������� ������ �� ����, �� ������� ����� ���������� ������ ������.
	/// </summary>
	static void INIT(sf::RenderWindow* window);
	
	/// <summary>
	/// ���������� ��������� ������� ������������ �������� ������ ���� �����, � �� ������.
	/// </summary>
	static sf::Vector2f getMapMousePos();

	/// <summary>
	/// ���������� �������� ������ �� ������ step.
	/// </summary>
	/// <param name="step">- ��� ��� �����������</param>
	static void moveMainCamera(sf::Vector2f step);
	/// <summary>
	/// ��������� �����, � ������������ �� "�������" �������� ������.
	/// </summary>
	static void updateView();

	GameCamera();
	GameCamera(sf::Vector2f position);

	/// <summary>
	/// ������������� ������ ������ ��� ��������.
	/// </summary>
	void setAsMain();

	/// <summary>
	/// ���������� ������ ������ �� step. ����������� ������ � �������� ������.
	/// </summary>
	/// <param name="step">- ������ ����������� (� ������� �������).</param>
	void move(sf::Vector2f step);
	
	/// <summary>
	/// ���������� ������ � ������������ � ������������, ��������� � ��������� ����������� �������. ����������� ������ � �������� ������.
	/// </summary>
	/// <param name="direction"> - ����������� �������� (����� ���� ������, � ������� ���������� ���������).</param>
	/// <param name="deltaTime"> - ��������� ���������� �������.</param>
	void move(sf::Vector2f direction ,float deltaTime);

	/// <summary>
	/// ������������� ������� ������ �� position.
	/// </summary>
	/// <param name="position">- ����� ������� ������.</param>
	void setPosition(sf::Vector2f position);
	/// <summary>
	/// ���������� ������� ������.
	/// </summary>
	/// <returns>������ ��������� (x,y), ��� ����������� ������.</returns>
	sf::Vector2f getPosition() const;

	/// <summary>
	/// ������������� �������� ����������� ������.
	/// </summary>
	/// <param name="speed">- ����� �������� �������� ������.</param>
	void setSpeed(float speed);
	/// <summary>
	/// ���������� �������� �������� ������.
	/// </summary>
	/// <returns>Float �������� �������� ������.</returns>
	float getSpeed() const;

	/// <summary>
	/// ������������� ��������, � ������� ����� ������ ������.
	/// </summary>
	/// <param name="borders"> - �������������, � ������� ����� ������ ������.</param>
	void setBorders(sf::FloatRect borders);

	/// <summary>
	/// ���������� �������, � ������� ����� ��������� ������.
	/// </summary>
	/// <returns>FloatRect, � ������� ����� ��������� ������.</returns>
	sf::FloatRect getBorders() const;

	/// <summary>
	/// ���������� ������ �� �������� ������.
	/// </summary>
	/// <returns>������ �� ������ �������� ������.</returns>
	static GameCamera* getMainCamera();
};

