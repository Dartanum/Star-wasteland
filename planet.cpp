#include "planet.h"

Planet::Planet(float Size, float AngularSpeed, Texture& Texture, Vector2f Position) : size(Size), angularSpeed(AngularSpeed), position(Position) {
	planet.setTexture(Texture);
	planet.setScale(size / 100.0f, size / 100.0f);
	planet.setOrigin(Vector2f(Texture.getSize().x / 2.0f, Texture.getSize().y / 2.0f));
	planet.setPosition(position);
};

void Planet::Rotation() {
	planet.rotate(angularSpeed / 100.0f);
}
