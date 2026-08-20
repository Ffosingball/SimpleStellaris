#include "SFMLApplication.h"
#include <iostream>
#include <filesystem>

#include <SFML/Graphics.hpp>

namespace gel
{
	void SFMLApplication::Run(const std::string& title)
	{
        //SET IN PART SCREEN
        //sf::Vector2u windowSize(1200, 800);
        //sf::VideoMode videoMode(windowSize);
        //auto renderWindow = sf::RenderWindow(videoMode, title.c_str());
        
        //SET IN FULLSCREEN
        //Get all videomodes
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        //first one is usually the best option
        auto renderWindow = sf::RenderWindow(modes[0], title.c_str(), sf::State::Fullscreen);
        
        //Stabilize fps
        renderWindow.setVerticalSyncEnabled(true);
        //Hide mouse
        renderWindow.setMouseCursorVisible(false);

        // Initialisation logic
		Init(renderWindow);

        //Create a clock to get deltaTime
        sf::Clock clock;

        //Debug timer
        //DEB: sf::Clock timer;

        //Main game loop
        while (renderWindow.isOpen())
        {
            //get deltaTime
            float dt = clock.restart().asSeconds();

            //DEB: std::cout << " -Display: " << timer.restart().asSeconds() << '\n';
            //DEB: std::cout << "Next frame: " << '\n';

            // Process events
            while (const std::optional event = renderWindow.pollEvent())
            {
                //If window is closed, then close the game
                if (event->is<sf::Event::Closed>())
                    renderWindow.close();

                // Event handling code here
                HandleEvent(event);
            }
            //DEB: std::cout << " -Handle events: " << timer.restart().asSeconds() << '\n';

            // Update logic
            Update(dt, renderWindow);
            //DEB: std::cout << " -Update: " << timer.restart().asSeconds() << '\n';

            // Clear window
            renderWindow.clear();
            //DEB: std::cout << " -Clear: " << timer.restart().asSeconds() << '\n';

            //Draw to the window
            Render(renderWindow);
            //DEB: std::cout << " -Render: " << timer.restart().asSeconds() << '\n';

            //Display image from the double buffer
            renderWindow.display();
        }
	}


}