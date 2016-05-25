#include <Windows.h>
#include <gl\GL.h>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>


float getLength(sf::Vector3f const & v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

sf::Vector3f setLength(sf::Vector3f v, float length) {
	float l = getLength(v);
	return sf::Vector3f(length * v.x / l, length * v.y / l, length * v.z / l);
}

void p(sf::Vector3f v) {
	std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

void p(float f) {
	std::cout << f << std::endl;
}

class Entity {
public:
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

class Boid : public Entity {
public:
	float MAX_SPEED = 12.0;
	float ROT_SPEED_MAX = 360.0;

	float size;
	float rot_speed;
	float behaviour_time;
	sf::Color c1;
	sf::Color c2;
	sf::Clock internal_clock;
	
	Boid(sf::Vector3f pos = sf::Vector3f(0.0, 20.0, 0.0)) : Entity(pos), 
		size(1.0), rot_speed(5.0), behaviour_time(2.0), c1(255, 0, 0), c2(255, 255, 0){
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

		if (internal_clock.getElapsedTime().asSeconds() > behaviour_time) {
			internal_clock.restart();
			behaviour_time = (rand() % 500) / 1000.0;
			if(behaviour_time > 0.2)
				behaviour_time += (rand() % 500) / 1000.0;
			if(behaviour_time > 0.4)
				behaviour_time += (rand() % 500) / 1000.0;
			if(behaviour_time > 0.6)
				behaviour_time += (rand() % 500) / 1000.0;

			rot_speed = ROT_SPEED_MAX - ROT_SPEED_MAX  * 2 * ((rand() % (1000)) / (1000.0));
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
		glScalef(1/size, 1/size, 1/size);
		glPopMatrix();
	}
};

class Cube : public Entity {
public:
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
		glTranslatef(pos.x, pos.y - sin(3*counter), pos.z);
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

class Camera : public Entity {
public:
	sf::Vector2f resolution;

	Camera(sf::Vector2f resolution, sf::Vector3f pos = sf::Vector3f(0.0, 0.0, 0.0)) : Entity(pos),
		resolution(resolution) {}

	virtual void update(float elapsed_time) {
		Entity::update(elapsed_time);
		//pos.x += vel.x * elapsed_time;
		//pos.y += vel.y * elapsed_time;
		//pos.z += vel.z * elapsed_time;
		vel.x = 0.0;
		vel.y = 0.0;
		vel.z = 0.0;
	}

	virtual void draw() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -resolution.y / resolution.x, resolution.y / resolution.x, 1.0, 1000.0);
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
		if (rot.y > 89) {
			rot.y = 89;
		}

		if (rot.y < -89) {
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

int main() {
	//Create window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(800, 500, 32), "Snoids?", sf::Style::Default, settings);
	window.setFramerateLimit(150);
	sf::Vector2f center((static_cast<float>(window.getSize().x) / 2.0f), (static_cast<float>(window.getSize().y) / 2.0f));
	sf::Vector2f size((static_cast<float>(window.getSize().x)), (static_cast<float>(window.getSize().y)));

	//Init OpenGl
	glClearDepth(20.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	//Mouse stuff
	window.setMouseCursorVisible(false);
	sf::Vector2i mouse_center(center.x + window.getPosition().x, center.y + window.getPosition().y);

	sf::Mouse::setPosition(mouse_center);
	float mouse_sensitivity = 0.1;

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/consola.ttf");
	sf::Text text("Is this for snoids?", font);

	//Create our camera
	Camera camera(size, sf::Vector3f(0.0, -40.0, -30.0));
	camera.rot.y = 45;
	
	//Create some cubes (snoid placeholder)
	std::vector<Cube> cubes;
	int range;
		
	range = 30;
	int count = 30;
	float csize = 20.0;
	for(int i = 0; i < count; i++) {
		for(int j = 0; j < count; j++) {
			cubes.push_back(Cube(sf::Vector3f((i - count/2) * csize,
				-csize/2,
				(j - count/2) * csize)));
			cubes.back().size = csize;
			cubes.back().color = sf::Color(10, 15 + (rand() % 3), 25 + (rand() % 3));
		}
	}

	std::vector<Boid> boids;
	for (int i = 0; i < 1000; i++) {
		boids.push_back(Boid(sf::Vector3f(0.0, 20.0, 0.0)));
	}
	boids.push_back(Boid(sf::Vector3f(0.0, 20.0, 0.0)));
	boids.back().size = 5.0;
	boids.back().c1 = sf::Color(0, 100, 150);
	boids.back().c2 = sf::Color(0, 200, 0);
	boids.back().ROT_SPEED_MAX = 180;

	sf::Clock frame_clock;
	float frame_time = 0.0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Resized) {
				center = sf::Vector2f((static_cast<float>(window.getSize().x) / 2.0f), (static_cast<float>(window.getSize().y) / 2.0f));
				size = sf::Vector2f((static_cast<float>(window.getSize().x)), (static_cast<float>(window.getSize().y)));
				window.setView(sf::View(center, size));
				camera.update_viewport(size);
				mouse_center = sf::Vector2i(center.x + window.getPosition().x, center.y + window.getPosition().y);
				sf::Mouse::setPosition(mouse_center);
			} else if (event.type == sf::Event::LostFocus) {
				window.setMouseCursorVisible(true);
			} else if (event.type == sf::Event::GainedFocus) {
				window.setMouseCursorVisible(false);
				mouse_center = sf::Vector2i(center.x + window.getPosition().x, center.y + window.getPosition().y);
				sf::Mouse::setPosition(mouse_center);
			} else if (event.type == sf::Event::MouseMoved) {
				if (window.hasFocus() && sf::Mouse::getPosition() != mouse_center) {
					camera.rotate((sf::Mouse::getPosition().x - mouse_center.x) * mouse_sensitivity,
						(sf::Mouse::getPosition().y - mouse_center.y) * mouse_sensitivity);
					sf::Mouse::setPosition(mouse_center);
				}
			} else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			} else if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) camera.moveX(-150);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) camera.moveX(150);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) camera.moveZ(150);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) camera.moveZ(-150);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) camera.moveY(150);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) camera.moveY(150);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) camera.moveY(-150);

		frame_time = frame_clock.getElapsedTime().asSeconds();
		frame_clock.restart();

		//Update stuff
		camera.update(frame_time);
		for (auto & c : cubes) c.update(frame_time);
		for (auto & b : boids) b.update(frame_time);

		//Draw stuff
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.draw();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		for (auto & c : cubes) c.draw();
		for (auto & b : boids) b.draw();

		window.pushGLStates();
		//Draw other stuff
		window.draw(text);
		window.popGLStates();

		window.display();
	}
	return 0;
}
