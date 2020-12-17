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
	WEAPON,
	DEST_EFFECT,
	UI
};

class textureLoader
{
public:
	std::vector<Texture> players;
	std::vector<Texture> planets;
	std::vector<Texture> asteroids;
	std::vector<Texture> weapons;
	std::vector<Texture> dest_effect;
	std::vector<Texture> ui;
	std::map<std::string, sf::Texture> menu;
	Texture background;

	textureLoader() = default;
	textureLoader(textureLoader&) = default;
	void loader(enum Objects obj, std::vector<String>& paths);
	void loderMenu(std::map<std::string, sf::String>& textures);
	void loader(enum Objects obj, String& path);
	void loderMenu(sf::String path, std::string name);
};
#endif
