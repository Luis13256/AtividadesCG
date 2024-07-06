#ifndef MOUSEKEYCALLBACKMANAGER_H
#define MOUSEKEYCALLBACKMANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class MouseKeyCallbackManager {
public:
    MouseKeyCallbackManager();

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    bool isObjectsMoving() const { return objectsMoving; }

    glm::vec3 getCameraPos() const { return cameraPos; }
    glm::vec3 getCameraFront() const { return cameraFront; }
    glm::vec3 getCameraUp() const { return cameraUp; }

private:
    bool objectsMoving;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    bool firstMouse;
    float lastX, lastY;
    float pitch, yaw;
    const float CAMERA_SPEED = 0.3f;
    const float SENSITIVITY = 0.05f;
};

#endif // MOUSEKEYCALLBACKMANAGER_H
