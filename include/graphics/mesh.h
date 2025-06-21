#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 color;
};

class Mesh {
private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    void setupMesh();

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();
    
    void draw() const;
    void drawInstanced(unsigned int instanceCount) const;
    
    // Utility functions to create common meshes
    static Mesh createCube(float size = 1.0f);
    static Mesh createPlane(float width = 1.0f, float height = 1.0f);
    static Mesh createSphere(float radius = 1.0f, int segments = 32);
    
    // Getters
    GLuint getVAO() const { return VAO; }
    GLuint getVBO() const { return VBO; }
    GLuint getEBO() const { return EBO; }
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
};

#endif // MESH_H 