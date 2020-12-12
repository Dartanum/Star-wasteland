#ifndef WEAPON_H
#define WEAPON_H
#include <SFML/Graphics.hpp>
#include "player.h"

using namespace sf;

class Weapon
{
public: 
	Sprite weapon;
	Clock clock;
	Weapon() = default;
	Weapon(Weapon&) = default;
	Weapon(Texture& texture, float Speed, float kd);
	void Move(Player& player);
	void update();
	bool getIsMove() { return weaponIsMove; }
	void setIsMove(bool newMove) { weaponIsMove = newMove; }
	float getKD() { return KD; }
private:
	bool flag = true;
	float KD;
	float speed;
	bool weaponIsMove;
	Vector2f spawnPosition;
};
#endif

