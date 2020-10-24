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

	void flyForward(float time, float& currentFrame); // смена анимации при движении вперед
	void flyBack(const float& time); //остановка при движении назад
	void flyLeft(const float& time); //поворот влево на месте
	void flyRight(const float& time); //поворот вправо на месте
	void Move(bool forward); //передвижение игрока
	void flyForwardAndBack(); //одновременное нажатие вперед и назад
	void standartCondition(); //установление стандартного состояния корабля (без пламяни)
	Vector2f getPos();
private:
	int speed; //скорость движения вперед
	float time; //время из мэйна для смены анимации
	double angularSpeed; //скорость вращения
	int width; //ширина тела корабля
	int heigth; //высота тела корабля
	Vector2u screen; //рахмер экрана
};
#endif