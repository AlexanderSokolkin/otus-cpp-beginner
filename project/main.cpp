#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>

#include "field.h"
#include "constants.h"


int main(int argc, char* argv[])
{
	// Create a window
	sf::RenderWindow window(sf::VideoMode(600, 600), "Sapper");
	Field f;
	f.move(50.f, 50.f);
	f.load("textures/tiles.png", 50, 10, 10, 10);

	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Button::Left)
			{
				if (event.mouseButton.x > 50	&&
					event.mouseButton.x < 550	&&
					event.mouseButton.y > 50	&&
					event.mouseButton.y < 550)
				{
					f.onFieldPressed(sf::Vector2i{event.mouseButton.x - 50, event.mouseButton.y - 50});
				}
				else {
					f.lightOffAllCells();
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
				event.type == sf::Event::MouseMoved)
			{
				if (event.mouseMove.x > 50	&&
					event.mouseMove.x < 550	&&
					event.mouseMove.y > 50	&&
					event.mouseMove.y < 550)
				{
					f.onFieldPressed(sf::Vector2i{event.mouseMove.x - 50, event.mouseMove.y - 50});
				}
				else {
					f.lightOffAllCells();
				}
			}


			if (event.type == sf::Event::MouseButtonReleased) {
				f.lightOffAllCells();
				Result res = f.onFieldReleased(sf::Vector2i{event.mouseButton.x - 50, event.mouseButton.y - 50}, event.mouseButton.button);

			}


		}

		window.clear();
		window.draw(f);
		window.display();
	}

	return 0;
}
