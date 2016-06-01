#include "QuadTree.hpp"
//#include <math.h>

int QuadTree::node_count = 0;

QuadTree::QuadTree(float size, sf::Vector3f pos) : size(size), pos(pos), removable(false) {
	entity = nullptr;
	parent = nullptr;
	children[0] = nullptr;
	children[1] = nullptr;
	children[2] = nullptr;
	children[3] = nullptr;

	node_count++;
}

//Quad calling this must be a leaf node without children and without an entity
//Set it as removable
//Set all its parents without children to removable
void QuadTree::remove() {
	QuadTree * current_node = this;
	//If current node has no children
	while(!current_node->has_child()) {
		//And is not root
		if(current_node->parent != nullptr) {
			//Mark as removable
			current_node->removable = true;
			//Go to parent
			current_node = current_node->parent;
			//And remove it
			current_node->remove_children();
		} else {
			//Is root
			break;
		}
	}
	//If has children, remove all removable children
	current_node->remove_children();
}

//Remove all children marked as removable
void QuadTree::remove_children() {
	for(size_t i = 0; i < 4; i++) {
		if(children[i] != nullptr && children[i]->removable) {
			delete children[i];
			children[i] = nullptr;
		}
	}
}

//Returns true if has atleast one child
bool QuadTree::has_child() {
	for(size_t i = 0; i < 4; i++) {
		if(children[i] != nullptr) {
			return true;
		}
	}
	return false;
}

void QuadTree::get_neighbors(Entity * e, float dist, std::vector<Entity*> & result) {
	// TODO, start search at node?
	QuadTree * current_node = e->node;
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
	return;
}


bool circle_intersect_rect(sf::Vector3f cpos, float radius, sf::Vector3f rpos, float size, bool rect_centered = false) {
	//Rect is centered (pos is in the middle)
	if(rect_centered) {
		//TODO
	}
	//Rect is not centered (pos is top left corner)
	else {
		//TODO hot code
		//Circle center is inside rect in x direction
		if(cpos.x > rpos.x &&
			cpos.x <= rpos.x + size) {

			//Circle center is inside rect in z direction
			if(cpos.z > rpos.z &&
				cpos.z <= rpos.z + size) {
				return true;
			}
			//Check distance in z direction
			if(abs(cpos.z - (rpos.z)) < radius || abs(cpos.z - (rpos.z + size)) < radius) {
				return true;
			}
		}
		//Circle center is inside rect in z direction
		if(cpos.z > rpos.z &&
			cpos.z <= rpos.z + size) {
			//Check distance in x direction
			if(abs(cpos.x - (rpos.x)) < radius || abs(cpos.x - (rpos.x + size)) < radius) {
				return true;
			}
		}
		//Check corners
		float n = radius*radius;
		bool h1 = (cpos.x - (rpos.x))			* (cpos.x - (rpos.x)) + (cpos.z - (rpos.z))			* (cpos.z - (rpos.z))		 < n;
		bool h2 = (cpos.x - (rpos.x + size))	* (cpos.x - (rpos.x + size)) + (cpos.z - (rpos.z))			* (cpos.z - (rpos.z))		 < n;
		bool h3 = (cpos.x - (rpos.x))			* (cpos.x - (rpos.x)) + (cpos.z - (rpos.z + size))	* (cpos.z - (rpos.z + size)) < n;
		bool h4 = (cpos.x - (rpos.x + size))	* (cpos.x - (rpos.x + size)) + (cpos.z - (rpos.z + size))	* (cpos.z - (rpos.z + size)) < n;
		if(h1 || h2 || h3 || h4) {
			return true;
		}
	}
	return false;
}




void QuadTree::add_entities_in_range(Entity * e, float dist, std::vector<Entity*> & res) {
	draw_p();
	//float n = dist*dist;
	//bool h1 = (e->pos.x - (pos.x)) * (e->pos.x - (pos.x))				+ (e->pos.z - (pos.z)) * (e->pos.z - (pos.z)) < n;
	//bool h2 = (e->pos.x - (pos.x + size)) * (e->pos.x - (pos.x + size)) + (e->pos.z - (pos.z)) * (e->pos.z - (pos.z)) < n;
	//bool h3 = (e->pos.x - (pos.x)) * (e->pos.x - (pos.x))				+ (e->pos.z - (pos.z + size)) * (e->pos.z - (pos.z + size)) < n;
	//bool h4 = (e->pos.x - (pos.x + size)) * (e->pos.x - (pos.x + size)) + (e->pos.z - (pos.z + size)) * (e->pos.z - (pos.z + size)) < n;

	////Completely in range, add all entities
	//if(h1 && h2 && h3 && h4) {
	//	add_entities(res);
	//	return;
	//}
	//Atleast part of the quadtree is in range, check all children
	//if(h1 || h2 || h3 || h4) {
	//if((x1 || x2) && (z1 || z2)) {
	//if(circle_intersect_rect()) {
	if(circle_intersect_rect(e->pos, dist, pos, size)) {

		if(entity != nullptr) {
			add_entities(res, e);
			return;
		}
		for(int i = 0; i < 4; ++i) {
			if(children[i] != nullptr) {
				children[i]->add_entities_in_range(e, dist, res);
			}
		}
	}

	//The Quad encapsulates the range fully, add entity ~or~ check all children
	//if(e->pos.x > pos.x &&
	//	e->pos.x <= pos.x + size &&
	//	e->pos.z > pos.z &&
	//	e->pos.z <= pos.z + size) {
	//	if(entity != nullptr) {
	//		add_entities(res);
	//		return;
	//	}
	//	for(int i = 0; i < 4; ++i) {
	//		if(children[i] != nullptr) {
	//			children[i]->add_entities_in_range(e, dist, res);
	//		}
	//	}
	//}
}

void QuadTree::add_entities(std::vector<Entity*> & res, Entity * excluded_entity){
	draw_p();
	if(entity != nullptr) {
		if(entity == excluded_entity) {
			return;
		}
		res.push_back(entity);
		return;
	}
	for(size_t i = 0; i < 4; i++) {
		if(children[i] != nullptr) {
			children[i]->add_entities(res, excluded_entity);
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
			old_node->remove();
			return false;
		}
	}
	if(current_node != old_node) {
		old_node->remove();
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

//TODO make more beautiful
void QuadTree::subdivide(int i) {
	if(i == 0) {
		if(children[0] == nullptr) {
			children[0] = new QuadTree(size / 2, sf::Vector3f(pos.x, 0.0, pos.z));
			children[0]->parent = this;
		}
	} else if(i == 1) {
		if(children[1] == nullptr) {
			children[1] = new QuadTree(size / 2, sf::Vector3f(pos.x + size / 2, 0.0, pos.z));
			children[1]->parent = this;
		}
	} else if(i == 2) {
		if(children[2] == nullptr) {
			children[2] = new QuadTree(size / 2, sf::Vector3f(pos.x, 0.0, pos.z + size / 2));
			children[2]->parent = this;
		}
	} else if(i == 3) {
		if(children[3] == nullptr) {
			children[3] = new QuadTree(size / 2, sf::Vector3f(pos.x + size / 2, 0.0, pos.z + size / 2));
			children[3]->parent = this;
		}
	}
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
		if(entity == nullptr) {
			//Has no children
			if(!has_child()) {
				entity = e;
				e->node = this;
				return true;
			} else {
				subdivide(in_quad(e->pos) - 1);
				return children[in_quad(e->pos) - 1]->insert(e);
			}
		}
		Entity * tmp = entity;
		entity = nullptr;
		subdivide(in_quad(e->pos) - 1);
		if(!children[in_quad(e->pos) - 1]->insert(e)) {
			//TODO should never happen
		}
		subdivide(in_quad(tmp->pos) - 1);
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
	draw_r();
	glPopMatrix();
}

void QuadTree::draw_p() {
	float y = 1.0;
	glPushMatrix();
	glLoadIdentity();
	//glColor3f(1.0, 0.0, 1.0);
	glColor3ub(231, 57, 83);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(pos.x, y, pos.z);
		glVertex3f(pos.x + size, y, pos.z);
		glVertex3f(pos.x + size, y, pos.z + size);
		glVertex3f(pos.x, y, pos.z + size);
	}
	glEnd();
	glPopMatrix();
}

void QuadTree::draw_r() {
	float y = 0.0;
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(pos.x, y, pos.z);
		glVertex3f(pos.x + size, y, pos.z);
		glVertex3f(pos.x + size, y, pos.z + size);
		glVertex3f(pos.x, y, pos.z + size);
	}
	glEnd();
	//if(entity != nullptr) {
	//	glColor3f(0.0, 1.0, 1.0);
	//	glBegin(GL_LINES);
	//	{
	//		glVertex3f(pos.x + size/2, 0.0, pos.z + size/2);
	//		glVertex3f(entity->pos.x, 0.0, entity->pos.z);
	//	}
	//	glEnd();
	//}
	for(int i = 0; i < 4; i++) {
		if(children[i] != nullptr) {
			children[i]->draw_r();
		}
	}
}

//void QuadTree::clean() {
//	if(entity == nullptr) {
//		// no entity, either has children or removable
//		bool canRemove = true;
//		for(size_t i = 0; i < 4; i++) {
//			if(children[i] != nullptr) {
//				children[i]->clean();
//				canRemove = false;
//			}
//		}
//		// No entity and no children, dead leaf
//		if(canRemove) {
//			std::cout << "removed node" << std::endl;
//			delete this;
//		}
//	} else {
//		// has entity, test if we can relocate up one level
//		if(parent != nullptr && !hasSiblings()) {
//			// Move entity up and delete the children
//			std::cout << "Moved up entity" << std::endl;
//			parent->entity = entity;
//			for(size_t i = 0; i < 4; i++) {
//				if(parent->children[i] != nullptr) {
//					delete children[i];
//				}
//			}
//			// test if we can continue to move the entity up
//			//parent->clean();
//		}
//	}
//}

//bool QuadTree::hasSiblings() {
//	if(parent != nullptr) {
//		for(size_t i = 0; i < 4; i++) {
//			if(parent->children[i] != nullptr && parent->children[i] != this)
//				return true;
//		}
//		return false;
//	}
//	return true;
//}

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
