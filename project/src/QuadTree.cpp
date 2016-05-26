#include "QuadTree.hpp"

int QuadTree::node_count = 0;

QuadTree::QuadTree(float size, sf::Vector3f pos) : size(size), pos(pos) {
	entity = nullptr;
	parent = nullptr;
	children[0] = nullptr;
	children[1] = nullptr;
	children[2] = nullptr;
	children[3] = nullptr;

	node_count++;
	std::cout << "CCONSTURNT" << node_count << std::endl;
}

QuadTree * QuadTree::update_node(Entity * e) {
	QuadTree * old_node = this;
	QuadTree * current_node = this;
	QuadTree * new_node = current_node->insert(e);

	if (current_node->parent == nullptr) {
		std::cout << "NO PARENT" << std::endl;
		return nullptr;
	}

	while (new_node == nullptr) {
		if (current_node->parent != nullptr) {
			current_node = current_node->parent;
		} else {
			std::cout << "OUTSIDE NODE: x=" << e->pos.x << ", z=" << e->pos.z << std::endl;
			return current_node;
		}
		new_node = current_node->insert(e);
	}
	if (new_node != old_node) {
		std::cout << "SWITCHED NODE" << std::endl;
	}
	return new_node;
}


bool QuadTree::in_node(sf::Vector3f const & p) {
	if (p.x > pos.x && p.x < pos.x + size &&
		p.z > pos.z && p.z < pos.z + size) {
		return true;
	}
	return false;
}

int QuadTree::in_quad(sf::Vector3f const & p) {
	if (p.x > pos.x && p.x < pos.x + size / 2 &&
		p.z > pos.z && p.z < pos.z + size / 2) {
		return 1;
	}
	if (p.x > pos.x + size / 2 && p.x < pos.x + size &&
		p.z > pos.z && p.z < pos.z + size / 2) {
		return 2;
	}
	if (p.x > pos.x && p.x < pos.x + size / 2 &&
		p.z > pos.z + size / 2 && p.z < pos.z + size) {
		return 3;
	}
	if (p.x > pos.x + size / 2 && p.x < pos.x + size &&
		p.z > pos.z + size / 2 && p.z < pos.z + size) {
		return 4;
	}
	return 0;
}

void QuadTree::subdivide() {
	std::cout << "subdivide " << pos.x << " " << pos.z << std::endl;
	if (children[0] == nullptr) {
		children[0] = new QuadTree(size / 2, sf::Vector3f(pos.x, 0.0, pos.z));
		children[0]->parent = this;
		std::cout << children[0]->size << std::endl;
	}
	if (children[1] == nullptr) {
		children[1] = new QuadTree(size / 2, sf::Vector3f(pos.x + size / 2, 0.0, pos.z));
		children[1]->parent = this;
		std::cout << children[1]->size << std::endl;
	}
	if (children[2] == nullptr) {
		children[2] = new QuadTree(size / 2, sf::Vector3f(pos.x, 0.0, pos.z + size / 2));
		children[2]->parent = this;
		std::cout << children[2]->size << std::endl;
	}
	if (children[3] == nullptr) {
		children[3] = new QuadTree(size / 2, sf::Vector3f(pos.x + size / 2, 0.0, pos.z + size / 2));
		children[3]->parent = this;
		std::cout << children[3]->size << std::endl;
	}
}

QuadTree * QuadTree::insert(Entity * e) {

	QuadTree * current_node = this;

	if (current_node->in_node(e->pos)) {
		if (current_node->entity != e && current_node->entity != nullptr) {
			Entity * tmp = current_node->entity;
			current_node->entity = nullptr;
			current_node->subdivide();
			e->node = current_node->children[current_node->in_quad(e->pos) - 1]->insert(e);
			tmp->node = current_node->children[current_node->in_quad(tmp->pos) - 1]->insert(tmp);
		} else {
			current_node->entity = e;
			return current_node;
		}
		return e->node;
	}
	return nullptr;
}

void QuadTree::draw() {
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
	draw_t();
	glPopMatrix();
}


void QuadTree::draw_t() {
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(pos.x, 0.0, pos.z);
		glVertex3f(pos.x + size, 0.0, pos.z);
		glVertex3f(pos.x + size, 0.0, pos.z + size);
		glVertex3f(pos.x, 0.0, pos.z + size);
	}
	glEnd();
	for (int i = 0; i < 4; i++) {
		if (children[i] != nullptr) {
			children[i]->draw_t();
		}
	}
}


