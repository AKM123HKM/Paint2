#include "constants.h"
#include "ui_design.h"

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
					 int min_value, int max_value ,float slider_value){
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