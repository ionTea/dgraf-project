#pragma once
#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

void draw_circle(sf::Vector3f pos, float radius) {
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(pos.x, pos.y, pos.z);
	for(int i = 0; i < 360; i++) {
		glRotatef(i, 0, 1, 0);
		glTranslatef(radius, 0, 0);
		glBegin(GL_POINTS);
		{
			glVertex3f(0, 0, 0);
		}
		glEnd();
		glTranslatef(-radius, 0, 0);
		glRotatef(-i, 0, 1, 0);
	}
	glPopMatrix();
}
