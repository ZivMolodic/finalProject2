#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>
#include "Board.h"
#include "RaftBlock.h"
#include "RaftMan.h"
#include "Weapon.h"
#include "Button.h"

using namespace std;
class RaftMan;

class Player
{
public:
    Player(int numOfRaftMen, const sf::Vector2f& position, Board* board);
    virtual ~Player() = default;
    void update();
    void draw(sf::RenderWindow* window);
    void raftButtons();
    void addRaft(RaftMan& pawn, const enum Menu& button);
    bool placeRaft(const enum Menu& button, RaftBlock& raftBlock, const Vector2i& cursorLocation);
    Menu buttonPressed(RenderWindow* window, const sf::Event& event);
    void getWeapon(RaftMan& pawn, enum Menu weapon);
    std::shared_ptr<GameObject> getObjectile() { for (const auto& x : m_weapons) if (x->firing())return x->getObjectile(); }
    void done(RaftMan& pawn);
    virtual void play(RenderWindow* window, const sf::Event& event);
    bool isPlaying() const;
    void resetButton() { m_lastButton = Menu::NON; }
    virtual void setPlay();
    bool shooting() { auto search = find_if(m_weapons.begin(), m_weapons.end(), [](std::shared_ptr<Weapon> w) { return w->firing(); }); return search != m_weapons.end(); }
    sf::Vector2f getObjectilePosition()
    {
        auto search = find_if(m_weapons.begin(), m_weapons.end(), [](std::shared_ptr<Weapon> w) { return w->firing(); });
        if (search != m_weapons.end())
            return search->get()->getObjectilePosition();

    }
    sf::Vector2f getUserPosition() const;
    sf::Vector2f getExplosionPosition() const { return m_explosionPosition; }
    bool isDead() { return m_raftMen.size() <= 0; }
protected:
    Board* getBoard() { return m_board; }
    vector<std::unique_ptr<RaftBlock>> m_raft;
    vector<std::unique_ptr<RaftMan>> m_raftMen;
    void restartTimer();
    sf::Clock m_timer;
    bool m_playing;
private:
    Board* m_board;
    void initRaftMen();
    void initWeapons();
    void initMenu();
    vector<std::shared_ptr<Weapon>> m_weapons;

    int m_crewSize;
    sf::Vector2f m_position;
    sf::Vector2f m_explosionPosition;
    std::vector<std::unique_ptr<GameMenuButton>> m_menu;
    enum Menu m_lastButton;
};


class Computer : public Player
{
public:
    Computer(int numOfRaftMen, const sf::Vector2f& position, Board* board)
        : Player(numOfRaftMen, position, board), m_turn(0), m_play(false), m_initTurn(false) {}
    void play(RenderWindow* window, const sf::Event& event) override;
    void setPlay() override;

private:
    void buildRaft();
    void walk(const sf::Vector2f& destination, RenderWindow* window, const sf::Event& event);
    void aim(const sf::Vector2f& target, RenderWindow* window, const sf::Event& event);
    sf::Vector2f calculateDirection(const sf::Vector2f& shooterPosition, const sf::Vector2f& targetPosition, float projectileSpeed);
    float calculateVelocity(const sf::Vector2f& target, const sf::Vector2f& position);
    bool onEdge(float position) const;
    int m_turn;
    sf::Vector2f m_userPosition;
    sf::Vector2f m_destination;
    bool m_play;
    bool m_initTurn;
};