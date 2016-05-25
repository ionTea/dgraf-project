#pragma once
#include "SFML\Graphics.hpp"

struct Entity {
	sf::Vector3f pos, vel;
	sf::Vector2f rot;//x = mouse x delta (rotation around y-axis), y = mouse y delta (rotation around the x-axis)
	sf::Vector3f direction, left;
	Entity(sf::Vector3f pos = sf::Vector3f(0.0, 0.0, 0.0), sf::Vector3f vel = sf::Vector3f(0.0, 0.0, 0.0)) :
		pos(pos), vel(vel), rot(0.0, 0.0), direction(0.0, 0.0, 1.0), left(-1.0, 0.0, 0.0) {}

	virtual void update(float elapsed_time) {
		pos.x += vel.x * elapsed_time;
		pos.y += vel.y * elapsed_time;
		pos.z += vel.z * elapsed_time;
	}

	virtual void draw() {}

	void update_directions() {
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
};
