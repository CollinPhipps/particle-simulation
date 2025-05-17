#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <memory>
#include "Ball.h"

class ParticleSystem
{
private:
	static constexpr int cellSize{ 14 };
	static constexpr int gridWidth{ 800 / cellSize + 1 };
	static constexpr int gridHeight{ 800 / cellSize + 1 };

	sf::CircleShape m_circle{ 400.f, 100 };
	std::vector<std::unique_ptr<Ball>> m_balls;
	sf::Vector2f m_gravity{ 0.f, 1000.f };
	std::array<std::array<std::vector<Ball*>, gridWidth>, gridHeight> m_grid{};

	void collide(Ball& ball1, Ball& ball2);
	void updateGrid();
	void detectCollisions();

public:
	ParticleSystem();

	void addBall(std::unique_ptr<Ball> ball);
	void update(sf::Time dt);
	void draw(sf::RenderWindow& window);
	int getVecSize();
	void applyAttractions(const sf::Vector2f& pos);

	template <typename... Args>
	void emplaceBall(Args&&... args)
	{
		m_balls.emplace_back(std::make_unique<Ball>(std::forward<Args>(args)...));
	}
};

