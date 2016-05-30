#include "QuadTree.hpp"
//#include <math.h>

int QuadTree::node_count = 0;

QuadTree::QuadTree(float size, sf::Vector3f pos) : size(size), pos(pos) {
	entity = nullptr;
	parent = nullptr;
	children[0] = nullptr;
	children[1] = nullptr;
	children[2] = nullptr;
	children[3] = nullptr;

	node_count++;
}

//bool QuadTree::clean() {
//	if(entity != nullptr) {
//		return false;
//	}
//	bool deletable = true;
//	for(int i = 0; i < 4; i++) {
//		if(children[i] != nullptr) {
//			if(children[i]->clean()) {
//				delete children[i];
//				children[i] = nullptr;
//			} else {
//				deletable = false;
//			}
//		}
//	}
//	return deletable;
//}
//
//QuadTree::~QuadTree() {
//}

std::vector<Entity *> QuadTree::get_neighbors(Entity * e, float dist) {
	// TODO, start search at node?
	QuadTree * current_node = e->node;
	std::vector<Entity*> result;
	while(!((e->pos.x + dist <= current_node->pos.x + current_node->size) &&  // right
		(e->pos.x - dist > current_node->pos.x) &&							  // left
		(e->pos.z + dist <= current_node->pos.z + current_node->size) &&	  // top
		(e->pos.z - dist > current_node->pos.z))) {							  // bottom
		if(current_node->parent != nullptr) {
			current_node = current_node->parent;
		} else {
			// range exceeeds the biggest quadtree node, search entire tree
			break;
		}
	}

	current_node->add_entities_in_range(e, dist, result);
	return result;
}

void QuadTree::add_entities_in_range(Entity * e, float dist, std::vector<Entity*> & res) {
	// TODO, start search at node?
	bool x1 = (abs(pos.x - e->pos.x) < dist);
	bool x2 = (abs(pos.x + size - e->pos.x) < dist);
	bool z1 = (abs(pos.z - e->pos.z) < dist);
	bool z2 = (abs(pos.z + size - e->pos.x) < dist);

	if(x1 && x2 && z1 && z2) {
		//Completly in range, return this QuadTree
		add_entites(res);
	}
	if((x1 || x2) && (z1 || z2)) {
		// atleast part of the quadtree is in range
		if(entity != nullptr) {
			add_entites(res);
			return;
		}
		for(int i = 0; i < 4; ++i) {
			if(children[i] != nullptr) {
				children[i]->add_entities_in_range(e, dist, res);
			}
		}
	}
}

void QuadTree::add_entites(std::vector<Entity*> & res){
	if(entity != nullptr) {
		res.push_back(entity);
		return;
	}
	for(size_t i = 0; i < 4; i++) {
		if(children[i] != nullptr) {
			children[i]->add_entites(res);
		}
	}
	return;
}


bool QuadTree::update_entity(Entity * e) {
	QuadTree * old_node = this;
	QuadTree * current_node = this;
	while(!current_node->insert(e)) {
		old_node->entity = nullptr;
		if(current_node->parent != nullptr) {
			current_node = current_node->parent;
		} else {
			return false;
		}
	}
	return true;
}

bool QuadTree::in_node(sf::Vector3f const & p) {
	if (p.x > pos.x && p.x <= pos.x + size &&
		p.z > pos.z && p.z <= pos.z + size) {
		return true;
	}
	return false;
}

int QuadTree::in_quad(sf::Vector3f const & p) {
	if (p.x > pos.x && p.x <= pos.x + size / 2 &&
		p.z > pos.z && p.z <= pos.z + size / 2) {
		return 1;
	}
	if (p.x > pos.x + size / 2 && p.x <= pos.x + size &&
		p.z > pos.z && p.z <= pos.z + size / 2) {
		return 2;
	}
	if (p.x > pos.x && p.x <= pos.x + size / 2 &&
		p.z > pos.z + size / 2 && p.z <= pos.z + size) {
		return 3;
	}
	if (p.x > pos.x + size / 2 && p.x <= pos.x + size &&
		p.z > pos.z + size / 2 && p.z <= pos.z + size) {
		return 4;
	}
	return 0;
}

void QuadTree::subdivide() {
	if (children[0] == nullptr) {
		children[0] = new QuadTree(size / 2, sf::Vector3f(pos.x, 0.0, pos.z));
		children[0]->parent = this;
	}
	if (children[1] == nullptr) {
		children[1] = new QuadTree(size / 2, sf::Vector3f(pos.x + size / 2, 0.0, pos.z));
		children[1]->parent = this;
	}
	if (children[2] == nullptr) {
		children[2] = new QuadTree(size / 2, sf::Vector3f(pos.x, 0.0, pos.z + size / 2));
		children[2]->parent = this;
	}
	if (children[3] == nullptr) {
		children[3] = new QuadTree(size / 2, sf::Vector3f(pos.x + size / 2, 0.0, pos.z + size / 2));
		children[3]->parent = this;
	}
}

bool QuadTree::insert(Entity * e) {
	if(in_node(e->pos)) {
		if(entity == e) {
			return true;
		}
		if(entity == nullptr && children[0] == nullptr) {
			entity = e;
			e->node = this;
			return true;
		}
		if(entity == nullptr && children[0] != nullptr) {
			return children[in_quad(e->pos) - 1]->insert(e);
		}
		Entity * tmp = entity;
		entity = nullptr;
		subdivide();
		if(!children[in_quad(e->pos) - 1]->insert(e)) {
			//TODO should never happen
		}
		if(!children[in_quad(tmp->pos) - 1]->insert(tmp)) {
			//TODO should never happen
		}
		return true;
	}
	return false;
}

void QuadTree::draw() {
	glPushMatrix();
	glLoadIdentity();
	draw_t();
	glPopMatrix();
}

void QuadTree::draw_t() {
	float y = 0.0;
	
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(pos.x, y, pos.z);
		glVertex3f(pos.x + size, y, pos.z);
		glVertex3f(pos.x + size, y, pos.z + size);
		glVertex3f(pos.x, y, pos.z + size);
	}
	glEnd();
	if(entity != nullptr) {
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(entity->pos.x - 2.0, 0.0, entity->pos.z - 2.0);
			glVertex3f(entity->pos.x + 2.0, 0.0, entity->pos.z - 2.0);
			glVertex3f(entity->pos.x + 2.0, 0.0, entity->pos.z + 2.0);
			glVertex3f(entity->pos.x - 2.0, 0.0, entity->pos.z + 2.0);
		}
		glEnd();
		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(pos.x + size/2, 0.0, pos.z + size/2);
			glVertex3f(entity->pos.x, 0.0, entity->pos.z);
		}
		glEnd();
	}
	for(int i = 0; i < 4; i++) {
		if(children[i] != nullptr) {
			children[i]->draw_t();
		}
	}
}
