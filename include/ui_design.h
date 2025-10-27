#pragma once
#include "constants.h"
#include "button_utility.h"

Button create_button(sf::Vector2f pos, sf::Vector2f size, std::string text);

Slider create_slider(sf::Vector2f slider_pos, sf::Vector2f track_size, sf::Vector2f handle_size, 
					 int min_value, int max_value ,float slider_value = 0);