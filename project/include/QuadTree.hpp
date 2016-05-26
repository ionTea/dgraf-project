#pragma once
#include <iostream>
#include "SFML\Graphics.hpp"
#include "Entity\Entity.hpp"

struct Entity;

struct QuadTree {
	static int node_count;

	Entity * entity;

	float size;
	sf::Vector3f pos;

	QuadTree * parent;
	QuadTree * children[4];

	QuadTree(float size, sf::Vector3f pos = sf::Vector3f(0.0, 0.0, 0.0));

	QuadTree * update_node(Entity * e);
	bool in_node(sf::Vector3f const & p);
	int in_quad(sf::Vector3f const & p);
	void subdivide();
	QuadTree * insert(Entity * e);

	void draw();

	void draw_t();
};
