#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Mesh
{
public:
    Mesh() {}
    ~Mesh() {}
    void initialize(GLuint VAO, int nVertices, Shader* shader, glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), float angle = 0.0, glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0), GLuint texID = 0, float rotationSpeed = 0.0f);
    void update(float deltaTime);
    void draw();
    void setPosition(const glm::vec3& newPosition);
    glm::vec3 getPosition() const;

protected:
    GLuint VAO; // Identificador do Vertex Array Object - Vértices e seus atributos
    int nVertices;
    glm::vec3 position;
    glm::vec3 scale;
    float angle;
    glm::vec3 axis;
    Shader* shader;
    GLuint textureID;
    float rotationSpeed; // Speed of rotation
};