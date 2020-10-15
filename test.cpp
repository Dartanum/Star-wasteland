#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

void test() {
    ContextSettings settings;
    settings.antialiasingLevel = 32; //установка уровня сглаживания фигур

    Texture texture; // создание объекта текстуры
    texture.loadFromFile("D:\\Projects\\Project_Qt\\textures\\earth.jpg"); //загрузка текстуры
    texture.setSmooth(true); //установление сглаживания

    Font font; //создание объекта стиля
    font.loadFromFile("D:\\Projects\\Project_Qt\\fonts\\19844.otf"); //установка стиля
    Text text; //создание объекта текста
    text.setFont(font); //применение стиля к тексту
    text.setString("Hello, World!"); //установка текста
    text.setCharacterSize(32); //установка размера текста
    text.setPosition(240, 160);


    RectangleShape shape(Vector2f(120, 50)); //создание прямоугольника со сторонами 120 x 50
    shape.setFillColor(Color::Red); //установка красного цвета для него (чтобы сделать ее прозрачной установить Color::Transparent)
    shape.setOutlineThickness(10); //установка обводки вокруг прямоугольника с толщиной 10
    shape.setOutlineColor(Color::Green); //установка зеленого цвета для обводки
    shape.setPosition(30, 90); //перемещение прямоугольника на 30 и 90 по X и Y соответственно от начала координат
    shape.move(10.0f, 10.0f); //перемещение прямоугольника на 10 по X и Y от текущего положения
    shape.setRotation(30); //поворот прямоугольника на 30 градусов по абсолютной величине
    shape.rotate(15); //поворот прямоугольника на 15 градусов от текущей позиции (в сумме 45)

    CircleShape circle(140); // создание круга
    circle.setTexture(&texture); //установление текстуры
    circle.setOrigin(-200.0f, -200.0f); //установление центра координат

    CircleShape triangle(50, 3); //создание треугольника со сторонами 50 и тремя вершинами
    triangle.setFillColor(Color::White);
    triangle.move(240.0f, 0.0f);

    ConvexShape convex; //создание многоугольника
    convex.setPointCount(5); //установление 5 вершин многоугольника
    convex.setPoint(0, Vector2f(0, 0)); //установка расположения каждой вершины
    convex.setPoint(1, Vector2f(150, 10));
    convex.setPoint(2, Vector2f(120, 90));
    convex.setPoint(3, Vector2f(30, 100));
    convex.setPoint(4, Vector2f(0, 50));
    convex.setFillColor(Color::Cyan);
    convex.move(350.0f, 0.0f);

    RectangleShape line(Vector2f(100, 5)); //создание прямоугольника
    line.rotate(45); // поворот его на 45 градусов
    line.move(60.0f, 60.0f);

    Vertex line2[] = { //создание вертекса (массива точек)
        Vertex(Vector2f(0, 0)),
        Vertex(Vector2f(60, 60))
    };
}