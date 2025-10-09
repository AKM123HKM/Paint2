#include "mouse_utility.h"
#include <iostream>

MouseState& Mouse::check_button(sf::Mouse::Button button){
	if (sf::Mouse::Left == button){
		return left_state;
	}
	else if (sf::Mouse::Right == button){
		return right_state;
	}
	return middle_state;
}

float Mouse::magnitude(sf::Vector2f vec){
	return sqrt(vec.x*vec.x + vec.y*vec.y);
}

sf::Vector2f Mouse::get_mouse_position(sf::RenderWindow& window){
	sf::Vector2i int_mouse_pos =  sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos = sf::Vector2f(int_mouse_pos);

	return mouse_pos;
}

ButtonResult Mouse::get_button_state(sf::Mouse::Button button,sf::RenderWindow& window){
	auto& button_state = check_button(button);
	ButtonResult result;

	button_state.was_pressed = button_state.is_pressed;

	if (sf::Mouse::isButtonPressed(button)){

		if (!button_state.is_pressed){
			button_state.press_timer.restart();
		}

		button_state.is_pressed = true;

		if (!button_state.was_pressed){
			result.press_transition = true;
		}

		// std::cout << prev_mouse_pos.x << std::endl;
		if (!(button_state.prev_mouse_pos.x == -1)){
			float delta = magnitude(button_state.prev_mouse_pos - get_mouse_position(window));
			if (delta >= HOLD_THRESHOLD){
				result.drag_value = get_mouse_position(window) - button_state.prev_mouse_pos;
				result.dragging = true;
			}
			else{
				result.drag_value = {0,0};
			}
		}
		button_state.prev_mouse_pos = get_mouse_position(window);
		// std::cout << prev_mouse_pos.x << std::endl;

		if (button_state.press_timer.getElapsedTime().asSeconds() >= HOLD_THRESHOLD){
			result.held = true;
		}

		result.pressed = true;
	}
	else{
		result.released = true;
		button_state.is_pressed = false;
		button_state.prev_mouse_pos = sf::Vector2f(-1,-1);
		if (button_state.was_pressed){
			if (button_state.press_timer.getElapsedTime().asSeconds() < HOLD_THRESHOLD){
					result.clicked = true;
					if (click_timer.getElapsedTime().asSeconds() <= DOUBLE_CLICK_THRESHOLD){
						result.double_clicked = true;
					}
					click_timer.restart();
			}
			result.release_transition = true;
		}
	}
return result;
}
