#include "World/entity.h"

   glm::vec3 position, size, color;
    float rotation;
    bool active;

Entity::Entity()
    :   position(glm::vec3(0.0f)), size(glm::vec3(1.0f)), color(glm::vec3(0.0f)),
        rotation(0.0f), active(true){ }

Entity::Entity(glm::vec3 pos, glm::vec3 size, glm::vec3 color)
    :   position(pos), size(size), color(color),
        rotation(0.0f), active(true){ }

