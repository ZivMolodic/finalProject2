#include "Board.h"
#include "Player.h"
#include "Collisions.h"

Board::Board(const sf::Vector2f& playerPosition, const sf::Vector2f& computerPosition, int numOfRaftMen)
    : m_user(nullptr), m_computer(nullptr), m_userTurn(true),
    m_userPosition(playerPosition), m_computerPosition(computerPosition)
{
    // Create the Board object without initializing m_user and m_computer

    // Create the Players after the Board object is fully constructed
    m_user = std::make_shared<Player>(numOfRaftMen, playerPosition, this);
    m_user->update();
    m_user->setPlay();
    m_computer = std::make_shared<Computer>(numOfRaftMen, computerPosition, this);
    m_computer->update();
}

bool Board::isPlaying()
{
    return !m_computer->isDead() && !m_user->isDead();
}

sf::Vector2f Board::getViewPosition() const
{
    if (m_userTurn && m_user->getExplosionPosition() != sf::Vector2f())
        return m_user->getExplosionPosition();

    if (!m_userTurn && m_computer->getExplosionPosition() != sf::Vector2f())
        return m_computer->getExplosionPosition();

    if (shooting())
        return getObjectilePosition();

    if (m_userTurn) 
        return m_userPosition; 

    return m_computerPosition; 
}

sf::Vector2f Board::getUserPosition() const
{ 
    return m_user->getUserPosition(); 
}

void Board::addObject(GameObject* object)
{
	//if(auto search = std::find(m_objects.begin(), m_objects.end(), *object); search == m_objects.end())
		m_objects.push_back(object);
    //m_objects.push_back(std::unique_ptr<GameObject>(object, deleteGameObject));
}

void Board::update()
{
    m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](GameObject* obj) {
        return obj->isDead();
        }), m_objects.end());
    
    m_user->update();
    m_computer->update();

    for (auto& object : m_objects) 
        object->update(); 
}

void Board::draw(RenderWindow* window) 
{
	for (auto& object : m_objects)
        if (std::string(typeid(*object).name()) != std::string("class RaftMan"))
            object->draw(window);

    for (auto& object : m_objects)
        if (std::string(typeid(*object).name()) == std::string("class RaftMan"))
            object->draw(window);

    m_user->draw(window);
}
void Board::play(RenderWindow* window, const sf::Event& event) 
{
    static bool x = false;
    if (m_userTurn)
    {
        if (x)
            x = x;
        m_user->play(window, event);
        if (!m_user->isPlaying())
        {
            m_userTurn = false;
            m_computer->setPlay();
        }
    }
    else 
    {
        x = true;
        m_computer->play(window, event);
        if (!m_computer->isPlaying())
        {
            m_userTurn = true;
            m_user->setPlay();
        }
    }
    //m_user->play(window, event);
}

void Board::handleCollisions()
{
    for (auto it1 = m_objects.begin(); it1 != m_objects.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != m_objects.end(); ++it2) {
            if ((*it1)->getRec().intersects((*it2)->getRec()))
                processCollision(**it1, **it2);

        }
    }

    if (m_user->shooting())
    {
        for (auto object : m_objects)
            if (m_user->getObjectile()->getRec().intersects(object->getRec()))
                processCollision(*(m_user->getObjectile()), *object);
    }

    if (m_computer->shooting())
    {
        for (auto object : m_objects)
            if (m_computer->getObjectile()->getRec().intersects(object->getRec()))
                processCollision(*(m_computer->getObjectile()), *object);
    }


}


bool Board::shooting() const
{ 
    return m_user->shooting() || m_computer->shooting();
}

sf::Vector2f Board::getObjectilePosition() const
{
    if (m_user->shooting()) 
        return m_user->getObjectilePosition(); 
    
    if (m_computer->shooting()) 
        return m_computer->getObjectilePosition();
}