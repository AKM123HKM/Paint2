#include "mouse_utility.h"
#include "button_utility.h"
#include <iostream>
#include <SFML/Graphics.hpp>

sf::Vector2f Button::get_position(){
		return rect.getPosition();
	}

void Button::set_state(ButtonState aState){
		state = aState;
	}

void Button::set_position(sf::Vector2f pos){
		rect.setPosition(pos);
	}

sf::Vector2f Button::get_size(){
		return rect.getSize();
	}

void Button::set_size(sf::Vector2f size){
		rect.setSize(size);
	}

void Button::set_text(std::string aText){
		text.setString(aText);
		text.setCharacterSize(15);
	}

void Button::set_text_position(sf::Vector2f pos){
		if (pos.x == -1){
			sf::Vector2f size = get_size();
			sf::Vector2f button_pos = get_position();
			float x = button_pos.x + size.x/2;
			float y = button_pos.y + size.y/2;
			auto text_rect = text.getLocalBounds();
			text.setOrigin(text_rect.left + text_rect.width/2,
						   text_rect.top + text_rect.height/2);
			text.setPosition(sf::Vector2f(x,y));
		}
		else{
			text.setPosition(pos);
		}
	}

void Button::set_text_font(sf::Font& font){
		text.setFont(font);
	}

void Button::set_normal_colors(sf::Color color, sf::Color outline_color, sf::Color text_color){
		normal_color = color;
		normal_outline_color = outline_color;
	}

void Button::set_hover_colors(sf::Color color, sf::Color outline_color, sf::Color text_color){
		hover_color = color;
		hover_outline_color = outline_color;
	}

void Button::set_click_colors(sf::Color color, sf::Color outline_color, sf::Color text_color){
		click_color = color;
		click_outline_color = outline_color;
	}

bool Button::check_collision(sf::Vector2f mouse_pos){
		auto pos = get_position();
		auto size = get_size();
		if(mouse_pos.x >= pos.x && mouse_pos.x <= size.x + pos.x){
			if(mouse_pos.y >= pos.y && mouse_pos.y <= size.y + pos.y){
				return true;
			}
		}
		return false;
	}

void Button::set_function(std::function<void()> func){
			on_click = func;
		}

void Button::do_function(){
		on_click();
	}

void Button::draw(sf::RenderWindow& window){
		if(state == ButtonState::Hover){
			rect.setFillColor(hover_color);
			rect.setOutlineColor(hover_outline_color);
			text.setFillColor(hover_text_color);
		}
		else if(state == ButtonState::Click){
			rect.setFillColor(click_color);
			rect.setOutlineColor(click_outline_color);
			text.setFillColor(click_text_color);
		}
		else{
			rect.setFillColor(normal_color);
			rect.setOutlineColor(normal_outline_color);
			text.setFillColor(normal_text_color);
		}
		window.draw(rect);
		window.draw(text);
	}

// SLIDER

void Slider::set_track_size(sf::Vector2f aSize){
	if (aSize.x > 0 & aSize.y > 0){
		track_size = aSize;
	}
}

sf::Vector2f Slider::get_track_size(){
	return track_size;
}

void Slider::set_track_colors(sf::Color filled_track_color, sf::Color remaining_track_color){
	filled.setFillColor(filled_track_color);
	remaining.setFillColor(remaining_track_color);
}

void Slider::set_handle_size(sf::Vector2f aSize){
	if (aSize.x > 0 & aSize.y > 0){
		handle.set_size(aSize);
	}
}

sf::Vector2f Slider::get_handle_size(){
	return handle.get_size();
}

void Slider::set_handle_colors(sf::Color hover_color, sf::Color click_color, sf::Color normal_color){
	handle.set_hover_colors(hover_color,sf::Color(0,0,0,0),sf::Color(0,0,0,0));
	handle.set_click_colors(click_color,sf::Color(0,0,0,0),sf::Color(0,0,0,0));
	handle.set_normal_colors(normal_color,sf::Color(0,0,0,0),sf::Color(0,0,0,0));
}

void Slider::set_track_position(sf::Vector2f aPosition){
	position = aPosition;
	handle.set_position(aPosition);
}

sf::Vector2f Slider::get_track_position(){
	return position;
}

void Slider::set_min_max_value(int aMin, int aMax){
	if (aMin < aMax){
		min_val = aMin;
		max_val = aMax;
	}
}

void Slider::set_track_size_offset(int offset){
	track_size_offset = offset;
}

int Slider::get_track_size_offset(){
	return track_size_offset;
}

float Slider::get_slider_value(){
	return (max_val - min_val)*((filled.getSize().x)/track_size.x) + min_val;
}

void Slider::set_slider_value(float value){
	float x = value/(max_val - min_val) * track_size.x + position.x;
	handle.set_position(sf::Vector2f(x,handle.get_position().y));
	update_track();
}

void Slider::update_track(){
	// std::cout << filled.getSize().x << "," << filled.getSize().y << std::endl;
	filled.setPosition(sf::Vector2f(position.x,position.y + track_size_offset/2));
	filled.setSize(sf::Vector2f(handle.get_position().x - position.x,handle.get_size().y - track_size_offset));

	remaining.setPosition(sf::Vector2f(handle.get_position().x + handle.get_size().x,position.y + track_size_offset/2));
	remaining.setSize(sf::Vector2f(track_size.x - (handle.get_size().x + filled.getSize().x),handle.get_size().y - track_size_offset));
	// std::cout << filled.getSize().x << "," << filled.getSize().y << std::endl;
}

void Slider::update_handle(sf::Vector2f mouse_pos, ButtonResult left_result){
	auto handle_pos = handle.get_position();
	auto handle_size = handle.get_size();
		
		if (mouse_pos.x >= handle_pos.x & mouse_pos.x <= handle_size.x + handle_pos.x){
			if (mouse_pos.y >= handle_pos.y & mouse_pos.y <= handle_size.y + handle_pos.y){
				handle.set_state(ButtonState::Hover);
				if (left_result.pressed){
					slider_state = State::Selected;
				}
			}
		}

		if (left_result.released){
			handle.set_state(ButtonState::Normal);
			slider_state = State::Not_Selected;
		}

		if (slider_state == State::Selected){
			handle.set_state(ButtonState::Click);
			if (left_result.dragging){
				float x = handle_pos.x + left_result.drag_value.x;
				if (x > position.x + track_size.x){
					x = position.x + track_size.x;
				}
				else if (x < position.x){
					x = position.x;
				}
				handle.set_position(sf::Vector2f(x,handle_pos.y));
			}
		}
	}

void Slider::update(sf::Vector2f mouse_pos, ButtonResult left_result){
	update_handle(mouse_pos,left_result);
	update_track();
}

void Slider::draw(sf::RenderWindow& window){
	window.draw(filled);
	window.draw(remaining);
	handle.draw(window);
}