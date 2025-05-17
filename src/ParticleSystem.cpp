#include <cmath>
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	m_circle.setFillColor(sf::Color::White);
}

void ParticleSystem::addBall(std::unique_ptr<Ball> ball)
{
	m_balls.push_back(std::move(ball));
}

void ParticleSystem::update(sf::Time dt)
{
	const int subSteps = 2;
	const float subdt = dt.asSeconds() / subSteps;

	for (int i(subSteps); i--;)
	{
		for (auto& ball : m_balls)
		{
			ball->applyForce(m_gravity);
			ball->update(subdt);
			ball->constrainToBounds();
		}

		updateGrid();
		detectCollisions();
	}
}

void ParticleSystem::draw(sf::RenderWindow& window)
{
	window.draw(m_circle);
	for (auto& ball : m_balls)
		window.draw(ball->getShape());
}

void ParticleSystem::collide(Ball& ball1, Ball& ball2)
{
	const sf::Vector2f collisionAxis = ball1.getPosition() - ball2.getPosition();
	const float dist = std::sqrt(collisionAxis.x * collisionAxis.x + collisionAxis.y * collisionAxis.y);

	if (dist < 1e-6)
		return;

	if (dist < ball1.getRadius() + ball2.getRadius())
	{
		const sf::Vector2f n = collisionAxis / dist;
		const float delta = ball1.getRadius() + ball2.getRadius() - dist;
		ball1.addToPosition(0.5f * delta * n);
		ball2.addToPosition(-0.5f * delta * n);
	}
}

int ParticleSystem::getVecSize()
{
	return m_balls.size();
}

void ParticleSystem::updateGrid()
{
	for (auto& row : m_grid)
	{
		for (auto& cell : row)
		{
			cell.clear();
		}
	}

	for (auto& ball : m_balls)
	{
		int gridX = static_cast<int>(ball->getPosition().x) / cellSize;
		int gridY = static_cast<int>(ball->getPosition().y) / cellSize;

		gridX = std::clamp(gridX, 0, gridWidth - 1);
		gridY = std::clamp(gridY, 0, gridHeight - 1);

		m_grid[gridY][gridX].push_back(ball.get());
	}
}

void ParticleSystem::detectCollisions()
{
	for (int y = 0; y < gridHeight; ++y)
	{
		for (int x = 0; x < gridWidth; ++x)
		{
			for (Ball* ballA : m_grid[y][x])
			{
				for (int offsetY = -1; offsetY <= 1; ++offsetY)
				{
					int neighborY = y + offsetY;
					if (neighborY < 0 || neighborY >= gridHeight)
						continue;

					for (int offsetX = -1; offsetX <= 1; ++offsetX)
					{
						int neighborX = x + offsetX;
						if (neighborX < 0 || neighborX >= gridWidth)
							continue;

						for (Ball* ballB : m_grid[neighborY][neighborX])
						{
							if (ballA == ballB)
								continue;

							collide(*ballA, *ballB);
						}
					}
				}
			}
		}
	}
}

void ParticleSystem::applyAttractions(const sf::Vector2f& mousePos)
{
	const float radius = 100.f;
	const float radiusSquared = radius * radius;
	const float strength = 15000.f; 

	for (auto& ball : m_balls)
	{
		sf::Vector2f delta = mousePos - ball->getPosition();
		float distSquared = delta.x * delta.x + delta.y * delta.y;

		if (distSquared < radiusSquared && distSquared > 1.0f)
		{
			float dist = std::sqrt(distSquared);
			sf::Vector2f direction = delta / dist;
			float forceMag = strength * (1.0f - (dist / radius));

			ball->applyForce(direction * forceMag);
		}
	}
}