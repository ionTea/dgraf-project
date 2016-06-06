#pragma once
#include "Entity.hpp"
#include "Util.hpp"
#include <vector>;

struct Boid : public Entity {
	std::vector<Entity*> neighbors;
	float MAX_SPEED = 62.0;
	float ROT_SPEED_MAX = 360.0;
	float N_DISTANCE = 20.0;
	bool main;
	static sf::Vector3f TARGET;
	static float COHESION_BIAS;
	static float SEPERATION_BIAS;
	static float ALIGNMENT_BIAS;
	static float SEPERATION_DISTANCE;
	static float TARGET_BIAS;




	float size;
	float rot_speed;
	float behaviour_time;
	sf::Color c1;
	sf::Color c2;
	sf::Clock internal_clock;

	Boid(sf::Vector3f pos = sf::Vector3f(0.0, 20.0, 0.0), bool m = false) : Entity(pos),
		size(1.0), rot_speed(5.0), behaviour_time(2.0), c1(255, 0, 0), 
		c2(255, 255, 0), neighbors(10), main(m) {
		rot.x = rand() % 360;
		rot_speed = 1360;
		update_directions();
		root_node->insert(this);
		internal_clock.restart();
	}

	virtual void update_position(float elapsed_time) {
		

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

	sf::Vector3f separation() {
		sf::Vector3f c(0.0, 0.0, 0.0);

		for(Entity * e : neighbors) {
			if(get_length_sqared(pos - e->pos) < (SEPERATION_DISTANCE * SEPERATION_DISTANCE)) {
				c = c - (pos - e->pos);
			}
		}

		return -c * SEPERATION_BIAS;
	}
	
	sf::Vector3f alignment() {
		sf::Vector3f v(0.0, 0.0, 0.0);
		for(Entity * e : neighbors) {
			v = v + e->vel;
		}
		return v * ALIGNMENT_BIAS;
	}
	
	sf::Vector3f cohesion() {
		sf::Vector3f center(0.0,0.0,0.0);
		for(Entity * e : neighbors) {
			center += e->pos;
		}
		center.x = center.x / (neighbors.size());
		center.y = center.y / (neighbors.size());
		center.z = center.z / (neighbors.size());

		return (center - pos) * COHESION_BIAS;
	}

	sf::Vector3f target_location() {
		return (TARGET - pos) * TARGET_BIAS;
	}
	// Limit the speed of the boids
	// TODO, global limit var
	void limit_vel() {
		if(get_length(vel) > 50) {
			vel = set_length(vel, 50);
		}
	}

	virtual void update(float elapsed_time) {
		if(main) {
			node->update_entity(this);
			return;
		}
		neighbors.clear();
		if(node != nullptr)
			node->get_neighbors(this, N_DISTANCE, neighbors);

		if(neighbors.size() > 0) {
			sf::Vector3f r1 = separation();
			sf::Vector3f r2 = alignment();
			sf::Vector3f r3 = cohesion();
			sf::Vector3f r4 = target_location();
			vel = vel + r1 + r2 + r3 + r4;
			limit_vel();
		} else {
			random_rot();
			rot.x += rot_speed * elapsed_time;

			update_directions();
			vel = set_length(direction, MAX_SPEED);
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
float Boid::COHESION_BIAS = 0.1;
float Boid::SEPERATION_DISTANCE = 20;
float Boid::SEPERATION_BIAS = 1;
float Boid::ALIGNMENT_BIAS = (1 / 8);
float Boid::TARGET_BIAS = 0.01;
sf::Vector3f Boid::TARGET(0.0, 0.0, 0.0);