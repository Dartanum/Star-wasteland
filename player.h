#ifndef PLAYER_H
#define PLAYER_H
#define PI 3.14159265

#include <SFML/Graphics.hpp>
#include <iosfwd>
#include <iostream>

using namespace sf;

class Player {
public:
	int speed;
	std::vector<Texture> flames; //�������� �������
	Sprite player;
	float time;
	std::vector<Texture> playerTextures;
	std::vector<Sprite> playerSprites;
	double angularSpeed; //�������� ��������
	int width; //������ ���� �������
	int heigth; //������ ���� �������

	Player() = default;
	Player(const std::vector<Texture>& players, const int& speed, const double& angularSpeed, const int& w, const int& h);
	Player(Sprite sprite, const int& speed, const double& angularSpeed, const int& w, const int& h);
	Player(const Player&) = default;

	void flyForward(float time, float& currentFrame);
	void flyBack(const float& time);
	void flyLeft(const float& time);
	void flyRight(const float& time);
	void Move(bool forward);
	void flyForwardAndBack();
	void standartCondition();
};
#endif