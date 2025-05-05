#pragma once
#include <string>
#include <vector>

struct Vertex {
    float x, y, z;
};

class Character {
public:
    Character() = default;
    bool load(const std::string& path);
    void draw() const;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

