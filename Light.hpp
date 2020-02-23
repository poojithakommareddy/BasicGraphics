#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;

class Light
{
private:
    glm::vec3 pos = glm::vec3(0,0,0);
    glm::vec3 color = glm::vec3(1,1,1);
    bool isPointLight = true;

public:
    Light();
    Light( glm::vec3 pos, glm::vec3 color, bool isPointLight);
    void setPos(glm::vec3 pos);
    void setIsPointLight(bool isPointLight);
    void setColor(glm::vec3 color);
    bool getIsPointLight();
    glm::vec4 getColor();
    glm::vec4 getPos();
};
#endif
