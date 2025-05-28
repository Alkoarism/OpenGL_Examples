#include "entity.h"

Entity::Entity() 
	:	position(0.0f, 0.0f), size(1.0f, 1.0f), 
		velocity(0.0f), rotation(0.0f),
		color(1.0f), spriteName(""), 
		destroyed(false) { }

Entity::Entity(glm::vec2 Pos, glm::vec2 Size, std::string SpriteName, glm::vec3 Color, glm::vec2 Velocity)
	:	position(Pos), size(Size),
		velocity(Velocity), rotation(0.0f),
		color(Color), spriteName(SpriteName),
		destroyed(false) { }

void Entity::Draw(SpriteRenderer& Renderer) {
	Renderer.DrawSprite(this->spriteName, this->position, this->size, this->rotation, this->color);
}
