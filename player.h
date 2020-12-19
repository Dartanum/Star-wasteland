#ifndef PLAYER_H
#define PLAYER_H
#define PI 3.14159265

#include <SFML/Graphics.hpp>
#include <iosfwd>
#include <iostream>

using namespace sf;

class Player {
public:
	RectangleShape player; //������ ������

	Player() = default;
	Player(Texture& texture, const double& speed, const double& angularSpeed, Vector2u screen, const int& w, const int& h);
	Player(const Player&) = default;

	// ����� �������� ��� �������� ������
	void flyForward(float time, float& currentFrame);
	//��������� ��� �������� �����
	void flyBack(const float& time); 
	//������� ����� �� �����
	void flyLeft(const float& time); 
	//������� ������ �� �����
	void flyRight(const float& time); 
	//������������ ������
	void Move(bool forward); 
	//������������� ������� ������ � �����
	void flyForwardAndBack(); 
	//������������ ������������ ��������� ������� (��� �������)
	void standartCondition();
	Vector2f getPos();
	//�������� ������ ��� ���������
	void Destroy(std::vector<Texture*>& textures);

	Clock dest_anim_clock;
	Sprite dest_sprite;
	bool anim_end;
	Vector2f pos_die;
private:
	//�������� �������� ������
	double speed; 
	//����� �� ����� ��� ����� ��������
	float time; 
	//�������� ��������
	double angularSpeed; 
	//������ ���� �������
	int width; 
	//������ ���� �������
	int heigth; 
	//������ ������
	Vector2u screen; 
};
#endif