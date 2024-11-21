#include "GameCamera.hpp"
#include "Constants.hpp"

sf::RenderWindow* GameCamera::_window = nullptr;
GameCamera* GameCamera::_mainCamera = nullptr;

void GameCamera::INIT(sf::RenderWindow* window)
{
	_window = window;
}

sf::Vector2f GameCamera::getMapMousePos()
{
	return (sf::Vector2f)sf::Mouse::getPosition(*_window) + _mainCamera->_position;
}

void GameCamera::moveMainCamera(sf::Vector2f step)
{
	_mainCamera->move(step);
}

void GameCamera::updateView()
{
	_window->setView(_mainCamera->_view);
}

GameCamera::GameCamera()
{
	_position = sf::Vector2f(0,0);
	_isMainCamera = false;

	_view = _window->getDefaultView();
	_view.setCenter(_position + sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
}

GameCamera::GameCamera(sf::Vector2f position)
{
	_position = position;
	_isMainCamera = false;

	_view = _window->getDefaultView();
	_view.setCenter(_position + sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
}

void GameCamera::setAsMain()
{
	if(_mainCamera)
		_mainCamera->_isMainCamera = false;
	_isMainCamera = true;
	_mainCamera = this;
}

void GameCamera::move(sf::Vector2f step)
{
	_position += step*(float)PIXELS_PER_CELL;
	_view.setCenter(_position + sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
}

void GameCamera::setPosition(sf::Vector2f position)
{
	_position = position;
	_view.setCenter(_position + sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
}

sf::Vector2f GameCamera::getPosition() const
{
	return _position;
}

GameCamera* GameCamera::getMainCamera()
{
	return _mainCamera;
}
