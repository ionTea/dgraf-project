#pragma once
#include "Entity.hpp"
#include "Util.hpp"

struct Cube : public Entity {
	float size;
	sf::Color color;
	float counter;

	Cube(sf::Vector3f pos = sf::Vector3f(0.0, 0.0, 0.0),
		float size = 5.0,
		sf::Color color = sf::Color(30, 130, 30)) : Entity(pos),
		size(size), color(color) {
		counter = 3.1415*(rand() % 100000) / 10000.0;
	}

	virtual void update(float elapsed_time) {
		counter += elapsed_time;
	}

	virtual void draw() {
		//Put in draw list? #swag
		glPushMatrix();
		glColor3ub(color.r, color.g, color.b);
		float s = size / 2;
		glTranslatef(pos.x, pos.y - sin(3 * counter), pos.z);
		glRotatef(-rot.y, 1.0, 0.0, 0.0);
		glRotatef(-rot.x, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		{
			//Front face
			glVertex3f(-s, -s, s);
			glVertex3f(s, -s, s);
			glVertex3f(s, s, s);
			glVertex3f(-s, s, s);
			//Back face
			glVertex3f(-s, -s, -s);
			glVertex3f(s, -s, -s);
			glVertex3f(s, s, -s);
			glVertex3f(-s, s, -s);
			//Top face
			glVertex3f(-s, s, -s);
			glVertex3f(s, s, -s);
			glVertex3f(s, s, s);
			glVertex3f(-s, s, s);
			//Bottom face
			glVertex3f(-s, -s, -s);
			glVertex3f(s, -s, -s);
			glVertex3f(s, -s, s);
			glVertex3f(-s, -s, s);
			//Left face
			glVertex3f(-s, -s, -s);
			glVertex3f(-s, s, -s);
			glVertex3f(-s, s, s);
			glVertex3f(-s, -s, s);
			//Right face
			glVertex3f(s, -s, -s);
			glVertex3f(s, s, -s);
			glVertex3f(s, s, s);
			glVertex3f(s, -s, s);
		}
		glEnd();
		glPopMatrix();
	}
};
