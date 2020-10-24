#include "weapon.h"

Weapon::Weapon(Texture& texture, float Speed) : speed(Speed){
	weapon.setTexture(texture);
	weapon.setOrigin(weapon.getLocalBounds().width / 2, weapon.getLocalBounds().height / 2);
}

void Weapon::Move(Player& player) {
	if (flag) {
		FloatRect playerBounds = player.player.getGlobalBounds();
		spawnPosition = Vector2f(playerBounds.left + playerBounds.width / 2, playerBounds.top + playerBounds.height / 2);
		weapon.setPosition(spawnPosition);
		weapon.setRotation(player.player.getRotation());
		flag = false;
	}
	Vector2f direction(weapon.getOrigin().x - 90, weapon.getOrigin().y - 90);
	weapon.move(direction.x * sin(-weapon.getRotation() * PI / 180) * speed / 3000, direction.y * cos(-weapon.getRotation() * PI / 180) * speed / 3000);
}

void Weapon::update(Vector2u screen) {
	flag = true;
	weapon.setPosition(- 100, - 100);
}
