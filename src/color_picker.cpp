#include "color_picker.h"

ColorPicker::ColorPicker(){
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

void ColorPicker::handle_button_collisions(sf::Vector2f mouse_pos,ButtonResult left_result){
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

sf::Color ColorPicker::handle_color_picking(sf::Vector2f mouse_pos,ButtonResult left_result){
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

void ColorPicker::update_selected_colors(sf::Vector2f mouse_pos, sf::Color& stroke_color){
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

void ColorPicker::draw_selected_colors(sf::RenderWindow& window){
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

void ColorPicker::update(sf::Vector2f mouse_pos, ButtonResult left_result, sf::Color& stroke_color){
	if (left_result.clicked){
		update_selected_colors(mouse_pos,stroke_color);
	}
	create_image();
	texture.loadFromImage(image);
	color_picker.setTexture(texture);
}

void ColorPicker::draw(sf::RenderWindow& window){
	draw_selected_colors(window);
	window.draw(color_picker);
	blue_slider.draw(window);
	add_color_button.draw(window);
}