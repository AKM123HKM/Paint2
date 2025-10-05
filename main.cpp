#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <functional>
#include "mouse_utility.h"

constexpr int WIDTH {800};
constexpr int HEIGHT {600};
const sf::Color BG_COLOR(150,150,150);
const sf::Color NORMAL_BUTTON_COLOR{85,85,85};
const sf::Color NORMAL_OUTLINE_COLOR{90,90,90};
const sf::Color NORMAL_TEXT_COLOR{170,170,170};
const sf::Color HOVER_BUTTON_COLOR{55,55,55};
const sf::Color HOVER_OUTLINE_COLOR{45,45,45};
const sf::Color HOVER_TEXT_COLOR{200,200,200};
const sf::Color CLICK_BUTTON_COLOR{20,20,20};
const sf::Color CLICK_OUTLINE_COLOR{10,10,10};
const sf::Color CLICK_TEXT_COLOR{200,200,200};
sf::Font FONT;

class Button{
private:[]
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
	std::string state{"normal"};
public:
	
	sf::Vector2f get_position(){
		return rect.getPosition();
	}

	void set_state(std::string aState){
		if (aState == "normal" or aState == "hover" or aState == " click"){
			aState = state;
		}
		else{
			std::cout << "State should be 'normal' or 'hover' or 'click'." << std::endl;
		}
	}

	void set_position(sf::Vector2f pos){
		rect.setPosition(pos);
	}

	sf::Vector2f get_size(){
		return rect.getSize();
	}

	void set_size(sf::Vector2f size){
		rect.setSize(size);
	}

	void set_text(std::string aText){
		text.setString(aText);
	}

	void set_text_position(sf::Vector2f pos = sf::Vector2f(-1,-1)){
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

	void set_text_font(sf::Font& font){
		text.setFont(font);
	}

	void set_normal_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(20,20,20)){
		normal_color = color;
		normal_outline_color = outline_color;
	}

	void set_hover_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(50,50,50)){
		hover_color = color;
		hover_outline_color = outline_color;
	}

	void set_click_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(120,120,120)){
		click_color = color;
		click_outline_color = outline_color;
	}

	bool check_collision(sf::Vector2f mouse_pos){
		auto pos = get_position();
		auto size = get_size();
		if(mouse_pos.x >= pos.x && mouse_pos.x <= size.x + pos.x){
			if(mouse_pos.y >= pos.y && mouse_pos.y <= size.y + pos.y){
				return true;
			}
		}
		return false;
	}

	void set_function(std::function<void()> func){
			on_click = func;
		}

	void do_function(){
		on_click();
	}

	void draw(sf::RenderWindow& window){
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
};

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

	int get_vertex_count(){
		return points.getVertexCount();
	}
};

class SketchBoard{
private:
	sf::Vector2f pos;
	int width;
	int height;
	std::vector<Stroke> strokes;
	float stroke_thickness = 10.f;
public:

	SketchBoard(sf::Vector2f aPos, int aWidth, int aHeight){
		pos = aPos;
		width = aWidth;
		height = aHeight;
		Stroke stroke(stroke_thickness);
		strokes.push_back(stroke);
	}

	bool check_dimensions(sf::Vector2f mouse_pos){
		if (mouse_pos.x >= pos.x && mouse_pos.x <= width+pos.x){
			if (mouse_pos.y >= pos.y && mouse_pos.y <= height+pos.y){
				return true;
			}
		}

		return false;	
	}

	void add_stroke(){
		if (strokes.back().get_vertex_count() != 0){
			Stroke stroke(stroke_thickness);
			strokes.push_back(stroke);
		}
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

	int get_strokes_count(){
		return strokes.size();
	}
};

class ButtonUi{
public:
	Button thickness_button;

	ButtonUi(){
		create_thickness_button();
	}

	void create_thickness_button(){
		thickness_button.set_position(sf::Vector2f(50,100));
		thickness_button.set_size(sf::Vector2f(200,60));
		thickness_button.set_normal_colors(NORMAL_BUTTON_COLOR,NORMAL_OUTLINE_COLOR,NORMAL_TEXT_COLOR);
		thickness_button.set_hover_colors(HOVER_BUTTON_COLOR,HOVER_OUTLINE_COLOR,HOVER_TEXT_COLOR);
		thickness_button.set_click_colors(CLICK_BUTTON_COLOR,CLICK_OUTLINE_COLOR,CLICK_TEXT_COLOR);
		thickness_button.set_text("Thickness");
		thickness_button.set_text_font(FONT);
		thickness_button.set_text_position();
	}

	void check_collision(sf::Vector2f mouse_pos){
		if (thickness_button.check_collision(mouse_pos)){
			thickness_button.set_state("hover");
		}
		else{
			thickness_button.set_state("normal");
		}
	}

	void draw(sf::RenderWindow& window){
		thickness_button.draw(window);
	}
};

int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"Paint");
	// window.setFramerateLimit(10);
	sf::Clock delta_clock;
	sf::Clock fps_clock;
	double dt;

	if(!FONT.loadFromFile("PoetsenOne-Regular.ttf")){
		std::cout << "Error loading the font file" << std::endl;
		return -1;
	}

	SketchBoard sketch_board(sf::Vector2f(100,0),WIDTH - 100,HEIGHT);

	Mouse mouse;

	ButtonUi buttonui;

	sf::Text fps;
	fps.setFont(FONT);
	fps.setFillColor(sf::Color::Black);
	fps.setPosition(sf::Vector2f(0,0));

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
	buttonui.check_collision(mouse_pos);
	if (sketch_board.check_dimensions(mouse_pos)){
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
	}
	// In case mouse gets out of bounds of sketch board add a new stroke
	else if (left_result.pressed){
		sketch_board.add_stroke();
	}

	auto right_result = mouse.get_button_state(sf::Mouse::Right,window);
	if (right_result.clicked){
		std::cout << sketch_board.get_strokes_count() << std::endl;
	}

	window.clear(BG_COLOR);
	sketch_board.draw(window);
	window.draw(fps);
	buttonui.draw(window);
	window.display();
	}

	return 0;
}