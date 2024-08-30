#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#define PI 3.14159265359

//**Init func */
void init();
//**Draw func */
void draw(sf::RenderWindow &window);
//**Global Vars
const int size = 15;
//**Array that holds the points
int extraH = 4;
int extraT = 4;
int headPortions = 90 / extraH;
int tailPortions = 90 / extraT;

sf::CircleShape snake[size];
sf::VertexArray bodyLinesL(sf::LineStrip, size + extraH + extraT);
sf::VertexArray bodyLinesR(sf::LineStrip, size + extraH + extraT);

float radius[size] = {27, 30, 25, 24, 23, 23, 22, 22, 21, 20, 19, 18, 16, 14, 11};
//**Distance constraint
float DIST = 35;
//** Font & text
sf::Font font;
sf::Text text;

int main(int argc, char const *argv[])
{
    //** Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Math baby!");
    //** Set Max FPS*/
    window.setFramerateLimit(60);

    init();
    while (window.isOpen())
    {

        sf::Event event;
        //** Events
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            snake[0].move(-5.f, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            snake[0].move(0.f, -5.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            snake[0].move(5.f, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            snake[0].move(0.f, 5.f);
        }
        //** Clear
        window.clear(sf::Color::Black);

        //** Draw shit!
        draw(window);

        //**Display frame
        window.display();
    }

    return 0;
}

void draw(sf::RenderWindow &window)
{
    std::string distanceStr = "";
    int side_points = size * 2;
    // Se if points are breaking constranints

    for (size_t i = 1; i < size; i++)
    {
        int point1x, point1y, point2x, point2y;
        point1x = snake[i - 1].getPosition().x;
        point1y = snake[i - 1].getPosition().y;
        point2x = snake[i].getPosition().x;
        point2y = snake[i].getPosition().y;

        sf::Vector2f distanceXY;
        distanceXY.x = point1x - point2x;
        distanceXY.y = point1y - point2y;
        bodyLinesL[0].color = sf::Color::Red;
        bodyLinesR[0].color = sf::Color::Red;

        // ! Actual Distance
        float diference = sqrt(pow(distanceXY.x, 2) + pow(distanceXY.y, 2));

        int newX, newY;
        // Get angle for the new position
        double angle = atan2(distanceXY.x, distanceXY.y);

        newX = DIST * sin(angle);
        newY = DIST * cos(angle);

        double angle_m90 = angle - 90 * (PI / 180);
        double angle_p90 = angle + 90 * (PI / 180);

        if (i == 1)
        {
            // Body "I + LineArrayOffset(start body segments at [2]) - 1(Starting Body Pos)"
            bodyLinesL[i + extraH - 1].position.x = snake[i - 1].getPosition().x + (radius[i - 1] * sin(angle_m90));
            bodyLinesL[i + extraH - 1].position.y = snake[i - 1].getPosition().y + (radius[i - 1] * cos(angle_m90));
            bodyLinesR[i + extraH - 1].position.x = snake[i - 1].getPosition().x + (radius[i - 1] * sin(angle_p90));
            bodyLinesR[i + extraH - 1].position.y = snake[i - 1].getPosition().y + (radius[i - 1] * cos(angle_p90));

            // Head
            for (size_t j = 0; j < extraH; j++)
            {
                bodyLinesL[j].position.x = snake[i - 1].getPosition().x + (radius[i - 1] * sin(angle - headPortions * j * (PI / 180)));
                bodyLinesL[j].position.y = snake[i - 1].getPosition().y + (radius[i - 1] * cos(angle - headPortions * j * (PI / 180)));
                bodyLinesR[j].position.x = snake[i - 1].getPosition().x + (radius[i - 1] * sin(angle + headPortions * j * (PI / 180)));
                bodyLinesR[j].position.y = snake[i - 1].getPosition().y + (radius[i - 1] * cos(angle + headPortions * j * (PI / 180)));
            }
        }
        if (i == size - 1)
        {
            // Tail
            for (size_t j = 0; j < extraT; j++)
            {                
                bodyLinesL[i + extraH + j + 1].position.x = snake[i].getPosition().x + (radius[i] * sin(angle_m90 - tailPortions * (j+1) * (PI / 180)));
                bodyLinesL[i + extraH + j + 1].position.y = snake[i].getPosition().y + (radius[i] * cos(angle_m90 - tailPortions * (j+1) * (PI / 180)));
                bodyLinesR[i + extraH + j + 1].position.x = snake[i].getPosition().x + (radius[i] * sin(angle_p90 + tailPortions * (j+1) * (PI / 180)));
                bodyLinesR[i + extraH + j + 1].position.y = snake[i].getPosition().y + (radius[i] * cos(angle_p90 + tailPortions * (j+1) * (PI / 180)));
            }
        }

        //bodyLinesL[i + extraH].color = sf::Color::Red;
        bodyLinesL[i + extraH].position.x = snake[i].getPosition().x + (radius[i] * sin(angle_m90));
        bodyLinesL[i + extraH].position.y = snake[i].getPosition().y + (radius[i] * cos(angle_m90));

        //bodyLinesR[i + extraH].color = sf::Color::Red;
        bodyLinesR[i + extraH].position.x = snake[i].getPosition().x + (radius[i] * sin(angle_p90));
        bodyLinesR[i + extraH].position.y = snake[i].getPosition().y + (radius[i] * cos(angle_p90));

        //** Calculate positions
        if (diference > DIST)
        {
            snake[i].setPosition(snake[i - 1].getPosition().x - newX, snake[i - 1].getPosition().y - newY);
            if (i == 1)
            {
                text.setString(distanceStr);
                window.draw(text);
            }
        }
    }

    sf::VertexArray snakeLines(sf::LineStrip, size);
    // Draw snake and lines
    for (size_t i = 0; i < size; i++)
    {
        snakeLines[i].position = snake[i].getPosition();
        window.draw(snake[i]);
    }
    //window.draw(snakeLines);
    window.draw(bodyLinesL);
    window.draw(bodyLinesR);
}

void init()
{
    font.loadFromFile("assets/font.ttf");
    text.setFont(font);

    for (size_t i = 0; i < size; i++)
    {
        snake[i].setFillColor(sf::Color::Black);
        snake[i].setRadius(radius[i]);
        snake[i].setOrigin(snake[i].getPosition().x + radius[i], snake[i].getPosition().y + radius[i]);
        snake[i].setPosition(800 / 3 + i * 70, 600 / 2);
    }
}