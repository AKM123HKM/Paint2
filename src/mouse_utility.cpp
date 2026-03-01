#include "mouse_utility.h"
#include <iostream>

MouseButton& Mouse::check_button(sf::Mouse::Button button){
	if (sf::Mouse::Button::Left == button){
		return left_button;
	}
	else if (sf::Mouse::Button::Right == button){
		return right_button;
	}
	return middle_button;
}

float Mouse::magnitude(sf::Vector2f vec){
	return sqrt(vec.x*vec.x + vec.y*vec.y);
}

sf::Vector2f Mouse::get_mouse_position(sf::RenderWindow& window){
	sf::Vector2i int_mouse_pos =  sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos = sf::Vector2f(int_mouse_pos);

	return mouse_pos;
}

void Mouse::updateMousePosition(sf::RenderWindow& window){
	prev_mouse_pos = current_mouse_pos;
	current_mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
}

void Mouse::updateButton(sf::Mouse::Button aButton){
	auto& button = check_button(aButton);
	button.event = MouseEvents::None;
	if (sf::Mouse::isButtonPressed(aButton)){
		if(button.state == MouseStates::Idle){
			button.event = MouseEvents::Press_Transition;
			button.press_timer.restart();
		}
		button.state = MouseStates::Pressed;
	}

	if (button.state == MouseStates::Pressed & button.press_timer.getElapsedTime().asSeconds() > HOLD_THRESHOLD){
		button.state = MouseStates::Held;
	}

	if (button.state == MouseStates::Pressed || button.state == MouseStates::Held){
		if (magnitude(prev_mouse_pos - current_mouse_pos) >= DRAG_THRESHOLD){
			button.state = MouseStates::Dragging;
		}
	}
	else{
		if(button.state != MouseStates::Idle){
			button.event = MouseEvents::Release_Transition;
		}
		if(button.state == MouseStates::Pressed){
			if(button.click_timer.getElapsedTime().asSeconds() >= DOUBLE_CLICK_THRESHOLD){
				button.event = MouseEvents::Double_Click;
				button.click_timer.restart();
			}
			button.event = MouseEvents::Click;
			button.click_timer.restart();
		}
		button.state = MouseStates::Idle;
		button.press_timer.reset();
		button.press_timer.stop();
	}
}

MouseStates Mouse::get_button_state(sf::Mouse::Button aButton){
	auto& button = check_button(aButton);
	return button.state;
}

MouseEvents Mouse::get_button_event(sf::Mouse::Button aButton){
	auto& button = check_button(aButton);
	return button.event;
}