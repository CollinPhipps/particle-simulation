#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Ball
{
private:
	sf::CircleShape m_ballShape;
	sf::Vector2f m_pos;
	sf::Vector2f m_prevPos;
	sf::Vector2f m_accel;
	float m_mass;
	float m_radius;

public:
	Ball(sf::Vector2f startPos, float radius, sf::Vector2f initialVelocity, float dt, sf::Color color);

	sf::CircleShape& getShape();
	void update(float dt);
	void applyForce(const sf::Vector2f& force);
	void constrainToBounds();
	const sf::Vector2f& getPosition() const;
	void addToPosition(const sf::Vector2f& pos);
	float getRadius() const;
};