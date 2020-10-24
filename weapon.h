#ifndef WEAPON_H
#define WEAPON_H
#include <SFML/Graphics.hpp>
#include "player.h"

using namespace sf;

class Weapon
{
public: 
	Sprite weapon;
	Weapon() = default;
	Weapon(Weapon&) = default;
	Weapon(Texture& texture, float Speed);
	void Move(Player& player);
	void update(Vector2u screen);
private:
	bool flag = true;
	float speed;
	Vector2f spawnPosition;
};
#endif

