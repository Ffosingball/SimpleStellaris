#pragma once

#include <string>
#include <optional>

namespace sf
{
	class RenderWindow;
	class Event;
}

namespace gel
{
	class SFMLApplication
	{
	public:
		//Run functions starts the game and contains the game loop
		void Run(const std::string& title);
		virtual ~SFMLApplication() = default;

	protected:
		//Init to initialize the game
		virtual void Init(sf::RenderWindow& renderWindow) {}
		//Update is called every frame, after HandleEvent if it was called
		virtual void Update(const float deltaTime, sf::RenderWindow& renderWindow) {}
		//Render is called every frame, after update
		virtual void Render(sf::RenderWindow& renderWindow) {}
		//It is called when some event is happened
		virtual void HandleEvent(const std::optional<sf::Event>& event) {}
	};
}