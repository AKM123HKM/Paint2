#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "stroke.h"
#include "button_utility.h"

class Paint;

class SketchBoard{
private:
	sf::Vector2f pos;
	int width;
	int height;
	std::vector<Stroke> strokes;
	float stroke_thickness = 10.f;
public:
	SketchBoard(sf::Vector2f aPos, int aWidth, int aHeight);
    bool check_dimensions(sf::Vector2f mouse_pos);
    sf::Vector2f get_dimensions();
    sf::Vector2f get_position();
    void add_stroke();
    void draw(sf::RenderWindow& window);
    void add_vertex(sf::Vector2f mouse_pos,sf::Color color);
    void add_click_vertex(sf::Vector2f mouse_pos,sf::Color color);
    void change_thickness(int delta);
    void set_thickness(float thickness);
    int get_strokes_count();
};