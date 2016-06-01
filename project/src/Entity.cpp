#include "Entity\Entity.hpp"


QuadTree * Entity::root_node = nullptr;

Entity::Entity(sf::Vector3f pos, sf::Vector3f vel) :
		pos(pos), vel(vel), rot(0.0, 0.0), direction(0.0, 0.0, 1.0), left(-1.0, 0.0, 0.0) {
}

void Entity::update(float elapsed_time) {
	pos.x += vel.x * elapsed_time;
	pos.y += vel.y * elapsed_time;
	pos.z += vel.z * elapsed_time;
}

void Entity::draw() {}

void Entity::update_position(float elapsed_time) {
	pos.x += vel.x * elapsed_time;
	pos.y += vel.y * elapsed_time;
	pos.z += vel.z * elapsed_time;
}

void Entity::update_directions() {
	//Calculate and get direction with opengl
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(rot.x, 0.0, 1.0, 0.0);
	glRotatef(rot.y, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1.0);
	float matrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	glPopMatrix();

	direction.x = -matrix[12];
	direction.y = -matrix[13];
	direction.z = matrix[14];
	//Left is normal of y-axis and direction
	float length;
	length = sqrt(direction.z*direction.z + direction.x*direction.x);
	left.x = -direction.z / length;
	left.y = 0.0;
	left.z = direction.x / length;
}