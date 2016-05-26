#pragma once
#include "Entity\Entity.hpp"

struct QuadTree {
	sf::Vector3f pos;
	float size;
	Entity * entity;
	QuadTree * parent;
	QuadTree * children[4];

	QuadTree(sf::Vector3f pos, float size) : pos(pos), size(size) {
		entity = nullptr;
		parent = nullptr;
		children[0] = nullptr;
		children[1] = nullptr;
		children[2] = nullptr;
		children[3] = nullptr;
		std::cout << "Created Quad Node" << std::endl;
	}

	bool in_node(sf::Vector3f const & p) {
		if (p.x > pos.x && p.x < pos.x + size && 
			p.y > pos.y && p.y < pos.y + size) {
			return true;
		}
		return false;
	}

	int in_quad(sf::Vector3f const & p) {
		if (p.x > pos.x && p.x < pos.x + size / 2 &&
			p.y > pos.y && p.y < pos.y + size / 2) {
			return 1;
		}
		if (p.x > pos.x + size / 2 && p.x < pos.x + size &&
			p.y > pos.y && p.y < pos.y + size / 2) {
			return 2;
		}
		if (p.x > pos.x && p.x < pos.x + size / 2 &&
			p.y > pos.y + size / 2 && p.y < pos.y + size) {
			return 3;
		}
		if (p.x > pos.x + size / 2 && p.x < pos.x + size &&
			p.y > pos.y + size / 2 && p.y < pos.y + size) {
			return 4;
		}
		return 0;
	}

	void subdivide() {
		std::cout << "Subdivide" << std::endl;

		if(children[0] == nullptr)
			children[0] = new QuadTree(sf::Vector3f(pos.x,				pos.y, 0.0), size / 2);

		if (children[1] == nullptr)
			children[1] = new QuadTree(sf::Vector3f(pos.x + size / 2,	pos.y, 0.0), size / 2);

		if (children[2] == nullptr)
			children[2] = new QuadTree(sf::Vector3f(pos.x,				pos.y + size / 2, 0.0), size / 2);

		if (children[3] == nullptr)
			children[3] = new QuadTree(sf::Vector3f(pos.x + size / 2,	pos.y + size / 2, 0.0), size / 2);
	}

	void insert(Entity * e) {
		std::cout << "insert" << std::endl;

		QuadTree * current_node = this;
		/*while (!current_node->in_node(e->pos)) {
			if (current_node->parent != nullptr) {
				current_node = current_node->parent;
			} else {
				//Create parent
				sf::Vector3f pos;
				if (e->pos.x > current_node->pos.x) {
					pos.x = current_node->pos.x + current_node->size * 2;
				} else {
					pos.x = current_node->pos.x - current_node->size;
				}
				if (e->pos.y > current_node->pos.x) {
					pos.y = current_node->pos.y + current_node->size * 2;
				} else {
					pos.y = current_node->pos.y - current_node->size;
				}
				current_node->parent = new QuadTree(pos, current_node->size * 2);
				//Create parent children
				current_node = current_node->parent;
			}
		}*/

		if (current_node->in_node(e->pos)) {
			std::cout << "INSIDE" << std::endl;
			if (current_node->entity != nullptr) {
				std::cout << (current_node->in_quad(e->pos) - 1) << std::endl;
				Entity * tmp = current_node->entity;
				std::cout << "	removed entity from quad" << std::endl;
				current_node->entity = nullptr;
				current_node->subdivide();
				current_node->children[current_node->in_quad(e->pos) - 1]->insert(e);
				current_node->children[current_node->in_quad(tmp->pos) - 1]->insert(tmp);
			} else {
				std::cout << "	added entity to quad" << std::endl;
				current_node->entity = e;
			}
		}
		else {
			std::cout << "NOT INSIDE" << std::endl;
		}
	}
};
