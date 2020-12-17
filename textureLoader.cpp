#include "textureLoader.h"

void textureLoader::loader(enum Objects obj, std::vector<String>& paths) {
	std::vector<Texture> textures;
	Texture texture;
		for (String i : paths) {
			texture.loadFromFile(i);
			textures.push_back(texture);
		}
		for (Texture i : textures) {
			switch (obj) {
			case Objects::PLAYER:
				players.push_back(i);
				break;
			case Objects::PLANET:
				planets.push_back(i);
				break;
			case Objects::ASTEROID:
				asteroids.push_back(i);
				break;
			case Objects::WEAPON:
				weapons.push_back(i);
				break;
			case Objects::DEST_EFFECT:
				dest_effect.push_back(i);
				break;
			case Objects::UI:
				ui.push_back(i);
				break;
			}					 
		}
}

void textureLoader::loader(enum Objects obj, String& path) {
		Texture texture;
		texture.loadFromFile(path);
		switch (obj) {
		case Objects::PLAYER:
			players.push_back(texture);
			break;
		case Objects::PLANET:
			planets.push_back(texture);
			break;
		case Objects::ASTEROID:
			asteroids.push_back(texture);
			break;
		case Objects::WEAPON:
			weapons.push_back(texture);
			break;
		case Objects::DEST_EFFECT:
			dest_effect.push_back(texture);
			break;
		case Objects::UI:
			ui.push_back(texture);
			break;
		}
}

void textureLoader::loderMenu(std::map<std::string, sf::String>& textures) {
	Texture temp;
	for (auto& item : textures) {
		temp.loadFromFile(item.second);
		menu[item.first] = temp;
	}
}

void textureLoader::loderMenu(sf::String path, std::string name) {
	Texture texture;
	texture.loadFromFile(path);
	menu[name] = texture;
}
