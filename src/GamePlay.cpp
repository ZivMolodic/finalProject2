#include "GamePlay.h"
#include "Resources.h"
#include "Macros.h"
#include <SFML/audio.hpp>



GamePlay::GamePlay()
{
    m_backGround.setTexture(&Resources::instance().getTexture("gameplay_background"));
    m_backGround.setSize(BACKGROUND_SIZE);
    m_backGround.setPosition({ 0,0 });

    
    m_text.setFont(Resources::instance().getFont());
    m_text.setCharacterSize(50);
    m_text.setPosition(WINDOW_SIZE.x / 2.f, 30);
}

void GamePlay::gameLoop(RenderWindow* window)
{
    Resources::instance().playBackGround();

    auto board = Board(Vector2f{ 220, 400 }, Vector2f{ 1150,400 }, 1);
    //auto player = std::make_shared<Player>(1, Vector2f{ 180, 400 });

    bool playerTurn = true;

    auto view = sf::View(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));

    int timeAsSeconds = 30; 
    
    window->setFramerateLimit(60);
    //openShot(window, player);

    sf::Clock turnTimer;

    while (window->isOpen())
    {
        window->clear();
        window->setView(view);
        window->draw(m_backGround);
        drawTime(timeAsSeconds-turnTimer.getElapsedTime().asSeconds(), *window);
        //update
        board.update();
        board.handleCollisions();
        board.draw(window);
        //player->update();
        //player->draw(window);


        window->display();

        auto event = sf::Event{};
        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
            {
                window->close();
                break;
            }
            case sf::Event::KeyPressed:
            {
               // view = View(sf::FloatRect(window->getView().getCenter().x + 10, 0, 300, 600));
                break;
            }
            case sf::Event::KeyReleased:
                break;
            }
        }
       // view = window->getView();
        /*if(view.getCenter().x + view.getSize().x/2.f < m_backGround.getSize().x &&
            view.getCenter().x + view.getSize().x / 2.f > 0)
            view.move(Vector2f({ 1, 0 }) * 0.15f);*/
        if (turnTimer.getElapsedTime().asSeconds() >= timeAsSeconds)
        {
            playerTurn = !playerTurn;
            turnTimer.restart();
        }


        board.play(window, event);
        //if (playerTurn)
        //{
        //    player->play(window, event);
        //    //playerTurn = false;
        //}
        //else if (player->isPlaying())
        //    player->play(window, event);
        

        //if (board->shooting())
        //{
        //    sf::Vector2f objPosition = board->getObjectilePosition();
        //    if (objPosition.x - view.getSize().x/2.f > 0 && objPosition.x + view.getSize().x/2.f <= m_backGround.getSize().x)
        //        view.setCenter({ objPosition.x, view.getCenter().y });
        //}
        //else
        //    view.setCenter({ view.getSize().x / 2.f , view.getCenter().y });

    }
}


void GamePlay::openShot(RenderWindow* window, std::shared_ptr<Player> player)
{
    auto view = sf::View(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));

    bool firstTime = true;
    
    while (window->isOpen())
    {
        window->clear();
        window->setView(view);
        window->draw(m_backGround);
        //player->draw(window);
        window->display();
        if (firstTime) {
            sleep(sf::seconds(1));
            firstTime = false;
        }
        view = window->getView();
        if (view.getCenter().x + view.getSize().x / 2.f < m_backGround.getSize().x &&
            view.getCenter().x + view.getSize().x / 2.f > 0)
            view.move(Vector2f({ 1, 0 }) * 10.f);
        else {
            sleep(sf::seconds(1));
            break;
        }
    }
}

void GamePlay::drawTime(unsigned int time, sf::RenderWindow& window)
{
    int min = (time / 60), sec = (time % 60);

    m_text.setFillColor(sf::Color::White);
    m_text.setOutlineThickness(4);
    m_text.setOutlineColor(sf::Color::Black);

    if (min < 10 && sec < 10)
        m_text.setString("0" + std::to_string(min) + ":0" + (std::to_string(sec)));
    else if (min < 10)
        m_text.setString("0" + std::to_string(min) + ":" + (std::to_string(sec)));
    else if (sec < 10)
        m_text.setString(std::to_string(min) + ":0" + (std::to_string(sec)));
    else
        m_text.setString(std::to_string(min) + ":" + (std::to_string(sec)));

    if (min == 0 && sec <= 10)
        m_text.setFillColor(sf::Color::Red);

    m_text.setOrigin(m_text.getLocalBounds().width / 2.f,
        m_text.getLocalBounds().height / 2.f);
    window.draw(m_text);
}