#include "PhysicsBehavior.h"
#include <iostream>

PhysicsBehavior::PhysicsBehavior(float bounce, float weight)
: m_weight(weight), m_velocity{ 0,0 }, m_jumping(false), m_stabilty(0),
m_rotate(false), m_bounce(bounce), m_walking(false), m_gravity(true) 
{ 
    if (weight < 0)m_weight = 1; m_timer.restart();
}

void PhysicsBehavior::setVelocity(const Vector2f& velocity)
{
    auto norma = std::sqrtf(norm(velocity));
    if (norma > 40.f)
        m_velocity = (velocity / norma) * 40.f;
    else
        m_velocity = velocity;
}


void PhysicsBehavior::update(Shape* body) 
{   
    if(m_gravity)
        setVelocity({ getVelocity().x, getVelocity().y + GRAVITY * m_weight });

    body->move(m_velocity * (m_timer.restart().asMilliseconds() * 0.08f));

    if(m_rotate)
    {
        auto angle = std::atan2(m_velocity.y, m_velocity.x);
        auto rotation = angle * 180.f / 3.14f;
        body->setRotation(rotation + 90);
    }
}

void PhysicsBehavior::rotate(float angle)
{
    float radians = angle * 3.14159265359 / 180.0;
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    float x = getVelocity().x * cosine - getVelocity().y * sine;
    float y = getVelocity().x * sine + getVelocity().y * cosine;

    setVelocity(sf::Vector2f(x, y));
}

void PhysicsBehavior::handleHit(const Vector2f& surface)
{    
    Vector2f fix = surface / std::sqrtf(norm(surface));
    auto res = m_velocity - 2 * dotProduct(m_velocity, fix) * fix;
    //std::cout << "response: " << res.x << " , " << res.y << std::endl;

    if (std::sqrtf(norm(res)) < 0.1f)
        res = sf::Vector2f();
    
    setVelocity(res * m_bounce);
}


sf::Vector2f CirclePhysics::manageCollision(const sf::Vector2f& position, const RectangleShape& rec)
{
    if (position.y + m_radius >= BACKGROUND_SIZE.y)
    {
        handleHit({ 0,1 });
        return sf::Vector2f(position.x, BACKGROUND_SIZE.y - (m_radius + 1));
    }
    if (position.x + m_radius >= BACKGROUND_SIZE.x)
    {
        handleHit({ 1,0 });
        return sf::Vector2f(BACKGROUND_SIZE.x - (m_radius + 1), position.y);
    }
    if (position.x <= 0)
    {
        handleHit({ 1,0 });
        return sf::Vector2f(1 + m_radius, position.y);
    }
    if (auto response = responseVector(position, getVelocity(), m_radius, rec); response != Vector2f{0,0})
    {
        auto direction = getVelocity();

        handleHit(response);

        auto pos = position;

        auto normDirection = Vector2f{ direction.x / std::sqrtf(norm(direction)), direction.y / std::sqrtf(norm(direction)) };
        while (isPointInRotatedRectangle(pos + normDirection * (m_radius + 0.2f), rec))
            pos -= normDirection * 0.1f;
      
        return(pos);
    }
    return sf::Vector2f(0, 0);
}

Vector2f CirclePhysics::responseVector(const sf::Vector2f& center, const sf::Vector2f& direction, float radius, const RectangleShape& rec)
{
    int count = 0;
    sf::Vector2f response;
    float angleIncrement = 3.14159f / (2 * NUMOFPOINTS);  // Half of the circle

    sf::Vector2f normalizedDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float startingAngle = std::atan2(normalizedDirection.y, normalizedDirection.x);

    for (unsigned int i = 0; i < NUMOFPOINTS / 2; ++i)
    {
        float angle = startingAngle + i * angleIncrement;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        if (isPointInRotatedRectangle(center + sf::Vector2f(x, y), rec))
        {
            response -= Vector2f(x, y);
            count++;
        }
        angle = startingAngle - i * angleIncrement;
        x = radius * std::cos(angle);
        y = radius * std::sin(angle);
        if (isPointInRotatedRectangle(center + sf::Vector2f(x, y), rec))
        {
            response -= Vector2f(x, y);
            count++;
        }
    }


    return response;
}

bool CirclePhysics::isPointInRotatedRectangle(const sf::Vector2f& point, const sf::RectangleShape& rectangle)
{
    if (rectangle.getRotation() == 0)
        return rectangle.getGlobalBounds().contains(point);
 
    sf::FloatRect bounds = rectangle.getGlobalBounds();
    sf::Vector2f center = rectangle.getPosition();
    sf::Vector2f halfSize = rectangle.getSize() * 0.5f;
    float rotation = rectangle.getRotation();

    // Convert the point to local coordinates
    sf::Vector2f localPoint = point - center;
    float angleRad = rotation * 3.14159f / 180.0f;
    float sinRotation = std::sin(angleRad);
    float cosRotation = std::cos(angleRad);
    localPoint = sf::Vector2f(localPoint.x * cosRotation + localPoint.y * sinRotation,
        -localPoint.x * sinRotation + localPoint.y * cosRotation);
    // Check if the point is within the transformed rectangle
    return (localPoint.x >= -halfSize.x && localPoint.x <= halfSize.x &&
        localPoint.y >= -halfSize.y && localPoint.y <= halfSize.y);
}

sf::Vector2f RectanglePhysics::manageCollision(const sf::Vector2f& position, const RectangleShape& rec)
{
    auto rectangle = RectangleShape(m_size);
    rectangle.setOrigin(m_size / 2.f);
    rectangle.setPosition(position);
    if (auto response = AABBResponse(rectangle, rec); response != sf::Vector2f())
    {
        auto direction = getVelocity();
        //static int stable = 0, check = 0;
        //if (std::abs(getVelocity().y) < 2)
        //{
        //    stable++;
        //    if (stable >= 5)
        //    {
        //        check++;
        //        direction = { 0, 1 };
        //    }
        //    if (check >= 5)
        //        stable = check = 0;
        //}
        
        
        if (isWalking())
        {
            if (std::abs(response.x) < 0.01f)
                direction.x = 0;
        }

        handleHit(response);

        direction /= std::sqrtf(norm(direction));
        //std::cout << "response: " << response.x << " , " << response.y << std::endl;

        while (AABBResponse(rectangle, rec) != sf::Vector2f())
            rectangle.move(- direction * 0.1f);

        return rectangle.getPosition();
    }
    if (position.y + m_size.y/2.f >= BACKGROUND_SIZE.y)
    {
        handleHit({ 0,1 });
        return sf::Vector2f(position.x, BACKGROUND_SIZE.y - (m_size.y / 2.f + 1));
    }
    if (position.x + m_size.x / 2.f >= BACKGROUND_SIZE.x)
    {
        handleHit({ 1,0 });
        return sf::Vector2f(BACKGROUND_SIZE.x - (m_size.x / 2.f + 1), position.y);
    }
    if (position.x <= 0)
    {
        handleHit({ 1,0 });
        return sf::Vector2f(m_size.x / 2.f, position.y);
    }
    

    return { 0,0 };
}

sf::Vector2f RectanglePhysics::AABBResponse(const sf::RectangleShape& rec1, const sf::RectangleShape& rec2)
{
    sf::Transform transform = rec1.getTransform();
    sf::FloatRect rect = rec1.getLocalBounds();

    std::vector<sf::Vector2f> vertices1 = {
    transform.transformPoint(sf::Vector2f(rect.left, rect.top)),
    transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top)),
    transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top + rect.height)),
    transform.transformPoint(sf::Vector2f(rect.left, rect.top + rect.height)) };

    transform = rec2.getTransform();
    rect = rec2.getLocalBounds();

    std::vector<sf::Vector2f> vertices2 = {
    transform.transformPoint(sf::Vector2f(rect.left, rect.top)),
    transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top)),
    transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top + rect.height)),
    transform.transformPoint(sf::Vector2f(rect.left, rect.top + rect.height)) };


    std::vector<sf::Vector2f> axes{
        vertices1[1] - vertices1[0], vertices1[2] - vertices1[1],
        vertices2[1] - vertices2[0], vertices2[2] - vertices2[1]
    };

    sf::Vector2f responseVector;
    float minOverlap = std::numeric_limits<float>::max();

    for (const auto& axis : axes)
    {

        float minProjection1 = std::numeric_limits<float>::max();
        float maxProjection1 = std::numeric_limits<float>::lowest();
        for (const auto& vertex : vertices1)
        {
            float projection = dotProduct(vertex, axis);
            minProjection1 = std::min(minProjection1, projection);
            maxProjection1 = std::max(maxProjection1, projection);
        }

        float minProjection2 = std::numeric_limits<float>::max();
        float maxProjection2 = std::numeric_limits<float>::lowest();
        for (const auto& vertex : vertices2)
        {
            float projection = dotProduct(vertex, axis);
            minProjection2 = std::min(minProjection2, projection);
            maxProjection2 = std::max(maxProjection2, projection);
        }

        if (!(maxProjection1 > minProjection2 && minProjection1 < maxProjection2))
            return { 0,0 }; // No collision

        float overlap = std::min(maxProjection1, maxProjection2) - std::max(minProjection1, minProjection2);
        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            responseVector = axis * overlap;
        }
    }

    return responseVector;
}
