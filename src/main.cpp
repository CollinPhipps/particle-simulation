#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <iostream>
#include "Ball.h"
#include "ParticleSystem.h"

namespace Interactions
{
    void applyAttractions(const sf::RenderWindow& window, ParticleSystem& ps)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            ps.applyAttractions(pos);
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Particles");

    std::unique_ptr<ParticleSystem> ps = std::make_unique<ParticleSystem>();

    sf::Clock clock;
    float timer = 0.f;
    const float creationInterval = 0.01f;
    float time = 0.f;

    sf::Clock fpsTimer;
    int frameCount = 0;

    const int maxParticles = 2000;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Time dt = clock.restart();
        time += dt.asSeconds();
        timer += dt.asSeconds();

        while (timer >= creationInterval && ps->getVecSize() < maxParticles)
        {
            timer -= creationInterval;            
            
            float initialVelocityX = std::sin(time * 5.f) * 250.f;
            float r = std::sin(time * 5.f) * 255.f;
            float g = std::sin(time * 2.f) * 255.f;
            float b = std::sin(time * 3.f) * 255.f;
            int radius = 5.f;

            ps->emplaceBall(
                sf::Vector2f{ 400.f, 200.f },
                radius,
                sf::Vector2f{ initialVelocityX, 250.f },
                dt.asSeconds(),
                sf::Color(r, g, b)
            );
        }

        ps->update(dt);
        window.clear();
        ps->draw(window);
        window.display();

        Interactions::applyAttractions(window, *ps);

        frameCount++;
        if (fpsTimer.getElapsedTime().asSeconds() >= 1.f)
        {
            float fps = frameCount / fpsTimer.getElapsedTime().asSeconds();
            std::cout << fps << " fps\n";
            frameCount = 0;
            fpsTimer.restart();
        }
    }

    return 0;
}