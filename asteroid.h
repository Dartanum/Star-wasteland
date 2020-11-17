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
	Asteroid(float Size, float AngularSpeed, float Speed, float kd, Vector2f SpawnPosition, int SpawnTime, Texture& texture);
	//перемещение астероида на игрока
	void Move(Player& player);
	//обновление позиции астероида
	void update(Vector2f spawn);
	//возврат времени спавна астероида
	int getSpawnTime() { return spawnTime; }
	void setExist(bool e) { exist = e; clock.restart(); }
	bool getExist() { return exist; }
	void setAsteroidMove(bool am) { asteroidIsMove = am; }
	bool& getAsteroidMove() { return asteroidIsMove; }
	int getKD() { return KD; }
	Sprite asteroid;
	Clock clock;
private:
	float size;
	bool flag = true;
	bool exist;
	bool asteroidIsMove;
	int angleRotation;
	float speed;
	float angularSpeed;
	float KD;
	int spawnTime;
	Vector2f spawnPosition;
	Vector2f playerPos;
};
#endif
