#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

enum class MouseStates{
	Idle,
	Pressed,
	Held,
	Dragging
};

enum class MouseEvents{
	None,
	Double_Click,
	Click,
	Release_Transition,
	Press_Transition
};

struct MouseButton{
	sf::Clock press_timer;
	sf::Clock click_timer;
	bool was_pressed{false};
	bool is_pressed{false};
	MouseStates state = MouseStates::Idle;
	MouseEvents event = MouseEvents::None;
	float drage_value{0.0f};
};

class Mouse{
private:
	MouseButton left_button;
	MouseButton right_button;
	MouseButton middle_button;
	sf::Vector2f current_mouse_pos;
	sf::Vector2f prev_mouse_pos;
	const double HOLD_THRESHOLD = 0.5;
	const double DRAG_THRESHOLD = 4.0;
	const double DOUBLE_CLICK_THRESHOLD = 1;

	MouseButton& check_button(sf::Mouse::Button button);

	float magnitude(sf::Vector2f vec);

public:

	sf::Vector2f get_mouse_position(sf::RenderWindow& window);
	void updateMousePosition(sf::RenderWindow& window);
	void updateButton(sf::Mouse::Button);
	MouseEvents get_button_event(sf::Mouse::Button);
	MouseStates get_button_state(sf::Mouse::Button);
};