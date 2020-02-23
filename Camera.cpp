#include "Camera.hpp"

Camera::Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fovDegrees, float nearPlane, float farPlane, int bufferWidth, int bufferHeight)
{
    this->eye = eye;
    this->lookAt = lookAt;
    this->up = up;
    fov = glm::radians(fovDegrees);
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->bufferWidth = (float)bufferWidth;
    this->bufferHeight = (float)bufferHeight;
}
void Camera::updateBufferSize(int width, int height)
{
    bufferWidth = (float)width;
    bufferHeight = (float)height;
}
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(eye, this->lookAt, up);
}
glm::mat4 Camera::getProjectionMatrix()
{
    float aspectRatio = bufferWidth / bufferHeight;
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
void Camera::rotateRight(float degrees)
{
    degrees=glm::radians(degrees);
    glm::vec4 lookAt_c = glm::vec4(lookAt, 1);
    lookAt_c = glm::translate(-eye)*lookAt_c;
    lookAt_c = glm::rotate(degrees, up)*lookAt_c;
    lookAt_c = glm::translate(eye)*lookAt_c;
    lookAt = glm::vec3(lookAt_c);
}
void Camera::rotateUp(float degrees)
{
    degrees = glm::radians(degrees);
    glm::vec3 forward = lookAt - eye;
    glm::vec3 local_x = glm::normalize(glm::cross(forward, up));
    glm::vec4 lookAt_c = glm::vec4(lookAt, 1);
    lookAt_c = glm::translate(-eye)*lookAt_c;
    lookAt_c = glm::rotate(degrees, local_x)*lookAt_c;
    lookAt_c = glm::translate(eye)*lookAt_c;
    lookAt = glm::vec3(lookAt_c);
}
void Camera::forward(float distance)
{
    glm::vec3 forward= glm::normalize(lookAt - eye);
    glm::vec3 ForwardVector = distance * forward;
    glm::vec4 eye_c = glm::vec4(eye, 1);
    eye_c = glm::translate(ForwardVector)*eye_c;
    eye = glm::vec3(eye_c);
    glm::vec4 lookAt_c = glm::vec4(lookAt, 1);
    lookAt_c = glm::translate(ForwardVector)*lookAt_c;
    lookAt = glm::vec3(lookAt_c);
}
void Camera::strafeRight(float distance)
{
    glm::vec3 forward = lookAt - eye;
    glm::vec3 local_x = glm::normalize(glm::cross(forward, up));
    glm::vec3 strafeRightVector = distance * local_x;
    glm::vec4 eye_c = glm::vec4(eye, 1);
    eye_c = glm::translate(strafeRightVector)*eye_c;
    eye = glm::vec3(eye_c);
    glm::vec4 lookAt_c = glm::vec4(lookAt, 1);
    lookAt_c = glm::translate(strafeRightVector)*lookAt_c;
    lookAt = glm::vec3(lookAt_c);
}
Camera::~Camera()
{
}

