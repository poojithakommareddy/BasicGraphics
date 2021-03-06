#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>
#include<GL/glew.h>
class Camera
{
public:
    Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fovDegrees, float nearPlane, float farPlane, int bufferWidth, int bufferHeight);
    void updateBufferSize(int width, int height);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void rotateRight(float degrees);
    void rotateUp(float degrees);
    void forward(float distance);
    void strafeRight(float distance);
    ~Camera();
    
private:
    glm::vec3 eye = glm::vec3(0, 0, 1);
    glm::vec3 lookAt = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    float fov = glm::radians(100.0f);
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float bufferWidth = 500.0f;
    float bufferHeight = 500.0f;
};
#endif
