#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

constexpr int WIDTH {800};
constexpr int HEIGHT {600};
const sf::Color BG_COLOR(150,150,150);
const sf::Color GREEN_COLOR{0,255,0};
const sf::Color RED_COLOR{255,0,0};

// Normal state
const sf::Color NORMAL_BUTTON_COLOR{120, 120, 120};
const sf::Color NORMAL_OUTLINE_COLOR{140, 140, 140};
const sf::Color NORMAL_TEXT_COLOR{220, 220, 220};

// Hover state
const sf::Color HOVER_BUTTON_COLOR{160, 160, 160};
const sf::Color HOVER_OUTLINE_COLOR{180, 180, 180};
const sf::Color HOVER_TEXT_COLOR{0, 0, 0};  // dark text visible on lighter hover

// Click state
const sf::Color CLICK_BUTTON_COLOR{100, 100, 100};
const sf::Color CLICK_OUTLINE_COLOR{120, 120, 120};
const sf::Color CLICK_TEXT_COLOR{255, 255, 255};  // bright text visible on dark click

const sf::Color INVALID_COLOR{0,0,0,0};

extern sf::Font FONT;

const int SKETCHBOARD_WIDTH = WIDTH - 100;
const int SKETCHBOARD_HEIGHT = HEIGHT;
const sf::Vector2f SKETCHBOARD_POS = {100,0};

const sf::Vector2f THICKNESS_BUTTON_SIZE = {80,20};
const sf::Vector2f THICKNESS_BUTTON_POS = {10,50};
const std::string THICKNESS_BUTTON_TEXT = "Thickness";

const sf::Vector2f THICKNESS_SLIDER_POS = {10,80};
const sf::Vector2f THICKNESS_SLIDER_SIZE = {80,20};
const sf::Vector2f THICKNESS_SLIDER_HANDLE_SIZE = {20,20};
const int MIN_THICKNESS = 5;
const int MAX_THICKNESS = 50;
const float DEFAULT_THICKNESS = 10;

const sf::Vector2f COLOR_BUTTON_SIZE = {80,20};
const sf::Vector2f COLOR_BUTTON_POS = {10,110};
const std::string COLOR_BUTTON_TEXT = "Color";

const sf::Vector2f SELECTED_COLOR_BOX_SIZE = {80,20};
const sf::Vector2f SELECTED_COLOR_BOX_POS = {10,140};

const int COLOR_PICKER_WIDTH = 255;
const int COLOR_PICKER_HEIGHT = 255;
const sf::Vector2f COLOR_PICKER_POS = {100,20};

//BLUE VALUE SLIDER
const sf::Vector2f BLUE_SLIDER_POS = {100,280};
const sf::Vector2f BLUE_SLIDER_TRACK_SIZE = {255,15};
const sf::Vector2f BLUE_SLIDER_HANDLE_SIZE = {15,15};
const int MIN_BLUE_VALUE = 0;
const int MAX_BLUE_VALUE = 255;
const float BLUE_SLIDER_VALUE = 10;

const int COLOR_GRID_WIDTH = 20;
const int COLOR_GRID_HEIGHT = 20;
const int COLOR_GRID_ROW = COLOR_PICKER_WIDTH/COLOR_GRID_WIDTH;
const int COLOR_GRID_COL = 2;
const sf::Vector2f COLOR_GRID_POS = {COLOR_PICKER_POS.x + (COLOR_PICKER_WIDTH - (COLOR_GRID_ROW * COLOR_GRID_WIDTH))/2,
									 BLUE_SLIDER_POS.y + BLUE_SLIDER_TRACK_SIZE.y + 5};

const sf::Vector2f ADD_COLOR_BUTTON_SIZE = {COLOR_GRID_WIDTH*COLOR_GRID_ROW - 10,20};
const sf::Vector2f ADD_COLOR_BUTTON_POS = {COLOR_GRID_POS.x + 5,COLOR_GRID_POS.y + COLOR_GRID_HEIGHT*COLOR_GRID_COL + 5};
const std::string ADD_COLOR_BUTTON_TEXT = "Add";

// SLIDERS
const int TRACK_SIZE_OFFSET = 4;
const sf::Color HANDLE_HOVER_COLOR = HOVER_BUTTON_COLOR;
const sf::Color HANDLE_CLICK_COLOR = CLICK_BUTTON_COLOR;
const sf::Color HANDLE_NORMAL_COLOR = NORMAL_BUTTON_COLOR;