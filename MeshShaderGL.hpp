#ifndef MESH_SHADER_GL
#define MESH_SHADER_GL
#include "ShaderGL.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include<string>
#include "Camera.hpp"
#include "Light.hpp"

class MeshShaderGL:public ShaderGL
{
public:
    MeshShaderGL(std::string vertex, std::string fragment, bool isFilepath);
    void setModelTransform(glm::mat4 &modelMat);
    void setViewAndProjection(Camera *camera);
    void setLight(Light *light);
    void setShininess(float s);
    void setMaterialChoice(int choice);
    
    ~MeshShaderGL();
private:
    GLint modelMatLoc =-1;
    glm::mat4 modelMat = glm::mat4(1.0);
    GLint viewMatLoc = -1;
    GLint projMatLoc = -1;
    glm::mat4 viewMat = glm::mat4(1.0);
    GLint NormMatLoc = -1;
    GLint LightPosLoc = -1;
    GLint LightColorLoc = -1;
    GLint ShininessLoc = -1;
    GLint MaterialChoiceLoc = -1;
    
};
#endif

