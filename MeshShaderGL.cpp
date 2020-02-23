#include "MeshShaderGL.hpp"
#include  "Camera.hpp"
void MeshShaderGL::setModelTransform(glm::mat4 &modelMat)
{
    this->modelMat=modelMat;
    glm::mat3 normMat = glm::mat3(glm::transpose(glm::inverse(viewMat*modelMat)));
    glUniformMatrix3fv(NormMatLoc,1,GL_FALSE, value_ptr(normMat));
    glUniformMatrix4fv(modelMatLoc,1,GL_FALSE, value_ptr(modelMat));
}
void MeshShaderGL::setViewAndProjection(Camera *camera)
{
    this->viewMat = camera->getViewMatrix();
    glm::mat4 projMat = camera->getProjectionMatrix();
    glm::mat3 normMat = glm::mat3(glm::transpose(glm::inverse(viewMat*modelMat)));
    glUniformMatrix3fv(NormMatLoc,1,GL_FALSE, value_ptr(normMat));
    glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(viewMat));
    glUniformMatrix4fv(projMatLoc,1,GL_FALSE, value_ptr(projMat));
}
void MeshShaderGL::setLight(Light *light)
{
    glm::vec4 viewLight = viewMat*(light->getPos());
    glUniform4fv(LightPosLoc,1, value_ptr(viewLight));
    glUniform4fv(LightColorLoc,1, value_ptr(light->getColor()));
}
void MeshShaderGL:: setShininess(float s)
{
    glUniform1f(ShininessLoc,s);
}
void MeshShaderGL::setMaterialChoice(int choice)
{
    glUniform1i(MaterialChoiceLoc,choice);
}
MeshShaderGL::MeshShaderGL(std::string vertex, std::string fragment, bool isFilepath): ShaderGL(vertex,fragment,isFilepath)
{
    modelMatLoc = glGetUniformLocation(programID,"modelMat");
    viewMatLoc  = glGetUniformLocation(programID,"viewMat");
    projMatLoc  = glGetUniformLocation(programID,"projMat");
    NormMatLoc = glGetUniformLocation(programID, "normMat");
    LightPosLoc = glGetUniformLocation(programID, "light.pos");
    LightColorLoc =glGetUniformLocation(programID, "light.color");
    ShininessLoc= glGetUniformLocation(programID, "shiny");
    MaterialChoiceLoc = glGetUniformLocation(programID,"matChoice");
}
MeshShaderGL::~MeshShaderGL()
{
}
