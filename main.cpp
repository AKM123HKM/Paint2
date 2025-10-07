#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <functional>
#include "mouse_utility.h"
#include "button_utility.h"
#include "constants.h"

enum class WindowState{
	Main,
	Color_Picker
};

WindowState window_state = WindowState::Main;

Button create_button(sf::Vector2f pos, sf::Vector2f size, std::string text){
	Button button;
	button.set_position(pos);
	button.set_size(size);
	button.set_normal_colors(NORMAL_BUTTON_COLOR,NORMAL_OUTLINE_COLOR,NORMAL_TEXT_COLOR);
	button.set_hover_colors(HOVER_BUTTON_COLOR,HOVER_OUTLINE_COLOR,HOVER_TEXT_COLOR);
	button.set_click_colors(CLICK_BUTTON_COLOR,CLICK_OUTLINE_COLOR,CLICK_TEXT_COLOR);
	button.set_text(text);
	button.set_text_font(FONT);
	button.set_text_position();

	return button;
}

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
	Button color_button;

	ButtonUi(){
		thickness_button = create_button(THICKNESS_BUTTON_POS,
										 THICKNESS_BUTTON_SIZE,
										 THICKNESS_BUTTON_TEXT);
		color_button = create_button(COLOR_BUTTON_POS,
									 COLOR_BUTTON_SIZE,
									 COLOR_BUTTON_TEXT);
		color_button.set_function([this](){if(window_state == WindowState::Main)window_state = WindowState::Color_Picker;
										   else window_state = WindowState::Main;});
	}	

	void check_collision(sf::Vector2f mouse_pos,ButtonResult left_result){
		if (thickness_button.check_collision(mouse_pos)){
			thickness_button.set_state(ButtonState::Hover);
		}
		else{
			thickness_button.set_state(ButtonState::Normal);
		}

		if (color_button.check_collision(mouse_pos)){
			color_button.set_state(ButtonState::Hover);
			if (left_result.clicked){
				color_button.set_state(ButtonState::Click);
				color_button.do_function();
			}
		}
		else{
			color_button.set_state(ButtonState::Normal);
		}
	}

	void draw(sf::RenderWindow& window){
		thickness_button.draw(window);
		color_button.draw(window);
	}
};

class ColorPicker{
private:
	sf::Sprite color_picker;
	sf::Texture texture;
	sf::Image image;
	int pickerWidth = COLOR_PICKER_WIDTH;
	int pickerHeight = COLOR_PICKER_WIDTH;
	int blueValue = 100;
	Button blue_up;
	Button blue_down;

	void create_image(){

	image.create(pickerWidth, pickerHeight);
	for(int x=0; x<pickerWidth; x++){
	    for(int y=0; y<pickerHeight; y++){
	        image.setPixel(x, y, sf::Color(x*255/pickerWidth, y*255/pickerHeight, blueValue));
			}
		}
	}

public:

	ColorPicker(){
		blue_up = create_button(BLUE_UP_POS,
								BLUE_UP_SIZE,
								BLUE_UP_TEXT);
		/* "[this]" means capture the value of increase_blueValue from this class ColorPicker
		since it needs the class to be called outside the main class*/
		blue_up.set_function([this]() { if(blueValue < 255) blueValue++; });

		blue_down = create_button(BLUE_DOWN_POS,
								  BLUE_DOWN_SIZE,
								  BLUE_DOWN_TEXT);
		blue_down.set_function([this]() { if(blueValue > 0) blueValue--;});

		create_image();
		texture.loadFromImage(image);
		color_picker.setTexture(texture);
		color_picker.setPosition(COLOR_PICKER_POS);
	}

	void handle_button_collisions(sf::Vector2f mouse_pos,ButtonResult left_result){
		if (blue_up.check_collision(mouse_pos)){
			blue_up.set_state(ButtonState::Hover);
			if (left_result.clicked){
				blue_up.set_state(ButtonState::Click);
				blue_up.do_function();
				create_image();
				texture.loadFromImage(image);
				color_picker.setTexture(texture);
			}
			else if (left_result.held){
				blue_up.do_function();
				create_image();
				texture.loadFromImage(image);
				color_picker.setTexture(texture);
			}
		}
		else{
			blue_up.set_state(ButtonState::Normal);
		}

		if (blue_down.check_collision(mouse_pos)){
			blue_down.set_state(ButtonState::Hover);
			if (left_result.clicked){
				blue_down.set_state(ButtonState::Click);
				blue_down.do_function();
				create_image();
				texture.loadFromImage(image);
				color_picker.setTexture(texture);
			}
			else if (left_result.held){
				blue_down.do_function();
				create_image();
				texture.loadFromImage(image);
				color_picker.setTexture(texture);
			}
		}
		else{
			blue_down.set_state(ButtonState::Normal);
		}
	}

	sf::Color handle_color_picking(sf::Vector2f mouse_pos,ButtonResult left_result){
		if (left_result.clicked){
			if (mouse_pos.x >= COLOR_PICKER_POS.x & mouse_pos.x <= COLOR_PICKER_WIDTH + COLOR_PICKER_POS.x){
				if (mouse_pos.y >= COLOR_PICKER_POS.y & mouse_pos.y <= COLOR_PICKER_HEIGHT + COLOR_PICKER_POS.y){
					int red = (mouse_pos.x - COLOR_PICKER_POS.x) * 255/COLOR_PICKER_WIDTH;
					int green = (mouse_pos.y - COLOR_PICKER_POS.y) * 255/COLOR_PICKER_HEIGHT;
					return sf::Color(red,green,blueValue);
				}
			}
		}
		return sf::Color(0,0,0,0);
	}

	void draw(sf::RenderWindow& window){
		window.draw(color_picker);
		blue_up.draw(window);
		blue_down.draw(window);
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

	Mouse mouse;

	SketchBoard sketch_board(SKETCHBOARD_POS,SKETCHBOARD_WIDTH,SKETCHBOARD_HEIGHT);
	sf::Color stroke_color = sf::Color(255,0,0);
	ButtonUi buttonui;

	ColorPicker color_window;

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
			if (window_state == WindowState::Main)
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
	buttonui.check_collision(mouse_pos,left_result);
	if (window_state == WindowState::Main)
		if (sketch_board.check_dimensions(mouse_pos)){
			if (left_result.clicked){
				// std::cout << "Click" << std::endl;
				sketch_board.add_click_vertex(mouse_pos,stroke_color);
			}
			if (left_result.dragging){
				// std::cout << "dragging" << std::endl;
				sketch_board.add_vertex(mouse_pos,stroke_color);
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

	if(window_state == WindowState::Color_Picker){
		color_window.handle_button_collisions(mouse_pos,left_result);
		sf::Color color = color_window.handle_color_picking(mouse_pos,left_result);
		if (!(color == INVALID_COLOR)){
			stroke_color = color;
		}
		color_window.draw(window);
	}

	window.display();
	}

	return 0;
}