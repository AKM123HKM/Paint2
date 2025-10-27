#include "sketchboard.h"
#include "paint.h"
#include "ui_design.h"

SketchBoard::SketchBoard(sf::Vector2f aPos, int aWidth, int aHeight){
	    pos = aPos;
		width = aWidth;
		height = aHeight;
		Stroke stroke(stroke_thickness);
		strokes.push_back(stroke);
}

bool SketchBoard::check_dimensions(sf::Vector2f mouse_pos){
		if (mouse_pos.x >= pos.x && mouse_pos.x <= width+pos.x){
			if (mouse_pos.y >= pos.y && mouse_pos.y <= height+pos.y){
				return true;
			}
		}
		return false;	
	}

sf::Vector2f SketchBoard::get_dimensions(){
		return sf::Vector2f(width,height);
	}

sf::Vector2f SketchBoard::get_position(){
		return pos;
	}

void SketchBoard::add_stroke(){
		if (strokes.back().get_vertex_count() != 0){
			Stroke stroke(stroke_thickness);
			strokes.push_back(stroke);
		}
	}

void SketchBoard::draw(sf::RenderWindow& window){
		int size = 0;
		for (auto stroke:strokes){
			stroke.draw(window);
		}
	}

void SketchBoard::add_vertex(sf::Vector2f mouse_pos,sf::Color color){
		strokes.back().add_vertex(mouse_pos,color);
	}

void SketchBoard::add_click_vertex(sf::Vector2f mouse_pos,sf::Color color){
		strokes.back().add_click_vertex(mouse_pos,color);
	}

void SketchBoard::change_thickness(int delta){
		stroke_thickness += delta;
		stroke_thickness = std::clamp(stroke_thickness,static_cast<float>(MIN_THICKNESS),static_cast<float>(MAX_THICKNESS));
		strokes.back().change_thickness(stroke_thickness);
	}

void SketchBoard::set_thickness(float thickness){
		stroke_thickness = thickness;
		stroke_thickness = std::clamp(stroke_thickness,static_cast<float>(MIN_THICKNESS),static_cast<float>(MAX_THICKNESS));
		strokes.back().change_thickness(stroke_thickness);
	}

int SketchBoard::get_strokes_count(){
		return strokes.size();
	}