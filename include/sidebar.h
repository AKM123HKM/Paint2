#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "button_utility.h"
#include "ui_design.h"

class Paint;

class SideBar{
public:
	Button thickness_button;
	Button color_button;
	Slider thickness_slider;
	sf::RectangleShape selected_color_box;
    Paint& paint;

	SideBar(Paint& aPaint);
    float get_thickness();
    void handle_button_collision(sf::Vector2f mouse_pos,ButtonResult left_result);
    void update_selected_color(sf::Color color);
    void update_selected_color_box(sf::Color color);
    void draw(sf::RenderWindow& window);
};