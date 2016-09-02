#include "QuadTree.hpp"

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
	old_node->entity = nullptr;
	e->node = nullptr;
	while(!current_node->insert(e)) {
		// insert failes, move one step up
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
		if(entity == e && e->node == this) {
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
		subdivide(in_quad(tmp->pos) - 1);

		//TODO should never happen
		if(!children[in_quad(e->pos) - 1]->insert(e)) {
			std::cerr << "Something is very wrong! " << std::endl;
			std::cerr << "size: "  << size << std::endl;
			std::cerr << "child size: " << children[in_quad(tmp->pos) - 1]->size << std::endl;
		}
		//TODO should never happen
		if(!children[in_quad(tmp->pos) - 1]->insert(tmp)) {
			std::cerr << "Something is very wrong 2! " << std::endl;
			std::cerr << "size: " << size << std::endl;
			std::cerr << "child size: " << children[in_quad(tmp->pos) - 1]->size << std::endl;
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
	//TODO add flag to print the possesion line
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
