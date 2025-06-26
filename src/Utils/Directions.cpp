#include "Directions.h"

#include <glm/common.hpp>
#include <glm/vec2.hpp>

glm::ivec3 Directions::ToDominantHorizontalDirection(const glm::vec3& v)
{
    glm::vec2 xz = glm::vec2(v.x, v.z);
    if (std::abs(xz.x) > std::abs(xz.y))
    {
        return {v.x > 0 ? 1 : -1, 0, 0};
    }

    return {0, 0, v.z > 0 ? 1 : -1};
}