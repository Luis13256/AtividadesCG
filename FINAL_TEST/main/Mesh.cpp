#include "Mesh.h"

void Mesh::initialize(GLuint VAO, int nVertices, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis, GLuint texID, float rotationSpeed)
{
    this->VAO = VAO;
    this->nVertices = nVertices;
    this->shader = shader;
    this->position = position;
    this->scale = scale;
    this->angle = angle;
    this->axis = axis;
    this->textureID = texID;
    this->rotationSpeed = rotationSpeed;
}

void Mesh::update(float deltaTime)
{
    angle += rotationSpeed * deltaTime; // Increment angle based on rotation speed and delta time
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::scale(model, scale);
    shader->setMat4("model", glm::value_ptr(model));
}

void Mesh::draw()
{
    shader->Use();
    if (textureID) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader->setInt("texture1", 0);
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
    glBindVertexArray(0);
}

void Mesh::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

glm::vec3 Mesh::getPosition() const {
    return position;
}
