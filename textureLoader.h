#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <vector>
#include <SFML/Graphics.hpp>
#include <iosfwd>
#include <iostream>

using namespace sf;

enum Objects{
	PLAYER,
	PLANET,
	ASTEROID,
	WEAPON
};

class textureLoader
{
public:
	std::vector<Texture> players;
	std::vector<Texture> planets;
	std::vector<Texture> asteroids;
	std::vector<Texture> weapons;
	Texture background;

	textureLoader() = default;
	textureLoader(textureLoader&) = default;
	void loader(enum Objects obj, std::vector<String>& paths);
	void loader(enum Objects obj, String& path);
};
#endif
