#include "SFMLApplication.h"
#include <iostream>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include "CompilerInstructions.h"

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

#ifdef OUTPUT_FRAME_TIMING
        //Debug timer
        sf::Clock timer;
#endif

        //Main game loop
        while (renderWindow.isOpen())
        {
            //get deltaTime
            float dt = clock.restart().asSeconds();

#ifdef OUTPUT_FRAME_TIMING
            std::cout << " -Display: " << timer.restart().asSeconds() << '\n';
            std::cout << "Next frame: " << '\n';
#endif

            // Process events
            while (const std::optional event = renderWindow.pollEvent())
            {
                //If window is closed, then close the game
                if (event->is<sf::Event::Closed>())
                    renderWindow.close();

                // Event handling code here
                HandleEvent(event);
            }
#ifdef OUTPUT_FRAME_TIMING
            std::cout << " -Handle events: " << timer.restart().asSeconds() << '\n';
#endif

            // Update logic
            Update(dt, renderWindow);
#ifdef OUTPUT_FRAME_TIMING
            std::cout << " -Update: " << timer.restart().asSeconds() << '\n';
#endif

            // Clear window
            renderWindow.clear();
#ifdef OUTPUT_FRAME_TIMING
            std::cout << " -Clear: " << timer.restart().asSeconds() << '\n';
#endif

            //Draw to the window
            Render(renderWindow);
#ifdef OUTPUT_FRAME_TIMING
            std::cout << " -Render: " << timer.restart().asSeconds() << '\n';
#endif

            //Display image from the double buffer
            renderWindow.display();
        }
	}


}