#pragma once
#include "Entity.hpp"
#include "Util.hpp"
#include <vector>;

struct Boid : public Entity {
	std::vector<Entity*> neighbors;
	float MAX_SPEED = 62.0;
	float ROT_SPEED_MAX = 360.0;
	float N_DISTANCE = 20.0;
	static float COHESION_BIAS;
	static float SEPERATION_BIAS;
	static float SEPERATION_DISTANCE;



	float size;
	float rot_speed;
	float behaviour_time;
	sf::Color c1;
	sf::Color c2;
	sf::Clock internal_clock;

	Boid(sf::Vector3f pos = sf::Vector3f(0.0, 20.0, 0.0)) : Entity(pos),
		size(1.0), rot_speed(5.0), behaviour_time(2.0), c1(255, 0, 0), c2(255, 255, 0), neighbors(10) {
		rot.x = rand() % 360;
		rot_speed = 1360;
		update_directions();
		root_node->insert(this);
		internal_clock.restart();
	}

	virtual void update_position(float elapsed_time) {
		rot.x += rot_speed * elapsed_time;

		update_directions();
		vel = set_length(direction, MAX_SPEED);

		pos.x += vel.x * elapsed_time;
		pos.y += vel.y * elapsed_time;
		pos.z += vel.z * elapsed_time;
	}

	void random_rot() {
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

	float separation_rot() {
		sf::Vector3f c(0.0, 0.0, 0.0);

		for(Entity * e : neighbors) {
			if(get_length_sqared(pos - e->pos) < (SEPERATION_DISTANCE * SEPERATION_DISTANCE)) {
				c = c - (pos - e->pos);
			}
		}

		c = (c - pos);
		// find angle
		return angle(c, direction) * SEPERATION_BIAS;
	}
	
	float alignment_rot() {
		return 0;
	}
	
	float cohesion_rot() {
		sf::Vector3f center(0.0,0.0,0.0);
		for(Entity * e : neighbors) {
			center += e->pos;
		}
		center.x = center.x / (neighbors.size());
		center.y = center.y / (neighbors.size());
		center.z = center.z / (neighbors.size());

		center = (center - pos);
		// find angle
		return angle(center, direction) * COHESION_BIAS;

	}

	virtual void update(float elapsed_time) {
		neighbors.clear();
		if(node != nullptr)
			node->get_neighbors(this, N_DISTANCE, neighbors);

		if(neighbors.size() > 0) {
			float r1 = separation_rot();
			float r2 = alignment_rot();
			float r3 = cohesion_rot();
			rot_speed = r1 + r2 + r3;

		} else {
			random_rot();
		}

		update_position(elapsed_time);

		if (node != nullptr) {
			node->update_entity(this);
		} else {
			c1 = sf::Color(255, 255, 255);
			c2 = sf::Color(255, 15, 255);
			MAX_SPEED = 0.0;
			ROT_SPEED_MAX = 0.0;
			rot_speed = 0.0;
			behaviour_time = 10000000.0;
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
float Boid::COHESION_BIAS = 10.0;
float Boid::SEPERATION_DISTANCE = 30;
float Boid::SEPERATION_BIAS = 100;
