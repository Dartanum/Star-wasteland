#ifndef PLAYER_H
#define PLAYER_H
#define PI 3.14159265

#include <SFML/Graphics.hpp>
#include <iosfwd>
#include <iostream>

using namespace sf;

class Player {
public:
	Sprite player; //спрайт игрока

	Player() = default;
	Player(Sprite sprite, const int& speed, const double& angularSpeed, Vector2u screen, const int& w, const int& h);
	Player(const Player&) = default;

	// смена анимации при движении вперед
	void flyForward(float time, float& currentFrame);
	//остановка при движении назад
	void flyBack(const float& time); 
	//поворот влево на месте
	void flyLeft(const float& time); 
	//поворот вправо на месте
	void flyRight(const float& time); 
	//передвижение игрока
	void Move(bool forward); 
	//одновременное нажатие вперед и назад
	void flyForwardAndBack(); 
	//установление стандартного состояния корабля (без пламяни)
	void standartCondition();
	Vector2f getPos();
private:
	//скорость движения вперед
	int speed; 
	//время из мэйна для смены анимации
	float time; 
	//скорость вращения
	double angularSpeed; 
	//ширина тела корабля
	int width; 
	//высота тела корабля
	int heigth; 
	//размер экрана
	Vector2u screen; 
};
#endif