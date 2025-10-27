#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "button_utility.h"
#include "ui_design.h"

class ColorPicker{
private:
	sf::Sprite color_picker;
	sf::Texture texture;
	sf::Image image;
	int pickerWidth = COLOR_PICKER_WIDTH;
	int pickerHeight = COLOR_PICKER_WIDTH;
	int blueValue = 100;
	Slider blue_slider;
	sf::Color selected_color;
	int selected_cell;
	sf::Color selected_colors[COLOR_GRID_ROW * COLOR_GRID_COL];
	Button add_color_button;
	void create_image(){

	image.create(pickerWidth, pickerHeight);
	for(int x=0; x<pickerWidth; x++){
	    for(int y=0; y<pickerHeight; y++){
	        image.setPixel(x, y, sf::Color(x*255/pickerWidth, y*255/pickerHeight, blueValue));
			}
		}
	}

public:
	ColorPicker();
    void handle_button_collisions(sf::Vector2f mouse_pos,ButtonResult left_result);
    sf::Color handle_color_picking(sf::Vector2f mouse_pos,ButtonResult left_result);
    void update_selected_colors(sf::Vector2f mouse_pos, sf::Color& stroke_color);
    void draw_selected_colors(sf::RenderWindow& window);
    void update(sf::Vector2f mouse_pos, ButtonResult left_result, sf::Color& stroke_color);
    void draw(sf::RenderWindow& window);
};