#ifndef ASTEROID_H
#define ASTEROID_H
#include <SFML/Graphics.hpp>
#include <iosfwd>
#include "player.h"

using namespace sf;

class Asteroid {
public:
	Asteroid() = default;
	Asteroid(Asteroid&) = default;
	Asteroid(float Size, float AngularSpeed, float Speed, Vector2f SpawnPosition, Texture& texture);
	void Move(Player& player);
	void update(Vector2u screen);
	Sprite asteroid;
private:
	float size;
	bool flag = true;
	int angleRotation;
	float speed;
	float angularSpeed;
	Vector2f spawnPosition;
	Vector2f playerPos;
};
#endif
