#include <SFML/Graphics.hpp>
#include <math.h>
#include "player.h"
#include "textureLoader.h"
#include "planet.h"
#define PI 3.14159265

using namespace sf;

int main(int args, char *argv[])
{
    textureLoader tl;
    Texture texture;

    std::vector<String> paths = { "textures/planet/Orange-Planet.png", "textures/planet/Lava-PLanet.png" };
    String path = "textures/player/player.png";
    tl.loader(Objects::PLAYER, path);
    texture.loadFromFile("textures/planet/Space.png");
    tl.loader(Objects::PLANET, paths);
    tl.background = texture;

    int width = 50;
    int heigth = 75;
    int w = 112;
    int h = 107;
    double speed = 165;
    double angularSpeed = 1;
    float sizePlanet = 25.0f;

    Sprite background;
    background.setTexture(tl.background);

    Sprite sprite;
    sprite.setTexture(tl.players[0]);
    Player player(sprite, speed, angularSpeed, w, h);

    Planet planet1(sizePlanet, 5.0f, tl.planets[0], Vector2f(140.0f, 267.0f));
    Planet planet2(sizePlanet, 9.0f, tl.planets[1], Vector2f(1094.0f, 872.0f));
    ContextSettings settings;
    settings.antialiasingLevel = 32; //установка уровня сглаживания фигур
    RenderWindow window(VideoMode(1000, 1000), "Test shapes", Style::Fullscreen, settings);
    Vector2u screen = window.getSize();
    float currentFrame = 0; //текущий кадр
    Clock clock;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 800;
        player.standartCondition();
        planet1.Rotation();
        planet2.Rotation();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) { 
            player.flyLeft(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player.flyRight(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            player.flyForward(time, currentFrame);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) { 
            player.flyBack(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Down)) {
            player.flyForwardAndBack();
        }
        window.clear();
        window.draw(background);
        window.draw(player.player);
        window.draw(planet1.planet);
        window.draw(planet2.planet);
        window.display();
    }

    return 0;
}
