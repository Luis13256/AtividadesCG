/* Código adaptado de https://learnopengl.com/ */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "MouseKeyCallbackManager.h"
#include "HelperFunctions.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Constantes
const float Pi = 3.1419f;
const GLuint WIDTH = 1000, HEIGHT = 1000;

// Funções de callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Variáveis globais
MouseKeyCallbackManager callbackManager;
float angle1 = 0.0f;
float angle2 = 0.0f;
float lastFrame = 0.0f;

void drawObject(Shader& shader, Mesh& mesh, float q, float deltaTime);
glm::vec3 rotateAroundCenter(float angle, float radius, const glm::vec3& center = glm::vec3(0.0f));

int main() {
    // Inicialização da GLFW
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Projeto Final - ERICO!", nullptr, nullptr);
    if (!window) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Configurações de callback
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD: carrega todos os ponteiros de funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // Informações de versão
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Configuração da viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Compilação e linkagem do programa de shader
    Shader shader("Phong.vs", "Phong.fs");
    glUseProgram(shader.ID);

    // Matrizes de view e projeção
    glm::mat4 view = glm::lookAt(callbackManager.getCameraPos(), callbackManager.getCameraPos() + callbackManager.getCameraFront(), callbackManager.getCameraUp());
    shader.setMat4("view", glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    shader.setMat4("projection", glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);

    // Carregamento de objetos e texturas
    int nVerts;
    GLuint obj_suzanne = HelperFunctions::loadSimpleOBJ("../../3D_models/Suzanne/suzanneTriLowPoly.obj", nVerts, glm::vec3(0.8f, 0.8f, 0.8f));
    GLuint obj_planet1 = HelperFunctions::loadSimpleOBJ("../../3D_models/Planetas/planeta.obj", nVerts, glm::vec3(0.6, 0.8, 1.0));
    GLuint obj_planet2 = HelperFunctions::loadSimpleOBJ("../../3D_models/Planetas/planeta.obj", nVerts, glm::vec3(0.7, 0.7, 0.7)); 


    GLuint texture_wall = HelperFunctions::loadTexture("../../textures/wall.jpg");
    GLuint texture_earth = HelperFunctions::loadTexture("../../textures/Terra.jpg");
    GLuint texture_mercury = HelperFunctions::loadTexture("../../textures/2k_mercury.jpg");

    // Inicialização dos objetos
    Mesh mesh_earth, mesh_suzanne, mesh_mercury;
    mesh_suzanne.initialize(obj_suzanne, nVerts, &shader, glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0), texture_wall);
    mesh_earth.initialize(obj_planet1, nVerts, &shader, glm::vec3(-2.75, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0), texture_earth, 50.0f);
    mesh_mercury.initialize(obj_planet2, nVerts, &shader, glm::vec3(2.75, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0), texture_mercury, 90.0f);

    // Propriedades do material da superfície
    shader.setFloat("ka", 0.2f);  // Ambiente
    shader.setFloat("kd", 0.8f);  // Difuso
    shader.setFloat("ks", 0.5f);  // Especular
    shader.setFloat("q", 10.0f);  // Exponente de especularidade

    // Fonte de luz pontual
    shader.setVec3("lightPos", -2.0f, 10.0f, 2.0f);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);  // Luz branca intensa

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window)) {
        
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Checa eventos de input e chama as funções de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Cor de fundo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Atualiza a posição e orientação da câmera
        view = glm::lookAt(callbackManager.getCameraPos(), callbackManager.getCameraPos() + callbackManager.getCameraFront(), callbackManager.getCameraUp());
        shader.setMat4("view", glm::value_ptr(view));

        // Atualiza o shader com a posição da câmera
        shader.setVec3("cameraPos", callbackManager.getCameraPos().x, callbackManager.getCameraPos().y, callbackManager.getCameraPos().z);

        if (callbackManager.isObjectsMoving()) {

            // Atualiza a posição dos objetos para movimento circular
            angle1 += 0.01f;  // Ajuste a velocidade do movimento circular
            float radius1 = 3.0f;
            glm::vec3 pos1 = rotateAroundCenter(angle1, radius1);
            mesh_earth.setPosition(pos1);

            angle2 -= 0.01f;  // Ajuste a velocidade do movimento circular
            float radius2 = 6.0f;
            glm::vec3 pos2 = rotateAroundCenter(angle2, radius2);
            mesh_mercury.setPosition(pos2);

        }

        // Desenha os objetos
        drawObject(shader, mesh_earth, 10.0f, deltaTime);
        drawObject(shader, mesh_suzanne, 1.0f, deltaTime);
        drawObject(shader, mesh_mercury, 250.0f, deltaTime);

        // Troca os buffers da tela
        glfwSwapBuffers(window);

        
    }

    // Desaloca os buffers do OpenGL
    glDeleteVertexArrays(1, &obj_planet1);
    glDeleteVertexArrays(1, &obj_suzanne);
    glDeleteVertexArrays(1, &obj_planet2);

    // Finaliza a execução da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    callbackManager.keyCallback(window, key, scancode, action, mode);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    callbackManager.mouseCallback(window, xpos, ypos);
}

void drawObject(Shader& shader, Mesh& mesh, float q, float deltaTime) {
    shader.setFloat("q", q);
    mesh.update(deltaTime);
    mesh.draw();
}

glm::vec3 rotateAroundCenter(float angle, float radius, const glm::vec3& center) {
    glm::vec3 position;
    position.x = center.x + radius * cos(angle);
    position.z = center.z + radius * sin(angle);
    position.y = center.y; // Assuming rotation around the Y-axis
    return position;
}