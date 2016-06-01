#pragma once
#include <iostream>
#include "SFML\Graphics.hpp"
#include "Entity\Entity.hpp"
#include <vector>

struct Entity;

struct QuadTree {
	static int node_count;

	Entity * entity;

	float size;
	sf::Vector3f pos;

	QuadTree * parent;
	QuadTree * children[4];
	bool removable;
	QuadTree(float size, sf::Vector3f pos = sf::Vector3f(0.0, 0.0, 0.0));
	
	//~QuadTree();


	bool update_entity(Entity * e);
	bool in_node(sf::Vector3f const & p);
	int in_quad(sf::Vector3f const & p);
	void subdivide(int i);
	void subdivide();

	bool insert(Entity * e);
	void add_entities_in_range(Entity * e, float dist, std::vector<Entity*> & res);
	void add_entities(std::vector<Entity*> & res, Entity * excluded_entity = nullptr);

	void remove();
	void remove_children();
	bool has_child();

	void get_neighbors(Entity * e, float dist, std::vector<Entity*>& result);

	void draw();
	void draw_p();
	void draw_r();

	//void clean();
	//bool hasSiblings();
};
