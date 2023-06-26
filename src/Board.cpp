#include "Board.h"
#include "Player.h"
#include "Collisions.h"

Board::Board(const sf::Vector2f& playerPosition, const sf::Vector2f& computerPosition, int numOfRaftMen)
    : m_user(nullptr), m_computer(nullptr)
{
    // Create the Board object without initializing m_user and m_computer

    // Create the Players after the Board object is fully constructed
    m_user = std::make_shared<Player>(numOfRaftMen, playerPosition, this);
    m_user->update();
    m_computer = std::make_shared<Computer>(numOfRaftMen, computerPosition, this);
    m_computer->update();
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
	m_user->play(window, event);
    //m_computer->play(window, event);
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