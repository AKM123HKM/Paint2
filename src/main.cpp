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
	Button_UI,
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

Slider create_slider(sf::Vector2f slider_pos, sf::Vector2f track_size, sf::Vector2f handle_size, 
					 int min_value, int max_value ,float slider_value = 0){
	Slider slider;
	slider.set_track_position(slider_pos);
	slider.set_track_size(track_size);
	slider.set_handle_size(handle_size);
	slider.set_min_max_value(min_value,max_value);
	if (slider_value >= min_value & slider_value <= max_value){
		slider.set_slider_value(slider_value);
	}
	else{
		slider.set_slider_value((min_value + max_value)/2);
	}
	slider.set_track_size_offset(TRACK_SIZE_OFFSET);
	slider.set_track_colors(GREEN_COLOR,RED_COLOR);
	slider.set_handle_colors(HANDLE_HOVER_COLOR, HANDLE_CLICK_COLOR, HANDLE_NORMAL_COLOR);

	return slider;
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

	sf::Vector2f get_dimensions(){
		return sf::Vector2f(width,height);
	}

	sf::Vector2f get_position(){
		return pos;
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
		stroke_thickness = std::clamp(stroke_thickness,static_cast<float>(MIN_THICKNESS),static_cast<float>(MAX_THICKNESS));
		strokes.back().change_thickness(stroke_thickness);
	}

	void set_thickness(float thickness){
		stroke_thickness = thickness;
		stroke_thickness = std::clamp(stroke_thickness,static_cast<float>(MIN_THICKNESS),static_cast<float>(MAX_THICKNESS));
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
	Slider thickness_slider;
	sf::RectangleShape selected_color_box;

	ButtonUi(){
		thickness_button = create_button(THICKNESS_BUTTON_POS,
										 THICKNESS_BUTTON_SIZE,
										 THICKNESS_BUTTON_TEXT);

		thickness_slider = create_slider(THICKNESS_SLIDER_POS,
										 THICKNESS_SLIDER_SIZE,
										 THICKNESS_SLIDER_HANDLE_SIZE,
										 MIN_THICKNESS,
										 MAX_THICKNESS,
										 DEFAULT_THICKNESS);

		color_button = create_button(COLOR_BUTTON_POS,
									 COLOR_BUTTON_SIZE,
									 COLOR_BUTTON_TEXT);
		color_button.set_function([this](){if(window_state == WindowState::Main || window_state == WindowState::Button_UI)window_state = WindowState::Color_Picker;
										   else window_state = WindowState::Button_UI;});

		selected_color_box.setSize(SELECTED_COLOR_BOX_SIZE);
		selected_color_box.setPosition(SELECTED_COLOR_BOX_POS);
	}

	float get_thickness(){
		return thickness_slider.get_slider_value();
	}

	void handle_button_collision(sf::Vector2f mouse_pos,ButtonResult left_result){
		thickness_slider.update(mouse_pos,left_result);

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

	void update_selected_color_box(sf::Color color){
		selected_color_box.setFillColor(color);
	}

	void draw(sf::RenderWindow& window){
		thickness_button.draw(window);
		thickness_slider.draw(window);
		color_button.draw(window);
		window.draw(selected_color_box);
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

	ColorPicker(){
		blue_slider = create_slider(BLUE_SLIDER_POS,BLUE_SLIDER_TRACK_SIZE,
									BLUE_SLIDER_HANDLE_SIZE,MIN_BLUE_VALUE,
									MAX_BLUE_VALUE,BLUE_SLIDER_VALUE);

		create_image();
		texture.loadFromImage(image);
		color_picker.setTexture(texture);
		color_picker.setPosition(COLOR_PICKER_POS);
		std::fill(std::begin(selected_colors), std::end(selected_colors), sf::Color(0,0,0,0));
		add_color_button = create_button(ADD_COLOR_BUTTON_POS,
										 ADD_COLOR_BUTTON_SIZE,
										 ADD_COLOR_BUTTON_TEXT);
		add_color_button.set_function([this](){selected_colors[selected_cell] = selected_color;});
	}

	void handle_button_collisions(sf::Vector2f mouse_pos,ButtonResult left_result){
		blue_slider.update(mouse_pos,left_result);
		blueValue = blue_slider.get_slider_value();
		if (add_color_button.check_collision(mouse_pos)){
			add_color_button.set_state(ButtonState::Hover);
			if (left_result.clicked){
				add_color_button.set_state(ButtonState::Click);
				add_color_button.do_function();
			}
		}
	}

	sf::Color handle_color_picking(sf::Vector2f mouse_pos,ButtonResult left_result){
		if (left_result.clicked){
			if (mouse_pos.x >= COLOR_PICKER_POS.x & mouse_pos.x <= COLOR_PICKER_WIDTH + COLOR_PICKER_POS.x){
				if (mouse_pos.y >= COLOR_PICKER_POS.y & mouse_pos.y <= COLOR_PICKER_HEIGHT + COLOR_PICKER_POS.y){
					int red = (mouse_pos.x - COLOR_PICKER_POS.x) * 255/COLOR_PICKER_WIDTH;
					int green = (mouse_pos.y - COLOR_PICKER_POS.y) * 255/COLOR_PICKER_HEIGHT;
					selected_color = sf::Color(red,green,blueValue);
					return selected_color;
				}
			}
		}
		return sf::Color(0,0,0,0);
	}

	void update_selected_colors(sf::Vector2f mouse_pos, sf::Color& stroke_color){
		float width = COLOR_GRID_WIDTH * COLOR_GRID_ROW;
		float height = COLOR_GRID_HEIGHT * COLOR_GRID_COL;
		if (mouse_pos.x >= COLOR_GRID_POS.x & mouse_pos.x <= COLOR_GRID_POS.x + width){
			if (mouse_pos.y >= COLOR_GRID_POS.y & mouse_pos.y <= COLOR_GRID_POS.y + height){
				int x = (mouse_pos.x - COLOR_GRID_POS.x) / COLOR_GRID_WIDTH;
				int y = (mouse_pos.y - COLOR_GRID_POS.y) / COLOR_GRID_HEIGHT;
				selected_cell = x*COLOR_GRID_COL + y;
				if (selected_colors[selected_cell] != sf::Color(0,0,0,0)){
					selected_color = selected_colors[selected_cell];
					stroke_color = selected_color;
				}
			}
		}
	}

	void draw_selected_colors(sf::RenderWindow& window){
		for (int i = 0; i < COLOR_GRID_ROW; i++){
			for (int j = 0; j < COLOR_GRID_COL; j++){
				sf::RectangleShape color_box(sf::Vector2f(COLOR_GRID_WIDTH,COLOR_GRID_HEIGHT));
				color_box.setPosition(COLOR_GRID_POS.x + COLOR_GRID_WIDTH*i,COLOR_GRID_POS.y + COLOR_GRID_HEIGHT*j);
				color_box.setFillColor(selected_colors[i*COLOR_GRID_COL + j]);
				if (selected_cell == i*COLOR_GRID_COL + j){
					color_box.setOutlineColor(sf::Color::Blue);
				}
				else{
					color_box.setOutlineColor(sf::Color::Black);
				}
				color_box.setOutlineThickness(1);
				window.draw(color_box);
			}
		}
	}

	void update(sf::Vector2f mouse_pos, ButtonResult left_result, sf::Color& stroke_color){
		if (left_result.clicked){
			update_selected_colors(mouse_pos,stroke_color);
		}
		create_image();
		texture.loadFromImage(image);
		color_picker.setTexture(texture);
	}

	void draw(sf::RenderWindow& window){
		draw_selected_colors(window);
		window.draw(color_picker);
		blue_slider.draw(window);
		add_color_button.draw(window);
	}

};

int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"Paint");
	// window.setFramerateLimit(1);
	sf::Clock delta_clock;
	sf::Clock fps_clock;
	double dt;

	if(!FONT.loadFromFile("assets/PoetsenOne-Regular.ttf")){
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

	if (!(window_state == WindowState::Color_Picker)){
		if (mouse_pos.x < sketch_board.get_position().x){
				window_state = WindowState::Button_UI;
		}
		else{
			window_state = WindowState::Main;
		}
	}

	auto left_result = mouse.get_button_state(sf::Mouse::Left,window);
	
	if (window_state == WindowState::Button_UI || window_state == WindowState::Color_Picker){
		buttonui.handle_button_collision(mouse_pos,left_result);
		buttonui.update_selected_color_box(stroke_color);
	}

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

	sketch_board.set_thickness(buttonui.get_thickness());
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
		color_window.update(mouse_pos,left_result,stroke_color);
		color_window.draw(window);
	}
	
	window.display();
	}

	return 0;
}