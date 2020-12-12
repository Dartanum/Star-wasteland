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
	int occupiedArea;
	CircleShape planet;
	RectangleShape icon;
	Texture texture_planet;
	Text hpView;
	Font font;
	Clock spawnClock;
	Clock dest_anim_clock;
	float spawnKD;
	int hp;
	int currentHp;
	bool born;
	int deathTime;
	bool beginSpawn;
	bool destroy;
	bool beginDestroy;
	Planet() = default;
	Planet(const Planet&) = default;
	Planet(float Size, float AngularSpeed, Texture& texture, Vector2f Position, int hp_, float spawnKD_, int Area, Font& font, Texture& icon);

	bool Intersects(RectangleShape shape);
	void Rotation();
	void Spawn();
	void Die(float Size, float AngularSpeed, Texture& texture, Vector2f Position, int hp_, float spawnKD_, int area);
	void Destroy(int width, int height, Vector2f beginPoint, int sizeRect);
};
#endif