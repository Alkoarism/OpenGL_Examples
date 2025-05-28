#ifndef ENTITY_H
#define ENTITY_H

#include "OpenGL\renderer.h"

#include "sprite_renderer.h"

class Entity {
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	bool destroyed;
	std::string spriteName;

	Entity();
	Entity(
		glm::vec2 pos, glm::vec2 size, 
		std::string textName, glm::vec3 color = glm::vec3(1.0f), 	
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(SpriteRenderer&);
};

#endif