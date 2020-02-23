#include "Light.hpp"
Light::Light (glm::vec3 pos, glm::vec3 color, bool isPointLight)
{
    this->pos = pos;
    this->color = color;
    this->isPointLight = isPointLight;
}
void Light::setPos(glm::vec3 pos)
{
    this->pos = pos;
}
void Light::setIsPointLight(bool isPointLight)
{
    this->isPointLight = isPointLight;
}
void Light::setColor(glm::vec3 color)
{
    this->color = color;
}
bool Light::getIsPointLight()
{
    return isPointLight;
}
glm::vec4 Light::getColor()
{
    return glm::vec4(color, 1.0);
}
glm::vec4 Light::getPos()
{
    if(isPointLight)
        return glm::vec4(pos, 1.0);
    else
        return glm::vec4(pos, 0.0);
}
