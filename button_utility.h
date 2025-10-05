#pragma once

class Button{
private:
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

	sf::Vector2f get_position();
	void get_state(std::string aState);
	void set_position(sf::Vector2f pos);
	sf::Vector2f get_size();
	void set_size(sf::Vector2f size);
	void set_text(std::string aText);
	void set_text_position(sf::Vector2f pos = sf::Vector2f(-1,-1));
	void set_text_font(sf::Font& font);
	void set_normal_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(20,20,20));
	void set_hover_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(50,50,50));
	void set_click_colors(sf::Color color, sf::Color outline_color, sf::Color text_color = sf::Color(120,120,120));
	bool check_collision(sf::Vector2f mouse_pos);
	void set_function(std::function<void()> func);
	void do_function();
	void draw(sf::RenderWindow& window);
}