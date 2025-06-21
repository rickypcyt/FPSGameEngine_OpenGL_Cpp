#include "../../include/graphics/mesh.h"
#include <iostream>
#include <cmath>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) 
    : vertices(vertices), indices(indices) {
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Load data into vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Load data into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    
    // Color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::drawInstanced(unsigned int instanceCount) const {
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
    glBindVertexArray(0);
}

// Static factory methods for common meshes
Mesh Mesh::createCube(float size) {
    std::vector<Vertex> vertices = {
        // Front face
        {{-size, -size,  size}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ size, -size,  size}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ size,  size,  size}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-size,  size,  size}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
        
        // Back face
        {{-size, -size, -size}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}},
        {{-size,  size, -size}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        {{ size,  size, -size}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{ size, -size, -size}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}},
        
        // Left face
        {{-size, -size, -size}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {0.8f, 0.2f, 0.2f}},
        {{-size, -size,  size}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {0.2f, 0.8f, 0.2f}},
        {{-size,  size,  size}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.2f, 0.2f, 0.8f}},
        {{-size,  size, -size}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {0.8f, 0.8f, 0.2f}},
        
        // Right face
        {{ size, -size, -size}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {0.8f, 0.2f, 0.8f}},
        {{ size,  size, -size}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.2f, 0.8f, 0.8f}},
        {{ size,  size,  size}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {0.6f, 0.6f, 0.6f}},
        {{ size, -size,  size}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {0.4f, 0.4f, 0.4f}},
        
        // Top face
        {{-size,  size, -size}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}, {0.9f, 0.9f, 0.9f}},
        {{-size,  size,  size}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}, {0.1f, 0.1f, 0.1f}},
        {{ size,  size,  size}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}, {0.3f, 0.3f, 0.3f}},
        {{ size,  size, -size}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}, {0.7f, 0.7f, 0.7f}},
        
        // Bottom face
        {{-size, -size, -size}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}, {0.2f, 0.2f, 0.2f}},
        {{ size, -size, -size}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, {0.8f, 0.8f, 0.8f}},
        {{ size, -size,  size}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}},
        {{-size, -size,  size}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, {0.1f, 0.1f, 0.1f}}
    };

    std::vector<unsigned int> indices = {
        0,  1,  2,    2,  3,  0,   // Front
        4,  5,  6,    6,  7,  4,   // Back
        8,  9,  10,   10, 11, 8,   // Left
        12, 13, 14,   14, 15, 12,  // Right
        16, 17, 18,   18, 19, 16,  // Top
        20, 21, 22,   22, 23, 20   // Bottom
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::createPlane(float width, float height) {
    std::vector<Vertex> vertices = {
        {{-width/2, 0.0f, -height/2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.8f, 0.8f, 0.8f}},
        {{ width/2, 0.0f, -height/2}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.6f, 0.6f, 0.6f}},
        {{ width/2, 0.0f,  height/2}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.8f, 0.8f}},
        {{-width/2, 0.0f,  height/2}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.6f, 0.6f, 0.6f}}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::createSphere(float radius, int segments) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= segments; ++i) {
        float lat = M_PI * (-0.5f + (float)i / segments);
        float y = radius * sin(lat);
        float r = radius * cos(lat);

        for (int j = 0; j <= segments; ++j) {
            float lon = 2 * M_PI * (float)j / segments;
            float x = r * cos(lon);
            float z = r * sin(lon);

            glm::vec3 position(x, y, z);
            glm::vec3 normal = glm::normalize(position);
            glm::vec2 texCoords((float)j / segments, (float)i / segments);
            glm::vec3 color(0.7f, 0.7f, 0.7f);

            vertices.push_back({position, normal, texCoords, color});
        }
    }

    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = i * (segments + 1) + j;
            int second = first + segments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return Mesh(vertices, indices);
} 