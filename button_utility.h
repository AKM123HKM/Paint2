#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include "mouse_utility.h"

enum class ButtonState{
	Normal,
	Hover,
	Click
};

class Button{
private:
	sf::RectangleShape rect;
	sf::Text text;
	sf::Color hover_color{150,150,150};
	sf::Color hover_outline_color{170,170,170};
	sf::Color hover_text_color{50,50,50};
	sf::Color click_color{200,200,200};
	sf::Color click_outline_color{220,220,220};
	sf::Color click_text_color{120,120,120};
	sf::Color normal_color{100,100,100};
	sf::Color normal_outline_color{120,120,120};
	sf::Color normal_text_color{20,20,20};
	std::function<void()> on_click;
	ButtonState state = ButtonState::Normal;

public:
	sf::Vector2f get_position();
	void set_position(sf::Vector2f pos);
	void set_state(ButtonState aState);
	sf::Vector2f get_size();
	void set_size(sf::Vector2f size);
	void set_text(std::string aText);
	void set_text_position(sf::Vector2f pos = sf::Vector2f(-1,-1));
	void set_text_font(sf::Font& font);
	void set_normal_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(20,20,20));
	void set_hover_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(50,50,50));
	void set_click_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(120,120,120));
	bool check_collision(sf::Vector2f mouse_pos);
	void set_function(std::function<void()> func);
	void do_function();
	void draw(sf::RenderWindow& window);
};

class Slider{
private:
	sf::RectangleShape filled;
	sf::RectangleShape remaining;
	Button handle;
	sf::Vector2f position = {0,0};
	sf::Vector2f track_size = {100,20};
	int track_size_offset = 4;
	int min_val = 0;
	int max_val = 100;

	enum class State{
		Selected,
		Not_Selected
	};

	State slider_state = State::Not_Selected;
public:
	void set_track_size(sf::Vector2f aSize);
	sf::Vector2f get_track_size();
	void set_track_position(sf::Vector2f aPosition);
	sf::Vector2f get_track_position();
	void set_track_colors(sf::Color filled_track_color, sf::Color remaining_track_color);
	void set_handle_size(sf::Vector2f aSize);
	sf::Vector2f get_handle_size();
	void set_handle_colors(sf::Color hover_color, sf::Color click_color, sf::Color normal_color);
	void set_min_max_value(int aMin, int aMax);
	void set_track_size_offset(int offset);
	int get_track_size_offset();
	float get_slider_value();
	void set_slider_value(float value);
	void update_track();
	void update_handle(sf::Vector2f mouse_pos, ButtonResult left_result);
	void update(sf::Vector2f mouse_pos, ButtonResult left_result);
	void draw(sf::RenderWindow& window);
};