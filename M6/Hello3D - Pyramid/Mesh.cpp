#include "Mesh.h"

void Mesh::initialize(GLuint VAO, int nVertices, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis, GLuint texID)
{
	this->VAO = VAO;
	this->nVertices = nVertices;
	this->shader = shader;
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->axis = axis;
	this->textureID = texID;
}

void Mesh::update()
{
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
