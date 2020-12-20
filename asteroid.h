#ifndef ASTEROID_H
#define ASTEROID_H
#include <SFML/Graphics.hpp>
#include <iosfwd>
#include "player.h"
#include "textureLoader.h"

using namespace sf;

class Asteroid {
public:
	Asteroid() = default;
	Asteroid(Asteroid&) = default;
	Asteroid(float Size, double Speed, float kd, Vector2f SpawnPosition, int SpawnTime, Texture* texture);
	//перемещение астероида на игрока
	void Move(Player& player);
	//обновление позиции астероида
	void update(Vector2f spawn, Texture* texture);
	//возврат времени спавна астероида
	int getSpawnTime() { return spawnTime; }
	void setExist(bool e) { exist = e; clock.restart(); }
	bool getExist() { return exist; }
	void setAsteroidMove(bool am) { asteroidIsMove = am; }
	bool& getAsteroidMove() { return asteroidIsMove; }
	int getKD() { return KD; }
	void setSpeed(float newSpeed) { speed = newSpeed; }
	float getSpeed() { return speed; }
	void setKD(float newKD) { KD = newKD; }
	void Destroy(std::vector<Texture*>& textures);
	Sprite dest_sprite;
	RectangleShape asteroid;
	Clock clock;
	Clock dest_anim_clock;
	Vector2f dest_pos;
	bool destroy;
private:
	float size;
	bool flag = true;
	bool exist;
	bool asteroidIsMove;
	int angleRotation;
	double speed;
	float KD;
	int spawnTime;
	Vector2f spawnPosition;
	Vector2f playerPos;
};
#endif
