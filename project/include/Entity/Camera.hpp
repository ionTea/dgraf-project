#pragma once
#include "Entity.hpp"

struct Camera : public Entity {
	sf::Vector2f resolution;

	Camera(sf::Vector2f resolution, sf::Vector3f pos = sf::Vector3f(0.0, 0.0, 0.0)) : Entity(pos),
		resolution(resolution) {}

	virtual void update(float elapsed_time) {
		Entity::update(elapsed_time);
		vel.x = 0.0;
		vel.y = 0.0;
		vel.z = 0.0;
	}

	virtual void draw() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -resolution.y / resolution.x, resolution.y / resolution.x, 1.0, 5000.0);
		glRotatef(rot.y, 1.0, 0.0, 0.0);
		glRotatef(rot.x, 0.0, 1.0, 0.0);
		glTranslatef(pos.x, pos.y, pos.z);
	}

	void update_viewport(sf::Vector2f new_resolution) {
		glViewport(0, 0, new_resolution.x, new_resolution.y);
		resolution = new_resolution;
	}

	void rotate(float x, float y) {
		rot.x += x;
		rot.y += y;
		if(rot.y > 89) {
			rot.y = 89;
		}

		if(rot.y < -89) {
			rot.y = -89;
		}
		update_directions();
	}

	void moveZ(float scalar) {
		vel.x += direction.x * scalar;
		vel.y += direction.y * scalar;
		vel.z += direction.z * scalar;
	}

	void moveX(float scalar) {
		vel.x += left.x * scalar;
		vel.y += left.y * scalar;
		vel.z += left.z * scalar;
	}

	void moveY(float scalar) {
		vel.y += -1.0 * scalar;
	}
};
