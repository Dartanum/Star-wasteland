#include "asteroid.h"

Asteroid::Asteroid(float Size, float Speed, float kd, Vector2f SpawnPosition, int SpawnTime, Texture& texture) :
	size(Size), speed(Speed), spawnPosition(SpawnPosition), spawnTime(SpawnTime), KD(kd) {
	exist = false;
	clock.restart();
	asteroidIsMove = false;
	asteroid.setSize(Vector2f(size, size));
	asteroid.setTexture(&texture);
	asteroid.setOrigin(asteroid.getLocalBounds().width / 2, asteroid.getLocalBounds().height / 2);
	asteroid.setPosition(SpawnPosition);
	destroy = false;
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
	clock.restart();
	asteroid.setPosition(spawn.x, spawn.y);
}

void Asteroid::Destroy(std::vector<Texture>& textures) {
	int currentFrame = dest_anim_clock.getElapsedTime().asMilliseconds() / 16;
	dest_sprite.setPosition(dest_pos);
	int anim_speed = 2;
	for (ptrdiff_t i = 0; i < textures.size() * anim_speed; i+=anim_speed) {
		if (currentFrame >= textures.size() * anim_speed) {
			destroy = false;
			break;
		}
		if (currentFrame > i && currentFrame < i + anim_speed) {
			dest_sprite.setTexture(textures[i/(anim_speed*2)]);
			break;
		}
	}
}