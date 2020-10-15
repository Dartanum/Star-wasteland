#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

void test() {
    ContextSettings settings;
    settings.antialiasingLevel = 32; //��������� ������ ����������� �����

    Texture texture; // �������� ������� ��������
    texture.loadFromFile("D:\\Projects\\Project_Qt\\textures\\earth.jpg"); //�������� ��������
    texture.setSmooth(true); //������������ �����������

    Font font; //�������� ������� �����
    font.loadFromFile("D:\\Projects\\Project_Qt\\fonts\\19844.otf"); //��������� �����
    Text text; //�������� ������� ������
    text.setFont(font); //���������� ����� � ������
    text.setString("Hello, World!"); //��������� ������
    text.setCharacterSize(32); //��������� ������� ������
    text.setPosition(240, 160);


    RectangleShape shape(Vector2f(120, 50)); //�������� �������������� �� ��������� 120 x 50
    shape.setFillColor(Color::Red); //��������� �������� ����� ��� ���� (����� ������� �� ���������� ���������� Color::Transparent)
    shape.setOutlineThickness(10); //��������� ������� ������ �������������� � �������� 10
    shape.setOutlineColor(Color::Green); //��������� �������� ����� ��� �������
    shape.setPosition(30, 90); //����������� �������������� �� 30 � 90 �� X � Y �������������� �� ������ ���������
    shape.move(10.0f, 10.0f); //����������� �������������� �� 10 �� X � Y �� �������� ���������
    shape.setRotation(30); //������� �������������� �� 30 �������� �� ���������� ��������
    shape.rotate(15); //������� �������������� �� 15 �������� �� ������� ������� (� ����� 45)

    CircleShape circle(140); // �������� �����
    circle.setTexture(&texture); //������������ ��������
    circle.setOrigin(-200.0f, -200.0f); //������������ ������ ���������

    CircleShape triangle(50, 3); //�������� ������������ �� ��������� 50 � ����� ���������
    triangle.setFillColor(Color::White);
    triangle.move(240.0f, 0.0f);

    ConvexShape convex; //�������� ��������������
    convex.setPointCount(5); //������������ 5 ������ ��������������
    convex.setPoint(0, Vector2f(0, 0)); //��������� ������������ ������ �������
    convex.setPoint(1, Vector2f(150, 10));
    convex.setPoint(2, Vector2f(120, 90));
    convex.setPoint(3, Vector2f(30, 100));
    convex.setPoint(4, Vector2f(0, 50));
    convex.setFillColor(Color::Cyan);
    convex.move(350.0f, 0.0f);

    RectangleShape line(Vector2f(100, 5)); //�������� ��������������
    line.rotate(45); // ������� ��� �� 45 ��������
    line.move(60.0f, 60.0f);

    Vertex line2[] = { //�������� �������� (������� �����)
        Vertex(Vector2f(0, 0)),
        Vertex(Vector2f(60, 60))
    };
}