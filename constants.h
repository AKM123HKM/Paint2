#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

constexpr int WIDTH {800};
constexpr int HEIGHT {600};
const sf::Color BG_COLOR(150,150,150);
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

sf::Font FONT;

const int SKETCHBOARD_WIDTH = WIDTH - 100;
const int SKETCHBOARD_HEIGHT = HEIGHT;
const sf::Vector2f SKETCHBOARD_POS = {100,0};

const sf::Vector2f THICKNESS_BUTTON_SIZE = {80,20};
const sf::Vector2f THICKNESS_BUTTON_POS = {10,50};
const std::string THICKNESS_BUTTON_TEXT = "Thickness";

const sf::Vector2f COLOR_BUTTON_SIZE = {80,20};
const sf::Vector2f COLOR_BUTTON_POS = {10,80};
const std::string COLOR_BUTTON_TEXT = "Color";

const int COLOR_PICKER_WIDTH = 255;
const int COLOR_PICKER_HEIGHT = 255;
const sf::Vector2f COLOR_PICKER_POS = {100,20};

const sf::Vector2f BLUE_UP_POS = {100,275};
const sf::Vector2f BLUE_UP_SIZE = {80,10};
const std::string BLUE_UP_TEXT = "/\\";

const sf::Vector2f BLUE_DOWN_POS = {100,290};
const sf::Vector2f BLUE_DOWN_SIZE = {80,10};
const std::string BLUE_DOWN_TEXT = "\\/";