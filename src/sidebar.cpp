#include "sidebar.h"
#include "paint.h"

SideBar::SideBar(Paint& aPaint) : paint(aPaint) {
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
		color_button.set_function([this](){if(paint.window_state == WindowState::Main || paint.window_state == WindowState::Button_UI)paint.window_state = WindowState::Color_Picker;
										   else paint.window_state = WindowState::Button_UI;});

		selected_color_box.setSize(SELECTED_COLOR_BOX_SIZE);
		selected_color_box.setPosition(SELECTED_COLOR_BOX_POS);
}

float SideBar::get_thickness(){
	return thickness_slider.get_slider_value();
}

void SideBar::handle_button_collision(sf::Vector2f mouse_pos,ButtonResult left_result){
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

void SideBar::update_selected_color_box(sf::Color color){
	selected_color_box.setFillColor(color);
}

void SideBar::draw(sf::RenderWindow& window){
	thickness_button.draw(window);
	thickness_slider.draw(window);
	color_button.draw(window);
	window.draw(selected_color_box);
}