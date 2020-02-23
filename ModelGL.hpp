#ifndef MODEL_GL
#define MODEL_GL
#include<vector>
#include"MeshShaderGL.hpp"
#include"ModelData.hpp"
#include"MeshData.hpp"
#include"MeshGL.hpp"
#include"glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/string_cast.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtx/transform.hpp"
#include"glm/gtc/type_ptr.hpp"
class ModelGL
{
public:
    void translate(glm::vec3 offset);
    void rotate(float degrees,glm::vec3 axis);
    void reset();
    void draw(MeshShaderGL *shader);
    ModelGL(ModelData *data);
    ~ModelGL();
private:
    vector<MeshGL*> meshes;
    glm::mat4 modelMat=glm::mat4 (1.0);
    glm::vec3 potcenter = glm::vec3(0.f,0.f,0.f);
};
#endif
