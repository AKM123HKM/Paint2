#include "paint.h"

Paint::Paint(sf::Vector2f aPos, int aWidth, int aHeight) : sketchboard(aPos, aWidth, aHeight), sidebar(*this) {
}

void Paint::handle_window_events(sf::Event event){
    if (window_state == WindowState::Main)
				if (event.type == sf::Event::MouseWheelScrolled) {
				    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
				        float delta = event.mouseWheelScroll.delta;
				        sketchboard.change_thickness(delta);
				    }
				}
}

void Paint::run(sf::RenderWindow& window){
    sf::Vector2i i_pos = sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos = sf::Vector2f(i_pos);
    if (!(window_state == WindowState::Color_Picker)){
		if (mouse_pos.x < sketchboard.get_position().x){
				window_state = WindowState::Button_UI;
		}
		else{
			window_state = WindowState::Main;
		}
	}

	auto left_result = mouse.get_button_state(sf::Mouse::Left,window);
	
	if (window_state == WindowState::Button_UI || window_state == WindowState::Color_Picker){
		sidebar.handle_button_collision(mouse_pos,left_result);
		sidebar.update_selected_color_box(stroke_color);
	}

	if (window_state == WindowState::Main)
		if (sketchboard.check_dimensions(mouse_pos)){
			if (left_result.clicked){
				// std::cout << "Click" << std::endl;
				sketchboard.add_click_vertex(mouse_pos,stroke_color);
			}
			if (left_result.dragging){
				// std::cout << "dragging" << std::endl;
				sketchboard.add_vertex(mouse_pos,stroke_color);
			}
			if (left_result.release_transition){
				// std::cout << "Added stroke" << std::endl;
				sketchboard.add_stroke();
			}
		}
		// In case mouse gets out of bounds of sketch board add a new stroke
		else if (left_result.pressed){
			sketchboard.add_stroke();
		}
        sketchboard.set_thickness(sidebar.get_thickness());

    if(window_state == WindowState::Color_Picker){
		color_picker.handle_button_collisions(mouse_pos,left_result);
		sf::Color color = color_picker.handle_color_picking(mouse_pos,left_result);
		if (!(color == INVALID_COLOR)){
			stroke_color = color;
		}
		color_picker.update(mouse_pos,left_result,stroke_color);
    }
}

void Paint::draw(sf::RenderWindow& window){
    sketchboard.draw(window);
    sidebar.draw(window);
    if(window_state == WindowState::Color_Picker){
        color_picker.draw(window);
    }
}