#include "Animation.h"

#include "Resources.h"

const auto AnimationTime = sf::seconds(0.3f);

Animation::Animation(const AnimationData& data, DirectionA dir, sf::Sprite& sprite, const std::string& str)
    : m_data(data), m_dir(dir), m_sprite(sprite)
{
    m_sprite.setTexture(Resources::instance().getTexture(str));
    update();
}

void Animation::direction(DirectionA dir)
{
    if (m_dir == dir || dir == DirectionA::Stay)
    {
        return;
    }

    m_dir = dir;
    update();
}

void Animation::update(sf::Time delta)
{
    m_elapsed += delta;
    if (m_elapsed >= AnimationTime)
    {
        m_elapsed -= AnimationTime;
        ++m_index;
        m_index %= m_data.m_data.find(m_dir)->second.size();
        update();
    }
}

void Animation::update()
{
    m_sprite.setTextureRect(m_data.m_data.find(m_dir)->second[m_index]);
}
