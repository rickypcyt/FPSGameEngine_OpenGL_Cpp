#include "../../models/character.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>


bool Character::load(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_FlipUVs |
        aiProcess_GenNormals);

    if (!scene || !scene->mRootNode) {
        std::cerr << "Assimp Error: " << importer.GetErrorString() << std::endl;
        return false;
    }

    // Solo un modelo sencillo con un solo mesh por ahora
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            aiVector3D pos = mesh->mVertices[j];
            vertices.push_back({ pos.x, pos.y, pos.z });
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            const aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                indices.push_back(face.mIndices[k]);
            }
        }
    }

    return true;
}

void Character::draw() const {
    glColor3f(1.0f, 1.0f, 1.0f); // blanco
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < indices.size(); ++i) {
        const Vertex& v = vertices[indices[i]];
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}

