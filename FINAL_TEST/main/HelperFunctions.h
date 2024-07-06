#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

class HelperFunctions {
public:
    static int loadSimpleOBJ(const std::string& filepath, int& nVerts, glm::vec3 color = glm::vec3(1.0, 0.0, 1.0));
    static int loadTexture(const std::string& path);
};

#endif // HELPERFUNCTIONS_H
