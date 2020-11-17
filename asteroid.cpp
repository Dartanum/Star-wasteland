#include "asteroid.h"

Asteroid::Asteroid(float Size, float AngularSpeed, float Speed, float kd, Vector2f SpawnPosition, int SpawnTime, Texture& texture) :
	size(Size), angularSpeed(AngularSpeed), speed(Speed), spawnPosition(SpawnPosition), spawnTime(SpawnTime), KD(kd) {
	exist = false;
	clock.restart();
	asteroidIsMove = false;
	asteroid.setTexture(texture);
	asteroid.setScale(size, size);
	asteroid.setOrigin(asteroid.getLocalBounds().width / 2, asteroid.getLocalBounds().height / 2);
	asteroid.setPosition(SpawnPosition);
}

void Asteroid::Move(Player& player) {
	Vector2f direction(asteroid.getOrigin());
	Vector2f playerPos = player.getPos();
	if (flag) {
		angleRotation = atan(abs(asteroid.getPosition().x - player.getPos().x) / abs(asteroid.getPosition().y - player.getPos().y)) / PI * 180;
		if (playerPos.y < asteroid.getPosition().y && playerPos.x < asteroid.getPosition().x)
			angleRotation = 180 - angleRotation;
		if (playerPos.y < asteroid.getPosition().y && playerPos.x > asteroid.getPosition().x)
			angleRotation = 180 + angleRotation;
		if (playerPos.y > asteroid.getPosition().y && playerPos.x > asteroid.getPosition().x)
			angleRotation = 360 - angleRotation;
		asteroid.setRotation(angleRotation);
		flag = false;
	}
	asteroid.move(direction.x * sin(-asteroid.getRotation() * PI / 180) * speed / 3000, direction.y * cos(-asteroid.getRotation() * PI / 180) * speed / 3000);
}

void Asteroid::update(Vector2f spawn) {
	flag = true;
	exist = true;
	asteroid.setPosition(spawn.x, spawn.y);
}