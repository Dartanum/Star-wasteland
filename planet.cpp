#include "planet.h"
#include <iostream>

Planet::Planet(float Size, float AngularSpeed, Texture& texture, Vector2f Position, int hp_, float spawnKD_, int Area, Font& font, Texture& icon_)
	: size(Size), angularSpeed(AngularSpeed), position(Position), spawnKD(spawnKD_), hp(hp_), currentHp(hp_), occupiedArea(Area) {
	planet.setRadius(size);
	texture_planet = texture;
	planet.setTexture(&texture_planet);
	planet.setOrigin(Vector2f(size, size));
	planet.setPosition(position);
	planet.setScale(0, 0);
	deathTime = 0;
	icon.setSize(Vector2f(size / 5, size / 5));
	icon.setTexture(&icon_);
	icon.setPosition(planet.getPosition().x - planet.getRadius() / 2, planet.getPosition().y + planet.getRadius());
	hpView.setFont(font);
	hpView.setPosition(icon.getPosition().x + icon.getGlobalBounds().width + 10, icon.getPosition().y);
	born = false;
	beginSpawn = false;
	destroy = false;
	beginDestroy = false;
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

void Planet::Spawn() {
	float currentFrame = spawnClock.getElapsedTime().asMilliseconds() / 16;
	float totalFrame = 60;
	float currentScale;
	int inc = 1;
	for (ptrdiff_t i = 1; i < totalFrame; i += inc) {
		if (currentFrame >= i && currentFrame < i + inc) {
			if (currentFrame == 1) beginSpawn = true;
			else beginSpawn = false;
			currentScale = i / totalFrame;
			planet.setScale(currentScale, currentScale);
		}
		if (currentFrame == totalFrame) born = true;
	}
}

void Planet::Die(float Size, float AngularSpeed, Texture& Texture, Vector2f Position, int hp_, float spawnKD_, int area) {
	size = Size;
	planet.setRadius(size);
	planet.setOrigin(Vector2f(size, size));
	planet.setPosition(Position);
	angularSpeed = AngularSpeed;
	position = Position;
	spawnKD = spawnKD_;
	hp = hp_;
	texture_planet = Texture;
	planet.setTexture(&texture_planet);
	planet.setTextureRect(IntRect(0, 0, texture_planet.getSize().x, texture_planet.getSize().y));
	planet.setScale(0, 0);
	currentHp = hp;
	occupiedArea = area;
	icon.setSize(Vector2f(size / 5, size / 5));
	icon.setPosition(planet.getPosition().x - planet.getRadius() / 2, planet.getPosition().y + planet.getRadius());
	hpView.setPosition(icon.getPosition().x + icon.getGlobalBounds().width + 10, icon.getPosition().y);
	born = false;
	beginSpawn = false;
	destroy = false;
}

void Planet::Destroy(int countRectWidth, int countRectHeight, Vector2f beginPoint, int sizeRect) {
	int currentFrame = dest_anim_clock.getElapsedTime().asMilliseconds() / 16;
	if (currentFrame == 1) beginDestroy = true;
	else beginDestroy = false;
	int anim_speed = 2;
	int diffX = 0;
	int diffY = 0;
	int I = 0;
	int countRect = countRectWidth * countRectHeight;
	if (currentFrame >= countRect * anim_speed) {
		destroy = false;
		return;
	}
	Vector2u sizeTexture = texture_planet.getSize();
	for (ptrdiff_t i = 0; i < countRectHeight * anim_speed; i += anim_speed) {
		if ((i / anim_speed * sizeRect + beginPoint.y) > sizeTexture.y) { //если высота прямоугольника выходит за размер текстуры
			diffY = beginPoint.y + (i / anim_speed * sizeRect) - sizeTexture.y;
		}
		if (i == 0) I = 1;
		else I = i;
		for (ptrdiff_t j = 0; j < countRectWidth * anim_speed; j += anim_speed) {
			if (currentFrame > I*j && currentFrame <= I*j + anim_speed) {
				if ((j / anim_speed * sizeRect + beginPoint.x) > sizeTexture.x) { //если ширина прямоугольника выходит за размер текстуры
					diffX = beginPoint.x + (j / anim_speed * sizeRect) - sizeTexture.x;
				}
				planet.setTextureRect(IntRect(beginPoint.x + (j / anim_speed) * sizeRect, beginPoint.y + (I / anim_speed) * sizeRect, sizeRect - diffX, sizeRect - diffY));
				int diffX = 0;
				return;
			}
		}
		diffY = 0;
	}
}
