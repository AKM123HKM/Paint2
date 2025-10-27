#include "stroke.h"

Stroke::Stroke(float aThickness):points(sf::Quads){
	thickness = aThickness;
}

void Stroke::add_vertex(sf::Vector2f pos, sf::Color color){
		if (temp_switch){
			temp_pos = pos;
			temp_switch = !temp_switch;
		}
		else{
			sf::Vector2f dir = temp_pos - pos;
			float magnitude = sqrt((dir.x * dir.x) + (dir.y * dir.y));
			if (!(magnitude == 0)){
				/* This is a normalized perpendicular vector to the direction vector of the previous
					and the current position/vertex */
				sf::Vector2f normalized_dir = dir / magnitude;
				sf::Vector2f normalized_perp(-normalized_dir.y,normalized_dir.x);
				sf::Vector2f offset = normalized_perp * (thickness/2.f);

				sf::Vector2f offset_point_one = temp_pos - offset;
				sf::Vector2f offset_point_two = temp_pos + offset;
				sf::Vector2f offset_point_three = pos + offset;
				sf::Vector2f offset_point_four = pos - offset;

				if (points.getVertexCount() == 0){
					points.append(sf::Vertex(offset_point_one,color));
					points.append(sf::Vertex(offset_point_two,color));
				}
				else{
					points.append(sf::Vertex(offset_point_one,color));
					points.append(sf::Vertex(offset_point_two,color));
					points.append(sf::Vertex(offset_point_one,color));
					points.append(sf::Vertex(offset_point_two,color));
				}
				
				points.append(sf::Vertex(offset_point_three,color));
				points.append(sf::Vertex(offset_point_four,color));
				points.append(sf::Vertex(offset_point_three,color));
				points.append(sf::Vertex(offset_point_four,color));

				temp_switch = !temp_switch;
			}
		}
	}

void Stroke::add_click_vertex(sf::Vector2f pos, sf::Color color){
		auto offset = thickness/2;
		auto offset_point_one = sf::Vector2f(pos.x - offset, pos.y - offset);
		auto offset_point_two = sf::Vector2f(pos.x + offset, pos.y - offset);
		auto offset_point_three = sf::Vector2f(pos.x + offset, pos.y + offset);
		auto offset_point_four = sf::Vector2f(pos.x - offset, pos.y + offset);

		points.append(sf::Vertex(offset_point_one,color));
		points.append(sf::Vertex(offset_point_two,color));
		points.append(sf::Vertex(offset_point_three,color));
		points.append(sf::Vertex(offset_point_four,color));
	}

void Stroke::draw(sf::RenderWindow& window){
		int size = points.getVertexCount();
		window.draw(points);
	}

void Stroke::change_thickness(float aThickness){
		thickness = aThickness;
	}

int Stroke::get_vertex_count(){
		return points.getVertexCount();
	}