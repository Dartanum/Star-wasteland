#ifndef PLANET_H
#define PLANET_H
#include <SFML/Graphics.hpp>
#include <iosfwd>

using namespace sf;

class Planet {
public:
	float size;
	float angularSpeed;
	Vector2f position;
	CircleShape planet;
	Planet() = default;
	Planet(const Planet&) = default;
	Planet(float Size, float AngularSpeed, Texture& Texture, Vector2f Position);

	bool Intersects(RectangleShape shape);
	void Rotation();
};
#endif