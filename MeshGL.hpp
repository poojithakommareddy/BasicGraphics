#ifndef MESH_GL
#define MESH_GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "MeshData.hpp"
class MeshGL
{
public:
    void draw();
    MeshGL(MeshData *data);
    ~MeshGL();
private:
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;
    int indexCnt = 0;
};
#endif
