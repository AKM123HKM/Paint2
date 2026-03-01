#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <functional>
#include "paint.h"
#include "constants.h"

int main(){
	sf::RenderWindow window(sf::VideoMode({WIDTH,HEIGHT}),"Paint");
	// window.setFramerateLimit(1);
	sf::Clock delta_clock;
	sf::Clock fps_clock;
	double dt;

	if(!FONT.openFromFile("assets/PoetsenOne-Regular.ttf")){
		std::cout << "Error loading the font file" << std::endl;
		return -1;
	}

	Paint paint(SKETCHBOARD_POS,SKETCHBOARD_WIDTH,SKETCHBOARD_HEIGHT);
	
	std::string fps_string= "";
	sf::Text fps(FONT,fps_string,10);
	fps.setFillColor(sf::Color::Black);
	fps.setPosition(sf::Vector2f(0,0));

	while (window.isOpen()){

		dt = delta_clock.restart().asSeconds();
		if (fps_clock.getElapsedTime().asSeconds() >= 0.5){
		fps_string = std::to_string((1/dt)).substr(0,5);
		fps_clock.restart();
		}

		while (const std::optional  event = window.pollEvent()){
			if (event->is<sf::Event::Closed>()){
				window.close();
			}
			paint.handle_window_events(event);
		}

	paint.run(window);
	window.clear(BG_COLOR);
	window.draw(fps);
	paint.draw(window);
	window.display();
	}

	return 0;
}