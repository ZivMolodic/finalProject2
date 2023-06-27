#include "Player.h"

Player::Player(int numOfRaftMen, const sf::Vector2f& position, Board* board)
	: m_playing(false), m_crewSize(numOfRaftMen), m_position(position),
	m_lastButton(NON), m_board(board)
{
	initMenu();
	auto raft = new DownRaft(position);
	m_raft.emplace_back(raft);
	m_board->addObject(raft);

	raft = new DownRaft({ position.x + 160, position.y});
	m_raft.emplace_back(raft);
	m_board->addObject(raft);

	raft = new DownRaft({ position.x + 160*3, position.y });
	m_raft.emplace_back(raft);
	m_board->addObject(raft);

	auto uraft = new UpRaft({ position.x + 160, position.y - 100 });
	m_raft.emplace_back(uraft);
	m_board->addObject(uraft);
	
	initRaftMen();
	//for(int i=0; i < 3; ++i)
	//	for(int j = 0; j < 2; ++j)
	//		m_raft.emplace_back(std::make_shared<RaftBlock>(sf::Vector2f{ BACKGROUND_SIZE.x - i*163, position.y - 350*j }));
	//for (int i = 1; i < 6; ++i)
	//	m_raft.emplace_back(std::make_shared<RaftBlock>(sf::Vector2f{ position.x + i * 163, position.y }));
	//std::unique_ptr<GameObject> p;
	//auto weapon = new Weapon();
	initWeapons();

}

void Player::initWeapons()
{
	m_weapons.emplace_back(std::make_shared<TennisGun>());
	m_weapons.emplace_back(std::make_shared<GrenadeLauncher>());
	m_weapons.emplace_back(std::make_shared<MissileLauncher>());
	m_weapons.emplace_back(std::make_shared<Remote>());
}

void Player::initRaftMen()
{
	for (int i = 0; i < m_crewSize; ++i)
	{
		auto self = this;
		auto raftMan = new RaftMan(self, sf::Vector2f(m_position.x + 30*i, m_position.y - 20));
		m_raftMen.emplace_back(raftMan);
		m_board->addObject(raftMan);
	}
}

void Player::initMenu()
{
	m_menu.emplace_back(make_unique<UpRaftButton>(Vector2f{ 70,80 }));
	m_menu.emplace_back(make_unique<DownRaftButton>(Vector2f{ 70,170 }));
	m_menu.emplace_back(make_unique<TennisButton>(Vector2f{ 70, 260 }));
	m_menu.emplace_back(make_unique<MissileButton>(Vector2f{ 70, 350 }));
	m_menu.emplace_back(make_unique<GrenadeButton>(Vector2f{ 70, 440 }));
	m_menu.emplace_back(make_unique<GuidedMissileButton>(Vector2f{ 70, 530 }));
}


void Player::setPlay()
{ 
	m_playing = true; 
	m_lastButton = Menu::NON; 
	m_explosionPosition = sf::Vector2f();
}

void Player::update()
{
	//m_timer.restart();
	//for (auto& pawn : m_raftMen)
	//{
	//	pawn->update();
	//	//for(const auto& raft: m_raft)
	//	//	pawn.handleCollision(raft->getRec());
	//	//pawn.handleCollision(m_raft[0]->getRec());
	//}
	for (auto& x : m_weapons) 
	{
		x->update();
		if (x->explodes())
			m_explosionPosition = x->getObjectile()->getPosition();
	}

	m_raft.erase(std::remove_if(m_raft.begin(), m_raft.end(), [](const std::unique_ptr<RaftBlock>& raft) {
		return raft->isDead();
		}), m_raft.end());

	m_raftMen.erase(std::remove_if(m_raftMen.begin(), m_raftMen.end(), [](const std::unique_ptr<RaftMan>& man) {
		return man->isDead();
		}), m_raftMen.end());

}

bool Player::isPlaying() const
{
	if(!m_playing)
		return m_timer.getElapsedTime().asSeconds() < 8;

	return true;
}

void Player::done(RaftMan& pawn)
{ 
	m_playing = false; 
	restartTimer();
	// Add an explicit reset to zero
	//sf::Time zeroTime = sf::Time::Zero;
	//m_timer.restart(zeroTime);
}

void Player::restartTimer()
{
	m_timer.restart();
}

void Player::draw(sf::RenderWindow* window)
{
	for (const auto& x : m_menu)
		x->draw(window, Vector2f{ sf::Mouse::getPosition(*window).x * 1.f, sf::Mouse::getPosition(*window).y * 1.f });

	//for (const auto& weapon : m_weapons)
	//	if (weapon->firing())
	//		weapon->draw(window);

	//for (const auto& x : m_raftMen)
	//	x->draw(window);
	//for (const auto& x: m_raftMen)
	//	if(x->)

}

void Player::getWeapon(RaftMan& pawn, enum Menu weapon)
{
	//needs to manage enabling
	std::shared_ptr wp = (m_weapons[weapon]);
	//std::shared_ptr wp = m_weapons.at(0);
	pawn.useWeapon(wp);
}

void Player::play(RenderWindow* window, const sf::Event& event)
{
	//needs to manage internal turns
	//m_playing = true;
	m_raftMen[0]->play(window, event);
}

sf::Vector2f Player::getUserPosition() const
{
	return m_raftMen[rand() % m_raftMen.size()]->getPosition();
}

void Computer::setPlay()
{
	Player::setPlay();
	m_play = false;
	m_initTurn = false;
	m_timer.restart();
}

void Computer::play(RenderWindow* window, const sf::Event& event)
{
	if(m_playing && m_timer.getElapsedTime().asSeconds() > 3)
	{
		if (!m_initTurn)
		{
			//illustrates computer thinking
			//sf::Clock timer;
			//timer.restart();
			//while (timer.getElapsedTime().asSeconds() < 2.f);

			m_initTurn = true;
			m_userPosition = getBoard()->getUserPosition();
			//m_turn = 0;
			m_turn = (m_turn + 1) % m_raftMen.size();
			m_destination = m_raft[rand() % m_raft.size()]->getPosition();
			//m_destination = m_raft[2]->getPosition();
		}
		

		if (std::abs(m_raftMen[m_turn]->getPosition().y + 40 - m_destination.y) > 2 ||
			std::abs(m_raftMen[m_turn]->getPosition().x - m_destination.x) > 2)
			walk(m_destination, window, event);
		else if (rand() % 4 == 0 && m_raft.size() <= 8)
			buildRaft();
		else
			aim(m_userPosition, window, event);

	}
}

void Computer::walk(const sf::Vector2f& destination, RenderWindow* window, const sf::Event& event)
{
	bool goingUp = false;

	if(destination.y < m_raftMen[m_turn]->getPosition().y)
	{
		for (auto& raft : m_raft)
			if (std::abs(raft->getPosition().x - m_raftMen[m_turn]->getPosition().x) <= 60 &&
				raft->getPosition().y < m_raftMen[m_turn]->getPosition().y)
			{
				m_raftMen[m_turn]->play(window, event, Direction::Up);
				goingUp = true;
				break;
			}
	}
	if(!goingUp)
	{
		if (onEdge(m_raftMen[m_turn]->getPosition().x))
		{
			auto position = m_raftMen[m_turn]->getPosition();
			if (auto find = find_if(m_raft.begin(), m_raft.end(),
				[&position](const std::unique_ptr<RaftBlock>& r) { return std::abs(position.x -
					r->getPosition().x) < 5 && position.y > r->getPosition().y; }); find != m_raft.end())
				m_destination = find->get()->getPosition();
			else
				m_destination = sf::Vector2f(m_raftMen[m_turn]->getPosition().x, m_raftMen[m_turn]->getPosition().y + 40);
		}

		else if (m_raftMen[m_turn]->getPosition().x - destination.x < -1)
			m_raftMen[m_turn]->play(window, event, Direction::Right);

		else if (m_raftMen[m_turn]->getPosition().x - destination.x > 1)
			m_raftMen[m_turn]->play(window, event, Direction::Left);
		else
			m_destination = sf::Vector2f(m_raftMen[m_turn]->getPosition().x, m_raftMen[m_turn]->getPosition().y + 40);
	}
}

bool Computer::onEdge(float position) const
{
	//if(m_destination.x > position)
	//{
	//	for (const auto& raft : m_raft)
	//	{
	//		if (std::abs(position + 80 - (raft->getRec().top + raft->getRec().width)) < 4)
	//			if (auto search = find_if(m_raft.begin(), m_raft.end(),
	//				[&raft](const std::unique_ptr<RaftBlock>& r) { return std::abs(raft->getRec().top + raft->getRec().width -
	//					r->getRec().top) < 3; }); search == m_raft.end())
	//				return true;
	//	}
	//}
	if(auto find = find_if(m_raft.begin(), m_raft.end(),
		[position](const std::unique_ptr<RaftBlock>& r) { return std::abs(position -
			r->getPosition().x) < 1; }); find != m_raft.end())
	{
		if (m_destination.x > position)
		{
			if (auto search = find_if(m_raft.begin(), m_raft.end(),
				[position](const std::unique_ptr<RaftBlock>& r) { return std::abs(position + 160 -
					r->getPosition().x) < 1; }); search == m_raft.end())
				return true;

			return false;
		}
		else if (m_destination.x < position)
		{
			if (auto search = find_if(m_raft.begin(), m_raft.end(),
				[position](const std::unique_ptr<RaftBlock>& r) { return std::abs(position - 160 -
					r->getPosition().x) < 1; }); search == m_raft.end())
				return true;

			return false;
		}
	}

	return false;
}
float Computer::calculateVelocity(const sf::Vector2f& target, const sf::Vector2f& position)
{
	float time = (2 * 20) / GRAVITY;
	
	//auto res = (target.x - position.x) / (time*2.3f);
	//return res;
	return (target.x - position.x) / (time * 1.f);
}
void Computer::aim(const sf::Vector2f& target, RenderWindow* window, const sf::Event& event)
{
	//m_raftMen[m_turn]->shoot(calculateDirection(m_raftMen[m_turn]->getPosition(), target, 13.f));
	//m_raftMen[m_turn]->shoot({-11,-11});
	std::vector<Menu> vec{ Menu::TENNIS, Menu::GRENADE , Menu::MISSILE };
	m_raftMen[m_turn]->shoot(sf::Vector2f(1 * calculateVelocity(target, m_raftMen[m_turn]->getPosition()), -20), vec[rand() % vec.size()]);
	m_playing = false;
	m_timer.restart();
}

sf::Vector2f Computer::calculateDirection(const sf::Vector2f& shooterPosition, const sf::Vector2f& targetPosition, float projectileSpeed)
{
	float verticalDisplacement = targetPosition.y - shooterPosition.y;
	
	float timeToHighestPoint = std::sqrt(2 * verticalDisplacement / (GRAVITY*projectileSpeed));

	float timeOfFlight = 2.f * timeToHighestPoint;
	//float timeOfFlight = 2.f * projectileSpeed * std::sin((2 * 3.14f) / 3)/GRAVITY;
	//float timeOfFlight = 60;

	// Calculate the required vertical velocity to counteract gravity and hit the target
	float verticalVelocity = (targetPosition.y - shooterPosition.y) / timeOfFlight - 0.5f * GRAVITY * timeOfFlight;

	// Calculate the horizontal velocity
	float horizontalVelocity = (targetPosition.x - shooterPosition.x) / timeOfFlight;

	// Create and return the initial velocity vector
	sf::Vector2f initialVelocity(horizontalVelocity, verticalVelocity);
	return initialVelocity;
}

Menu Player::buttonPressed(RenderWindow* window, const sf::Event& event)
{
	for (const auto& button : m_menu)
		if (event.type == sf::Event::MouseButtonReleased &&
			event.mouseButton.button == sf::Mouse::Button::Left &&
			button.get()->globalBounds().contains(Vector2f{sf::Mouse::getPosition(*window).x * 1.f, sf::Mouse::getPosition(*window).y * 1.f}))
			return m_lastButton = button->choose();

	return m_lastButton;
}

void Player::raftButtons()
{
	if (m_raft.size() >= MAX_SIZE_RAFTS)
	{
		for (int i = 0; i < m_menu.size(); i++)
		{
			if (dynamic_cast<UpRaftButton*>(m_menu[i].get()) != nullptr)
				m_menu.erase(m_menu.begin() + i);
			else if (dynamic_cast<DownRaftButton*>(m_menu[i].get()) != nullptr)
				m_menu.erase(m_menu.begin() + i);
		}
	}

	else {
		bool exist = false;
		for (int i = 0; i < m_menu.size(); i++)
			if (dynamic_cast<UpRaftButton*>(m_menu[i].get()) != nullptr)
			{
				exist = true;
				break;
			}
		if (!exist)
		{
			m_menu.emplace_back(make_unique<UpRaftButton>(Vector2f{ 70,80 }));
			m_menu.emplace_back(make_unique<DownRaftButton>(Vector2f{ 70,170 }));
		}
	}
}

bool Player::placeRaft(const enum Menu& button, RaftBlock& raftBlock, const Vector2i& cursorLocation)
{
	for (const auto& raft : m_raft)
	{
		Vector2f raftPos = raft->getPosition();
		float top = raft->getRectangle().getGlobalBounds().top;
		float width = raft->getGlobalBounds().width;
		
		if (button == UP_RAFT)
		{
			if (UpRaft(Vector2f{ raftPos.x, top - 70 }).getGlobalBounds().contains(Vector2f(cursorLocation)))
			{
				auto position = sf::Vector2f(raftPos.x, top - 70);
				if (auto search = find_if(m_raft.begin(), m_raft.end(),
					[&position](const std::unique_ptr<RaftBlock>& r)
					{ return (r->getPosition().x == position.x) && (r->getPosition().y == position.y); });
				search == m_raft.end())
			{
					raftBlock.setPosition(Vector2f{ raftPos.x, top - 70 });
				return true;
			}
		}
		}
		
		else if (button == DOWN_RAFT)
		{
			if (dynamic_cast<UpRaft*>(raft.get()) != nullptr)
				return false; 

			if (DownRaft(Vector2f{ raftPos.x + width, raftPos.y }).getGlobalBounds().contains(Vector2f(cursorLocation)))
			{
				auto position = sf::Vector2f(raftPos.x + width, raftPos.y);
				if (auto search = find_if(m_raft.begin(), m_raft.end(),
					[&position](const std::unique_ptr<RaftBlock>& r)
					{ return (r->getPosition().x == position.x) && (r->getPosition().y == position.y); });
					search == m_raft.end())
				{
				raftBlock.setPosition(Vector2f{ raftPos.x + width, raftPos.y });

				return true;
			}
			}

			else if (DownRaft(Vector2f{ raftPos.x - width, raftPos.y }).getGlobalBounds().contains(Vector2f(cursorLocation)))
			{
				auto position = sf::Vector2f(raftPos.x - width, raftPos.y);
				if (auto search = find_if(m_raft.begin(), m_raft.end(),
					[&position](const std::unique_ptr<RaftBlock>& r)
					{ return (r->getPosition().x == position.x) && (r->getPosition().y == position.y); });
					search == m_raft.end())
				{
				raftBlock.setPosition(Vector2f{ raftPos.x - width, raftPos.y });

				return true;
			}
		}
	}
	}
	
	return false;
}

void Player::addRaft(RaftMan& pawn, const enum Menu& button)
{
	for (const auto& raft : m_raft)
		if (raft.get()->getPosition() == pawn.getRaftBlock()->getPosition())
			return;
	
	m_playing = false;

	if (button == UP_RAFT)
	{
		auto raft = new UpRaft(pawn.getRaftBlock()->getPosition());
		m_raft.emplace_back(raft);
		m_board->addObject(raft);
	}

	else if (button == DOWN_RAFT)
	{
		auto raft = new DownRaft(pawn.getRaftBlock()->getPosition());
		m_raft.emplace_back(raft);
		m_board->addObject(raft);
	}

	m_lastButton = NON;
}


void Computer::buildRaft()
{
	while (true)
	{
		int index = rand() % m_raft.size();
		if (dynamic_cast<UpRaft*>(m_raft[index].get()) != nullptr)
			continue;

		int raftType = rand() % 3;
		RaftBlock* raft = nullptr;
		switch (raftType)
		{
		case 1:
		{
			raft = new UpRaft(Vector2f{ m_raft[index]->getPosition().x, m_raft[index]->getPosition().y - 60 });
			break;
		}
		case 2:
		{
			raft = new DownRaft(Vector2f{ m_raft[index]->getPosition().x + m_raft[index]->getGlobalBounds().width, m_raft[index]->getPosition().y });
			break;
		}
		case 0:
		{
			raft = new DownRaft(Vector2f{ m_raft[index]->getPosition().x - m_raft[index]->getGlobalBounds().width, m_raft[index]->getPosition().y });
			break;
		}
		}

		bool exist = false;
		for (const auto& x : m_raft)
			if (raft != nullptr && x.get()->getPosition() == raft->getPosition())
			{
				exist = true;
				break;
			}
		if (!exist)
		{
			m_raft.emplace_back(raft);
			getBoard()->addObject(raft);
			break;
		}
	}
	m_playing = false;
}

