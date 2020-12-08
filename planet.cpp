#include "planet.h"
#include <iostream>

Planet::Planet(float Size, float AngularSpeed, Texture& Texture, Vector2f Position) : size(Size), angularSpeed(AngularSpeed), position(Position) {
	planet.setRadius(size);
	planet.setTexture(&Texture);
	planet.setOrigin(Vector2f(size, size));
	planet.setPosition(position);
};

void Planet::Rotation() {
	planet.rotate(angularSpeed / 100.0f);
}

bool Planet::Intersects(RectangleShape shape) {
	float radius = planet.getRadius();
	Vector2f centre = Vector2f(planet.getPosition().x + radius, planet.getPosition().y + radius);
	RectangleShape sector(Vector2f(radius * 2 - 25, 10));
	sector.setOrigin(sector.getLocalBounds().width / 2, sector.getLocalBounds().height / 2);
	sector.setPosition(planet.getPosition().x, planet.getPosition().y);
	for (int i = 0; i < 360; i += 15) {
		sector.setRotation(i);
		if (sector.getGlobalBounds().intersects(shape.getGlobalBounds())) {
			return true;
		}
	}
	return false;
}
