#include "HelperFunctions.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stb_image.h>

using namespace std;

int HelperFunctions::loadSimpleOBJ(const string& filepath, int& nVerts, glm::vec3 color) {
    vector<glm::vec3> vertices;
    vector<GLuint> indices;
    vector<glm::vec2> texCoords;
    vector<glm::vec3> normals;
    vector<GLfloat> vbuffer;

    ifstream inputFile(filepath);
    if (!inputFile.is_open()) {
        cout << "Problema ao encontrar o arquivo " << filepath << endl;
        return 0;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream ssline(line);
        string word;
        ssline >> word;

        if (word == "v") {
            glm::vec3 v;
            ssline >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (word == "vt") {
            glm::vec2 vt;
            ssline >> vt.s >> vt.t;
            texCoords.push_back(vt);
        }
        else if (word == "vn") {
            glm::vec3 vn;
            ssline >> vn.x >> vn.y >> vn.z;
            normals.push_back(vn);
        }
        else if (word == "f") {
            string tokens[3];
            ssline >> tokens[0] >> tokens[1] >> tokens[2];

            for (int i = 0; i < 3; ++i) {
                int pos = tokens[i].find("/");
                int index = atoi(tokens[i].substr(0, pos).c_str()) - 1;
                indices.push_back(index);

                vbuffer.push_back(vertices[index].x);
                vbuffer.push_back(vertices[index].y);
                vbuffer.push_back(vertices[index].z);
                vbuffer.push_back(color.r);
                vbuffer.push_back(color.g);
                vbuffer.push_back(color.b);

                tokens[i] = tokens[i].substr(pos + 1);
                pos = tokens[i].find("/");
                index = atoi(tokens[i].substr(0, pos).c_str()) - 1;
                vbuffer.push_back(texCoords[index].s);
                vbuffer.push_back(texCoords[index].t);

                tokens[i] = tokens[i].substr(pos + 1);
                index = atoi(tokens[i].c_str()) - 1;
                vbuffer.push_back(normals[index].x);
                vbuffer.push_back(normals[index].y);
                vbuffer.push_back(normals[index].z);
            }
        }
    }

    inputFile.close();

    GLuint VBO, VAO;
    nVerts = vbuffer.size() / 11;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(GLfloat), vbuffer.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

int HelperFunctions::loadTexture(const string& path) {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Failed to load texture" << endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}
