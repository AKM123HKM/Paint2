#include "button_utility.h"
#include <iostream>

sf::Vector2f Button::get_position(){
		return rect.getPosition();
	}

void Button::set_state(std::string aState){
		if (aState == "normal" or aState == "hover" or aState == " click"){
			aState = state;
		}
		else{
			std::cout << "State should be 'normal' or 'hover' or 'click'." << std::endl;
		}
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
	}

void Button::set_text_position(sf::Vector2f pos = sf::Vector2f(-1,-1)){
		if (pos.x == -1){
			sf::Vector2f size = get_size();
			sf::Vector2f button_pos = get_position();
			std::cout << size.x << "," << size.y << std::endl;
			std::cout << size.x/2 << "," << size.y/2 << std::endl;
			std::cout << button_pos.x << "," << button_pos.y << std::endl;
			float x = button_pos.x + size.x/2;
			float y = button_pos.y + size.y/2;
			std::cout << x << "," << y << std::endl;
			auto text_rect = text.getLocalBounds();
			text.setOrigin(text_rect.width/2,
						   text_rect.height/2);
			text.setPosition(sf::Vector2f(x,y));
		}
		else{
			text.setPosition(pos);
		}
	}

void Button::set_text_font(sf::Font& font){
		text.setFont(font);
	}

void Button::set_normal_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(20,20,20)){
		normal_color = color;
		normal_outline_color = outline_color;
	}

void Button::set_hover_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(50,50,50)){
		hover_color = color;
		hover_outline_color = outline_color;
	}

void Button::set_click_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(120,120,120)){
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
		if(state == "hover"){
			rect.setFillColor(hover_color);
			rect.setOutlineColor(hover_outline_color);
			text.setFillColor(hover_text_color);
		}
		else if(state == "click"){
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