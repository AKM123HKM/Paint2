#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Stroke{
private:
	sf::VertexArray points;

	sf::Vector2f temp_pos;
	bool temp_switch = true;

	float thickness = 10.f;

public:
    Stroke(float aThickness);
    void add_vertex(sf::Vector2f pos, sf::Color color);
    void add_click_vertex(sf::Vector2f pos, sf::Color color);
    void draw(sf::RenderWindow& window);
    void change_thickness(float aThickness);
    int get_vertex_count();
};