#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "constants.h"
#include "stroke.h"
#include "sketchboard.h"
#include "sidebar.h"
#include "color_picker.h"
#include "mouse_utility.h"

enum class WindowState{
        Main,
        Button_UI,
        Color_Picker
};

class Paint{   
public:
    WindowState window_state = WindowState::Main;
    SketchBoard sketchboard;
    SideBar sidebar;
    ColorPicker color_picker;
    sf::Color stroke_color;
    Mouse mouse;
    Paint(sf::Vector2f aPos, int aWidth, int aHeight);
    void handle_window_events(sf::Event event);
    void run(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};