#include "MeshGL.hpp"

void MeshGL::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCnt, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
MeshGL::MeshGL(MeshData *data)
{
vector<Vertex>* vertices = data->getVertices();
vector<unsigned int>* elements = data->getIndices();
    indexCnt=elements->size();
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 elements->size() * sizeof(GLuint),
                 elements->data(),
                 GL_STATIC_DRAW);
    glBindVertexArray(0);
}
MeshGL::~MeshGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &EBO);
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
}
