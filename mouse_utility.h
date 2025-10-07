#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

struct MouseState{
	bool is_pressed{false};
	bool was_pressed{false};
	bool click_consumed{false};
	sf::Clock press_timer;
	sf::Vector2f prev_mouse_pos{-1,-1};
};

struct ButtonResult{
	bool clicked{false};
	bool pressed{false};
	bool held{false};
	bool released{false};
	bool dragging{false};
	bool double_clicked{false};
	bool press_transition{false};
	bool release_transition{false};
};

class Mouse{
private:
	MouseState left_state;
	MouseState right_state;
	MouseState middle_state;
	sf::Clock click_timer;
	const double HOLD_THRESHOLD = 0.5;
	const double DRAG_THRESHOLD = 4.0;
	const double DOUBLE_CLICK_THRESHOLD = 1;

	MouseState& check_button(sf::Mouse::Button button);

	float magnitude(sf::Vector2f vec);

public:

	sf::Vector2f get_mouse_position(sf::RenderWindow& window);

	ButtonResult get_button_state(sf::Mouse::Button button,sf::RenderWindow& window);
};