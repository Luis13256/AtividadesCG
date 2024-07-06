#include "MouseKeyCallbackManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

MouseKeyCallbackManager::MouseKeyCallbackManager()
    : objectsMoving(true), cameraPos(glm::vec3(0.0, 0.0, 3.0)), cameraFront(glm::vec3(0.0, 0.0, -1.0)), cameraUp(glm::vec3(0.0, 1.0, 0.0)),
    firstMouse(true), lastX(400), lastY(300), pitch(0.0f), yaw(-90.0f) {}

void MouseKeyCallbackManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        objectsMoving = false;

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        objectsMoving = true;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        objectsMoving = !objectsMoving;

    if (key == GLFW_KEY_W)
        cameraPos += cameraFront * CAMERA_SPEED;

    if (key == GLFW_KEY_S)
        cameraPos -= cameraFront * CAMERA_SPEED;

    if (key == GLFW_KEY_A)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * CAMERA_SPEED;

    if (key == GLFW_KEY_D)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * CAMERA_SPEED;
}

void MouseKeyCallbackManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float offsetx = xpos - lastX;
    float offsety = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    offsetx *= SENSITIVITY;
    offsety *= SENSITIVITY;

    pitch += offsety;
    yaw += offsetx;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
