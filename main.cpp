#include <SFML/Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <ctime>

using namespace sf;


int main()
{
    //ESSENTIAL VARIABLES
    float velo = 0.5f;
    int pipeSpawnTimer = 0;
    float upperVelo = 0.1f;
    bool gameEnd = false;
    int scoreValue = 0;
    int scoreTimer = 0;


    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<> distribut(-250, -60);

    RenderWindow window(VideoMode(800, 500), "Flappy Fly");
    window.setFramerateLimit(60);

    //FONT AND SCORE
    Font scoreFont;
    scoreFont.loadFromFile("fonts/CHERI___.ttf");
    Text score;
    score.setFont(scoreFont);
    score.setCharacterSize(50);
    score.setFillColor(Color::White);
    score.setOutlineColor(Color::Black);
    score.setOutlineThickness(2.f);
    score.setPosition(Vector2f(window.getSize().x / 2, 60.f));



    //PIPES



    RectangleShape pipeUpper;
    RectangleShape pipeLower;
    RectangleShape gap;
    RectangleShape gapEnd;

    pipeUpper.setSize(Vector2f(80.f, 400.f));
    pipeLower.setSize(Vector2f(80.f, 400.f));
    gap.setSize(Vector2f(80.f, 150.f));
    gapEnd.setSize(Vector2f(80.f, 150.f));


    pipeUpper.setFillColor(Color::Green);
    pipeUpper.setOutlineThickness(3.f);
    pipeUpper.setOutlineColor(Color::Black);

    pipeLower.setFillColor(Color::Green);
    pipeLower.setOutlineThickness(3.f);
    pipeLower.setOutlineColor(Color::Black);

    gap.setFillColor(Color::Transparent);
    gapEnd.setFillColor(Color::Transparent);

    std::vector<RectangleShape> upperPipes;
    std::vector<RectangleShape> lowerPipes;
    std::vector<RectangleShape> gaps;
    std::vector<RectangleShape> gapEnds;



    //BACKGROUND
    Texture backgroundTex;
    Sprite background;

    if (!backgroundTex.loadFromFile("textures/background.png"))
        std::cout << "Error loading background";

    background.setTexture(backgroundTex);



    //FLY
    Texture flyTex;
    Sprite fly;

    if (!flyTex.loadFromFile("textures/fly.png"))
        std::cout << "Error loading fly";

    fly.setTexture(flyTex);
    fly.setScale(Vector2f(2.f, 2.f));
    fly.setPosition(Vector2f(200.f, window.getSize().y / 2 - 14));
    fly.setOrigin(Vector2f(0.f, 0.f));

    RectangleShape flyCollision(Vector2f(54.f, 36.f));
    flyCollision.setFillColor(Color::Transparent);
    flyCollision.setPosition(Vector2f(fly.getPosition().x + 10, fly.getPosition().y + 10));
    fly.setOrigin(Vector2f(0.f, 0.f));


    //Scoreboard

    RectangleShape scoreboard(Vector2f(200.f, 60.f));
    scoreboard.setFillColor(Color(209, 211, 85));
    scoreboard.setOutlineColor(Color::Black);
    scoreboard.setOutlineThickness(2.f);
    scoreboard.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 - 30));

    Text endText;
    endText.setFont(scoreFont);
    endText.setCharacterSize(30.f);
    endText.setOutlineColor(Color::Black);
    endText.setOutlineThickness(3.f);
    endText.setPosition(Vector2f(scoreboard.getPosition().x + 5, scoreboard.getPosition().y + 10));




    while (window.isOpen())
    {

        if (fly.getPosition().y < window.getSize().y - 40) {
            if (velo < 8) {
                fly.move(0.f, velo);
                flyCollision.move(0.f, velo);
                if (velo < 4) {
                    fly.rotate(velo);
                    flyCollision.rotate(velo);
                }
                velo += 0.1f;
            }
        }
        else {
            gameEnd = true;
        }


        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();

        }
        score.setString(std::to_string(scoreValue));

        //PIPE GENERATING
        if (pipeSpawnTimer < 70)
            pipeSpawnTimer++;

        //Upper Pipes
        if (pipeSpawnTimer >= 70)
        {
            pipeUpper.setPosition(Vector2f(800.f, distribut(gen)));
            upperPipes.push_back(RectangleShape(pipeUpper));


            gap.setPosition(Vector2f(pipeUpper.getPosition().x, pipeUpper.getPosition().y + pipeUpper.getSize().y));
            gaps.push_back(RectangleShape(gap));

            gapEnd.setPosition(Vector2f(gap.getPosition().x+80, gap.getPosition().y));
            gapEnds.push_back(RectangleShape(gapEnd));

            pipeLower.setPosition(Vector2f(gap.getPosition().x, gap.getPosition().y + gap.getSize().y));
            lowerPipes.push_back(RectangleShape(pipeLower));

            pipeSpawnTimer = 0;
        }


        if (!gameEnd) {
            for (size_t i = 0; i < upperPipes.size(); i++)
            {
                upperPipes[i].move(-5.f, 0.f);

                if (upperPipes[i].getPosition().x < -100)
                    upperPipes.erase(upperPipes.begin() + i);
            }

            for (size_t i = 0; i < gaps.size(); i++)
            {
                gaps[i].move(-5.f, 0.f);

                if (gaps[i].getPosition().y < -100)
                    gaps.erase(gaps.begin() + i);
            }

            for (size_t i = 0; i < gapEnds.size(); i++)
            {
                gapEnds[i].move(-5.f, 0.f);

                if (gapEnds[i].getPosition().y < -100)
                    gaps.erase(gaps.begin() + i);
            }

            for (size_t j = 0; j < lowerPipes.size(); j++)
            {
                lowerPipes[j].move(-5.f, 0.f);

                if (lowerPipes[j].getPosition().x < -100)
                    lowerPipes.erase(lowerPipes.begin() + j);

            }
        }
        else
        {

        }
        //PIPE GENERATING


        //FLY CONTROL


        if (Keyboard::isKeyPressed(Keyboard::Space)) {


            if (!gameEnd && fly.getPosition().y > 0) {
                while (upperVelo >= 0) {

                    fly.move(0.f, -upperVelo);
                    fly.setRotation(-20.f);
                    flyCollision.move(0.f, -upperVelo);
                    flyCollision.setRotation(-20.f);
                    upperVelo -= 0.1f;

                }
                upperVelo = 1.5f;
                velo = 0.5f;

            }
        }


        //COLLISION
        for (size_t i = 0; i < upperPipes.size(); i++)
        {
            if (flyCollision.getGlobalBounds().intersects(upperPipes[i].getGlobalBounds()))
            {
                gameEnd = true;

            }
        }

        for (size_t i = 0; i < lowerPipes.size(); i++)
        {
            if (flyCollision.getGlobalBounds().intersects(lowerPipes[i].getGlobalBounds()))
            {
                gameEnd = true;
            }
        }


        if (scoreTimer < 100)
            scoreTimer++;
        if (!gameEnd) {
            for (size_t i = 0; i < gapEnds.size(); i++)
            {
                if (flyCollision.getGlobalBounds().intersects(gapEnds[i].getGlobalBounds()) && scoreTimer >= 60)
                {

                    scoreValue++;
                    scoreTimer = 0;

                }
            }
        }





        window.clear();

        window.draw(background);
        window.draw(flyCollision);
        window.draw(fly);

        for (size_t i = 0; i < upperPipes.size(); i++)
        {
            window.draw(upperPipes[i]);
        }

        for (size_t i = 0; i < lowerPipes.size(); i++)
        {
            window.draw(lowerPipes[i]);
        }

        for (size_t i = 0; i < gaps.size(); i++)
        {
            window.draw(gaps[i]);
        }

        for (size_t i = 0; i < gapEnds.size(); i++)
        {
            window.draw(gapEnds[i]);
        }

        if (!gameEnd)
            window.draw(score);
        if (gameEnd) {
            window.draw(scoreboard);
            window.draw(endText);
            endText.setString("Your score:" + std::to_string(scoreValue));
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                window.close();
                main();
            }
        }
        window.display();
        window.setKeyRepeatEnabled(false);
    }

    return 0;
}
