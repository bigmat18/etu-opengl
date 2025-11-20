#include "Types.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <Camera.hpp>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cmath>

namespace etugl {

void Camera::update_position(float delta_time, int key) {
    float speed = m_MovementSpeed * delta_time;

    switch (key) {
        case GLFW_KEY_W:
            m_Position += (speed * m_Front);
            break;

        case GLFW_KEY_S:
            m_Position -= (speed * m_Front);
            break;

        case GLFW_KEY_A:
            m_Position += (speed * glm::normalize(glm::cross(m_Front, m_Up)));
            break;

        case GLFW_KEY_D:
            m_Position += (speed * glm::normalize(glm::cross(m_Front, m_Up)));
            break;

        case GLFW_KEY_SPACE:
            m_Position += (speed * m_Up);
            break;

        case GLFW_KEY_LEFT_SHIFT:
            m_Position -= (speed * m_Up);
            break;
    }
}

void Camera::update_front(float delta_time, float new_x, float new_y) {
    static bool init_camera = true;
    static float last_x = 0.0f;
    static float last_y = 0.0f;

    if (init_camera) {
        init_camera = false;
        last_x = new_x;
        last_y = new_y;
        return;
    }

    const float offset_x = new_x - last_x;
    const float offset_y = last_y - new_y;
    last_x = new_x;
    last_y = new_y;

    m_Yaw += (offset_x * m_MouseSensitivity);
    m_Pitch += (offset_y * m_MouseSensitivity);
    
    if (m_Pitch > 89.0f) m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) m_Pitch = -89.0f;

    m_Front = glm::normalize(vec3f(
        std::cosf(glm::radians(m_Yaw)) * std::cosf(glm::radians(m_Pitch)),
        std::sinf(glm::radians(m_Pitch)),
        std::sinf(glm::radians(m_Yaw)) * std::cosf(glm::radians(m_Pitch))
    ));   
}
    
void Perspective::update_fov(float delta_time, float offset_y) {
    m_FOV -= offset_y;
    if (m_FOV < 1.0f) m_FOV = 1.0f;
    if (m_FOV < 45.0f) m_FOV = 45.0f;
} 

}
