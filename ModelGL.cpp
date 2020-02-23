#include "ModelGL.hpp"
void ModelGL::translate(glm::vec3 offset)
{
    potcenter = potcenter + offset;
    glm::mat4 T =glm::translate(offset);
    modelMat = T *modelMat;
}
void ModelGL::rotate(float degrees,glm::vec3 axis)
{
    glm::mat4 Trn = glm::translate(glm::vec3(-potcenter));
    glm::mat4 Rt = glm::rotate(glm::radians(degrees),axis);
    glm::mat4 Ivt = glm::inverse(Trn);
    glm::mat4 T = Ivt*Rt*Trn;
    modelMat = T*modelMat;
}
void ModelGL::reset()
{
    potcenter=glm::vec3(0.f,0.f,0.f);
    modelMat=glm::mat4(1.0);
}
void ModelGL::draw(MeshShaderGL *shader)
{
    shader->setModelTransform(modelMat);
    for (int i=0; i<meshes.size(); i++)
    {
        meshes.at(i)->draw();
    }
}
ModelGL::ModelGL(ModelData *data)
{
  for (int i=0; i<data->getMeshCnt(); i++)
    {
        meshes.push_back(new MeshGL(data->getMesh(i)));
    }
}
ModelGL::~ModelGL()
{
    for (int i=0; i<meshes.size(); i++){
    delete meshes.at(i);
}
meshes.clear();
}
