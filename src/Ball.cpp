#include "Ball.h"
#include <iostream>
#include <cmath>

Ball::Ball(sf::Vector2f startPos, float radius, sf::Vector2f initialVelocity, float dt, sf::Color color)
	: m_pos(startPos), m_accel({ 0, 0 }), m_mass(radius), m_radius(radius)
{
	m_prevPos = m_pos - initialVelocity * dt;
	m_ballShape.setRadius(radius);
	m_ballShape.setOrigin({ radius, radius });
	m_ballShape.setFillColor(color);
}

void Ball::applyForce(const sf::Vector2f& force)
{
	m_accel += force / m_mass;
}

sf::CircleShape& Ball::getShape()
{
	m_ballShape.setPosition(m_pos);
	return m_ballShape;
}

void Ball::update(float dt)
{
	sf::Vector2f currentPos = m_pos;
	m_pos = m_pos + (m_pos - m_prevPos) + m_accel * (dt * dt);
	m_prevPos = currentPos;
	m_accel = { 0, 0 };
}

void Ball::constrainToBounds()
{
	const sf::Vector2f center{ 400.f, 400.f };
	const float radius = 400.f;
	const sf::Vector2f distToObj = m_pos - center;
	const float distToCenter = std::sqrt(distToObj.x * distToObj.x + distToObj.y * distToObj.y);
	if (distToCenter > radius - getRadius())
	{
		const sf::Vector2f n = distToObj / distToCenter;
		m_pos = center + n * (radius - getRadius());
	}
}

const sf::Vector2f& Ball::getPosition() const
{
	return m_pos;
}

void Ball::addToPosition(const sf::Vector2f& pos)
{
	m_pos += pos;
}

float Ball::getRadius() const
{
	return m_radius;
}