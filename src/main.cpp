#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <functional>
#include "paint.h"
#include "constants.h"

int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"Paint");
	// window.setFramerateLimit(1);
	sf::Clock delta_clock;
	sf::Clock fps_clock;
	double dt;

	if(!FONT.loadFromFile("assets/PoetsenOne-Regular.ttf")){
		std::cout << "Error loading the font file" << std::endl;
		return -1;
	}

	Paint paint(SKETCHBOARD_POS,SKETCHBOARD_WIDTH,SKETCHBOARD_HEIGHT);

	sf::Text fps;
	fps.setFont(FONT);
	fps.setFillColor(sf::Color::Black);
	fps.setPosition(sf::Vector2f(0,0));

	while (window.isOpen()){

		dt = delta_clock.restart().asSeconds();
		if (fps_clock.getElapsedTime().asSeconds() >= 0.5){
		fps.setString((std::to_string((1/dt))).substr(0,5));
		fps_clock.restart();
		}

		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}
			paint.handle_window_events(event);
		}

	paint.run(window);
	window.clear(BG_COLOR);
	paint.draw(window);
	window.display();
	}

	return 0;
}