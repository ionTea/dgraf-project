#pragma once
#include "SFML\Graphics.hpp"
#include "QuadTree.hpp"
#include <Windows.h>
#include <gl\GL.h>

struct QuadTree;

struct Entity {
	static QuadTree * root_node;
	QuadTree * node;

	sf::Vector3f pos, vel;
	sf::Vector2f rot;//x = mouse x delta (rotation around y-axis), y = mouse y delta (rotation around the x-axis)
	sf::Vector3f direction, left;

	Entity(sf::Vector3f pos = sf::Vector3f(0.0, 0.0, 0.0), sf::Vector3f vel = sf::Vector3f(0.0, 0.0, 0.0));
	virtual void update(float elapsed_time);
	virtual void draw();
	void update_directions();
};
