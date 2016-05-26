#pragma once
#include "Entity.hpp"
#include "Util.hpp"

struct Boid : public Entity {
	float MAX_SPEED = 12.0;
	float ROT_SPEED_MAX = 360.0;

	float size;
	float rot_speed;
	float behaviour_time;
	sf::Color c1;
	sf::Color c2;
	sf::Clock internal_clock;

	Boid(sf::Vector3f pos = sf::Vector3f(0.0, 20.0, 0.0)) : Entity(pos),
		size(1.0), rot_speed(5.0), behaviour_time(2.0), c1(255, 0, 0), c2(255, 255, 0) {
		rot.x = rand() % 360;
		rot_speed = 1360;
		update_directions();
		internal_clock.restart();
	}

	virtual void update(float elapsed_time) {
		rot.x += rot_speed * elapsed_time;
		update_directions();
		vel = setLength(direction, MAX_SPEED);

		pos.x += vel.x * elapsed_time;
		pos.y += vel.y * elapsed_time;
		pos.z += vel.z * elapsed_time;

		if(internal_clock.getElapsedTime().asSeconds() > behaviour_time) {
			internal_clock.restart();
			behaviour_time = (rand() % 500) / 1000.0;
			if(behaviour_time > 0.2)
				behaviour_time += (rand() % 500) / 1000.0;
			if(behaviour_time > 0.4)
				behaviour_time += (rand() % 500) / 1000.0;
			if(behaviour_time > 0.6)
				behaviour_time += (rand() % 500) / 1000.0;

			rot_speed = ROT_SPEED_MAX - ROT_SPEED_MAX * 2 * ((rand() % (1000)) / (1000.0));
		}
	}

	virtual void draw() {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(-rot.y, 1.0, 0.0, 0.0);
		glRotatef(-rot.x, 0.0, 1.0, 0.0);
		glScalef(size, size, size);
		glBegin(GL_TRIANGLES);
		{
			glColor3ub(c1.r, c1.g, c1.b);
			glVertex3f(0.2, 0.1, -0.4);
			glVertex3f(-0.2, 0.1, -0.4);
			glColor3ub(c2.r, c2.g, c2.b);
			glVertex3f(0.0, 0.0, 0.6);

			glColor3ub(c1.r, c1.g, c1.b);
			glVertex3f(0.2, -0.1, -0.4);
			glVertex3f(-0.2, -0.1, -0.4);
			glColor3ub(c2.r, c2.g, c2.b);
			glVertex3f(0.0, 0.0, 0.6);

			glColor3ub(c1.r, c1.g, c1.b);
			glVertex3f(0.2, 0.1, -0.4);
			glVertex3f(0.2, -0.1, -0.4);
			glColor3ub(c2.r, c2.g, c2.b);
			glVertex3f(0.0, 0.0, 0.6);

			glColor3ub(c1.r, c1.g, c1.b);
			glVertex3f(-0.2, 0.1, -0.4);
			glVertex3f(-0.2, -0.1, -0.4);
			glColor3ub(c2.r, c2.g, c2.b);
			glVertex3f(0.0, 0.0, 0.6);

			glColor3ub(c1.r, c1.g, c1.b);
			glVertex3f(-0.2, 0.1, -0.4);
			glVertex3f(-0.2, -0.1, -0.4);
			glVertex3f(0.2, -0.1, -0.4);
			glVertex3f(-0.2, 0.1, -0.4);
			glVertex3f(0.2, 0.1, -0.4);
			glVertex3f(0.2, -0.1, -0.4);
		}
		glEnd();
		glScalef(1 / size, 1 / size, 1 / (size));
		glPopMatrix();
	}
};
