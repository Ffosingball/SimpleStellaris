#include "SFMLApplication.h"
#include <iostream>
#include <filesystem>

#include <SFML/Graphics.hpp>

namespace gel
{
	void SFMLApplication::Run(const std::string& title)
	{
        //Open the window
		//sf::Vector2u windowSize(windowWidth, windowHeight);
		//sf::VideoMode videoMode(windowSize);
        //Get all videomodes
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        //first one is usually the best option
        auto window = sf::RenderWindow(modes[0], title.c_str(), sf::State::Fullscreen);
        //Stabilize fps
        window.setVerticalSyncEnabled(true);
        //Hide mouse
        window.setMouseCursorVisible(false);

        // Initialisation logic
		Init(window);

        //Create a clock to get deltaTime
        sf::Clock clock;

        //Debug timer
        //DEB: sf::Clock timer;

        //Main game loop
        while (window.isOpen())
        {
            //get deltaTime
            float dt = clock.restart().asSeconds();

            //DEB: std::cout << " -Display: " << timer.restart().asSeconds() << '\n';
            //DEB: std::cout << "Next frame: " << '\n';

            // Process events
            while (const std::optional event = window.pollEvent())
            {
                //If window is closed, then close the game
                if (event->is<sf::Event::Closed>())
                    window.close();

                // Event handling code here
                HandleEvent(event);
            }
            //DEB: std::cout << " -Handle events: " << timer.restart().asSeconds() << '\n';

            // Update logic
            Update(dt, window);
            //DEB: std::cout << " -Update: " << timer.restart().asSeconds() << '\n';

            // Clear window
            window.clear();
            //DEB: std::cout << " -Clear: " << timer.restart().asSeconds() << '\n';

            //Draw to the window
            Render(window);
            //DEB: std::cout << " -Render: " << timer.restart().asSeconds() << '\n';

            //Display image from the double buffer
            window.display();
        }
	}


}