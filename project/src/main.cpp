#include <Windows.h>
#include <gl\GL.h>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include "Entity\Camera.hpp"
#include "Entity\Boid.hpp"
#include "Entity\Cube.hpp"
#include "QuadTree.hpp"
#include "Util.hpp"

int main() {
	//Entity::root_node = new QuadTree(5000.0, sf::Vector3f(-5000.0 / 2, 0.0, -5000.0 / 2));
	Entity::root_node = new QuadTree(600.0);

	p("OIJK");
	//new Entity(sf::Vector3f(5.0, 5.0, 0.0));
	//new Entity(sf::Vector3f(215.0, 465.1, 0.0));
	//new Entity(sf::Vector3f(100.0, 145.1, 0.0));

	//char in;
	//std::cin >> in;
	//return 0;



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
	//Mac
	//font.loadFromFile("~/Library/Fonts/TODO.ttf");
	//Ubuntu
	//font.loadFromFile("/usr/share/fonts/TODO.ttf");
	font.loadFromFile("C:/Windows/Fonts/consola.ttf");
	sf::Text text("Is this for snoids?", font);

	//Create our camera
	Camera camera(size, sf::Vector3f(0.0, -40.0, -30.0));
	camera.rot.y = 45;
	
	//Create some cubes (snoid placeholder)
	//std::vector<Cube> cubes;
	int range;
		
	range = 30;
	int count = 30;
	float csize = 20.0;
	for(int i = 0; i < count; i++) {
		for(int j = 0; j < count; j++) {
			//cubes.push_back(Cube(sf::Vector3f((i - count/2) * csize,
			//	-csize/2,
			//	(j - count/2) * csize)));
			//cubes.back().size = csize;
			//cubes.back().color = sf::Color(10, 15 + (rand() % 3), 25 + (rand() % 3));
		}
	}


	std::vector<Boid*> boids;
	for (int i = 5; i < 450; i += 100) {
		for (int j = 5; j < 450; j += 100) {
			std::cout << "inserting boid at: x=" << i << " z=" << j << std::endl;
			std::cout << "count" << QuadTree::node_count << std::endl;
			boids.push_back(new Boid(sf::Vector3f(i, 0.0, j)));
			boids.back()->size = 5.0;
		}
	}
	//boids.push_back(new Boid(sf::Vector3f(50 * (rand() % 100000) / 100000, 20.0, 50 * (rand() % 100000) / 100000)));
	//boids.back()->size = 5.0;
	//boids.back()->c1 = sf::Color(0, 100, 150);
	//boids.back()->c2 = sf::Color(0, 200, 0);
	//boids.back()->ROT_SPEED_MAX = 180;

	p("dflsjkdnfoksd fkjs dkfj sdkfj ");
	p(QuadTree::node_count);

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
		//for (auto & c : cubes) c.update(frame_time);
		for (auto & b : boids) b->update(frame_time);

		//Draw stuff
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.draw();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//for (auto & c : cubes) c.draw();
		for (auto & b : boids) b->draw();
		Entity::root_node->draw();

		window.pushGLStates();
		//Draw other stuff
		window.draw(text);
		window.popGLStates();

		window.display();
	}
	return 0;
}
