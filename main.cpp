#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "mouse_utility.h"

constexpr int WIDTH {800};
constexpr int HEIGHT {600};
const sf::Color BG_COLOR(150,150,150);

class Stroke{
private:
	sf::VertexArray points;

	sf::Vector2f temp_pos;
	bool temp_switch = true;

	float thickness = 10.f;

public:
	Stroke(float aThickness):points(sf::Quads){
		thickness = aThickness;
	}

	void add_vertex(sf::Vector2f pos, sf::Color color){
		if (temp_switch){
			temp_pos = pos;
			temp_switch = !temp_switch;
		}
		else{
			sf::Vector2f dir = temp_pos - pos;
			float magnitude = sqrt((dir.x * dir.x) + (dir.y * dir.y));
			if (!(magnitude == 0)){
				/* This is a normalized perpendicular vector to the direction vector of the previous
					and the current position/vertex */
				sf::Vector2f normalized_dir = dir / magnitude;
				sf::Vector2f normalized_perp(-normalized_dir.y,normalized_dir.x);
				sf::Vector2f offset = normalized_perp * (thickness/2.f);

				sf::Vector2f offset_point_one = temp_pos - offset;
				sf::Vector2f offset_point_two = temp_pos + offset;
				sf::Vector2f offset_point_three = pos + offset;
				sf::Vector2f offset_point_four = pos - offset;

				if (points.getVertexCount() == 0){
					points.append(sf::Vertex(offset_point_one,color));
					points.append(sf::Vertex(offset_point_two,color));
				}
				else{
					points.append(sf::Vertex(offset_point_one,color));
					points.append(sf::Vertex(offset_point_two,color));
					points.append(sf::Vertex(offset_point_one,color));
					points.append(sf::Vertex(offset_point_two,color));
				}
				
				points.append(sf::Vertex(offset_point_three,color));
				points.append(sf::Vertex(offset_point_four,color));
				points.append(sf::Vertex(offset_point_three,color));
				points.append(sf::Vertex(offset_point_four,color));

				temp_switch = !temp_switch;
			}
		}
	}

	void add_click_vertex(sf::Vector2f pos, sf::Color color){
		auto offset = thickness/2;
		auto offset_point_one = sf::Vector2f(pos.x - offset, pos.y - offset);
		auto offset_point_two = sf::Vector2f(pos.x + offset, pos.y - offset);
		auto offset_point_three = sf::Vector2f(pos.x + offset, pos.y + offset);
		auto offset_point_four = sf::Vector2f(pos.x - offset, pos.y + offset);

		points.append(sf::Vertex(offset_point_one,color));
		points.append(sf::Vertex(offset_point_two,color));
		points.append(sf::Vertex(offset_point_three,color));
		points.append(sf::Vertex(offset_point_four,color));
	}

	void draw(sf::RenderWindow& window){
		int size = points.getVertexCount();
		window.draw(points);
	}

	void change_thickness(float aThickness){
		thickness = aThickness;
	}

};

class SketchBoard{
private:
	
	float stroke_thickness = 10.f;
public:
	std::vector<Stroke> strokes;
	void add_stroke(){
		Stroke stroke(stroke_thickness);
		strokes.push_back(stroke);
	}

	void draw(sf::RenderWindow& window){
		int size = 0;
		for (auto stroke:strokes){
			stroke.draw(window);
		}
	}

	void add_vertex(sf::Vector2f mouse_pos,sf::Color color){
		strokes.back().add_vertex(mouse_pos,color);
	}

	void add_click_vertex(sf::Vector2f mouse_pos,sf::Color color){
		strokes.back().add_click_vertex(mouse_pos,color);
	}

	void change_thickness(int delta){
		stroke_thickness += delta;
		stroke_thickness = std::clamp(stroke_thickness,1.f,25.f);
		strokes.back().change_thickness(stroke_thickness);
	}
};

int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"Paint");
	// window.setFramerateLimit(10);
	sf::Clock delta_clock;
	sf::Clock fps_clock;
	double dt;

	SketchBoard sketch_board;
	sketch_board.add_stroke();

	sf::Font font;
	if(!font.loadFromFile("PoetsenOne-Regular.ttf")){
		std::cout << "Error loading the font file" << std::endl;
		return -1;
	}

	Mouse mouse;

	sf::Text fps;
	fps.setFont(font);
	fps.setFillColor(sf::Color::Black);
	fps.setPosition(sf::Vector2f(0,0));

	bool ispressed = false;
	bool waspressed = false;

	while (window.isOpen()){

		dt = delta_clock.restart().asSeconds();
		if (fps_clock.getElapsedTime().asSeconds() >= 0.5){
		fps.setString((std::to_string((1/dt))).substr(0,5));
		fps_clock.restart();
		}

		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}
			if (event.type == sf::Event::MouseWheelScrolled) {
			    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
			        float delta = event.mouseWheelScroll.delta;
			        sketch_board.change_thickness(delta);
			    }
			}
		}

	sf::Vector2i i_pos = sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos = sf::Vector2f(i_pos);

	auto left_result = mouse.get_button_state(sf::Mouse::Left,window);
	if (left_result.clicked){
		// std::cout << "Click" << std::endl;
		sketch_board.add_click_vertex(mouse_pos,sf::Color::Red);
	}
	if (left_result.dragging){
		// std::cout << "dragging" << std::endl;
		sketch_board.add_vertex(mouse_pos,sf::Color::Red);
	}
	if (left_result.release_transition){
		// std::cout << "Added stroke" << std::endl;
		sketch_board.add_stroke();
	}

	auto right_result = mouse.get_button_state(sf::Mouse::Right,window);
	if (right_result.clicked){
		std::cout << sketch_board.strokes.size() << std::endl;
	}

	window.clear(BG_COLOR);
	sketch_board.draw(window);
	window.draw(fps);
	window.display();
	}

	return 0;
}