#include "asteroid.h"

Asteroid::Asteroid(float Size, float AngularSpeed, float Speed, Vector2f SpawnPosition, Texture& texture) :
	size(Size), angularSpeed(AngularSpeed), speed(Speed), spawnPosition(SpawnPosition) {
	asteroid.setTexture(texture);
	asteroid.setScale(size, size);
	asteroid.setOrigin(asteroid.getLocalBounds().width / 2, asteroid.getLocalBounds().height / 2);
	asteroid.setPosition(SpawnPosition);
}

void Asteroid::Move(Player& player) {
	Vector2f direction(asteroid.getOrigin());
	if (flag) {
		angleRotation = 180 - atan(abs(asteroid.getPosition().x - player.getPos().x) / abs(asteroid.getPosition().y - player.getPos().y)) / PI * 180;
		asteroid.setRotation(angleRotation);
		flag = false;
	}
	asteroid.move(direction.x * sin(-asteroid.getRotation() * PI / 180) * speed / 3000, direction.y * cos(-asteroid.getRotation() * PI / 180) * speed / 3000);
}

void Asteroid::update(Vector2u screen) {
	flag = true;
	asteroid.setPosition(screen.x + asteroid.getTexture()->getSize().x, screen.y + asteroid.getTexture()->getSize().y);
}