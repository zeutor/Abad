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

	// √раницы, в которых камера может двигатьс€.
	sf::FloatRect _boxOfMotion;

	float _speed;
public:
	/// <summary>
	/// »нициализирует ссылку на окно, на которое будет отображено зрение камеры.
	/// </summary>
	static void INIT(sf::RenderWindow* window);
	
	/// <summary>
	/// ¬озвращает положение курсора относительно верхнего левого угла карты, а не экрана.
	/// </summary>
	static sf::Vector2f getMapMousePos();

	/// <summary>
	/// ѕеремещает основную камеру на вектор step.
	/// </summary>
	/// <param name="step">- шаг дл€ перемещение</param>
	static void moveMainCamera(sf::Vector2f step);
	/// <summary>
	/// ќбновл€ет экран, в соответствии со "зрением" основной камеры.
	/// </summary>
	static void updateView();

	GameCamera();
	GameCamera(sf::Vector2f position);

	/// <summary>
	/// ”станавливает данную камеру как основную.
	/// </summary>
	void setAsMain();

	/// <summary>
	/// ѕеремещает данную камеру на step. ѕеремещение только в пределах границ.
	/// </summary>
	/// <param name="step">- вектор перемещени€ (в игровых клетках).</param>
	void move(sf::Vector2f step);
	
	/// <summary>
	/// ѕеремещает камеру в соответвтвии с направлением, скоростью и прошедшим промежутком времени. ѕеремещение только в пределах границ.
	/// </summary>
	/// <param name="direction"> - направление движени€ (может быть точкой, к которой необходимо двигатьс€).</param>
	/// <param name="deltaTime"> - прошедший промежуток времени.</param>
	void move(sf::Vector2f direction ,float deltaTime);

	/// <summary>
	/// ”станавливает позицию камеры на position.
	/// </summary>
	/// <param name="position">- нова€ позици€ камеры.</param>
	void setPosition(sf::Vector2f position);
	/// <summary>
	/// ¬озвращает позицию камеры.
	/// </summary>
	/// <returns>¬ектор координат (x,y), где расположена камера.</returns>
	sf::Vector2f getPosition() const;

	/// <summary>
	/// ”станавливает скорость перемещени€ камеры.
	/// </summary>
	/// <param name="speed">- нова€ скорость движени€ камеры.</param>
	void setSpeed(float speed);
	/// <summary>
	/// ¬озвращает значение скорости камеры.
	/// </summary>
	/// <returns>Float значение скорости камеры.</returns>
	float getSpeed() const;

	/// <summary>
	/// ”станавливает грацницы, в которых может видеть камера.
	/// </summary>
	/// <param name="borders"> - пр€моугольник, в котором может видеть камера.</param>
	void setBorders(sf::FloatRect borders);

	/// <summary>
	/// ¬озвращает границы, в которых может двигатьс€ камера.
	/// </summary>
	/// <returns>FloatRect, в котором может двигатьс€ камера.</returns>
	sf::FloatRect getBorders() const;

	/// <summary>
	/// ¬озвращает ссылку на основную камеру.
	/// </summary>
	/// <returns>—сылка на объект основной камеры.</returns>
	static GameCamera* getMainCamera();
};

