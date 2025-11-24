#pragma once 

#include "Types.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace etugl {

class Camera {

    vec3f m_Position = vec3f(0.0f, 0.0f, 0.0f);
    vec3f m_Front = vec3f(0.0f, 0.0f, -0.0f);
    vec3f m_Up = vec3f(0.0f, 1.0f, 0.0f);

    float m_Yaw = -90.0f; 
    float m_Pitch = 0.0f;  
    
    float m_MovementSpeed = 2.5f;   
    float m_MouseSensitivity = 0.1f;

protected:

    int m_Width = 1.0f;
    int m_Height = 1.0f;

public: 

    Camera() = default;

    Camera(float mov_speed, float mouse_sens) :
        m_MovementSpeed(mov_speed), m_MouseSensitivity(mouse_sens) {}

    virtual ~Camera() = default;

    [[nodiscard]] virtual inline mat4f projection() const = 0; 

    [[nodiscard]] inline mat4f view() const {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    } 

    [[nodiscard]] inline vec3f position() const { return m_Position; }
 
    void update_position(float delta_time, int key);
 
    void update_front(float new_x, float new_y);  

    [[nodiscard]] float& movemet_speed() { return m_MovementSpeed; }

    [[nodiscard]] float& mouse_sensitivity() { return m_MouseSensitivity; }

    [[nodiscard]] int& width() { return m_Width; }

    [[nodiscard]] int& height() { return m_Height; }
};

class Ortographic : public Camera {

    float m_Left = -1.0f;
    float m_Right = 1.0f;
    float m_Bottom = -1.0f;  
    float m_Top = 1.0f;

public:
    Ortographic() = default;

    Ortographic(float mov_speed, float mouse_sens) : 
        Camera(mov_speed, mouse_sens) {}

    Ortographic(float left, float right, float bottom, float top, 
                float mov_speed, float mouse_sens) : 
        Camera(mov_speed, mouse_sens), 
        m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top) {}

    [[nodiscard]] inline mat4f projection() const override {
        return glm::ortho(m_Left, m_Right, m_Bottom, m_Top, -1.0f, 1.0f);
    } 

    float& left() { return m_Left; }

    float& right() { return m_Right; }

    float& bottom() { return m_Bottom; }
 
    float& top() { return m_Top; }
};


class Perspective : public Camera {
    float m_FOV = 45.0f;
    float m_NearClip = 0.1f;
    float m_FarClip = 100.0f;

public:
    Perspective() = default;

    Perspective(float mov_speed, float mouse_sens) : 
        Camera(mov_speed, mouse_sens) {};

    [[nodiscard]] inline mat4f projection() const override {
        return glm::perspective(glm::radians(m_FOV), 
                                (float)(m_Width) / (float)(m_Height), 
                                m_NearClip, m_FarClip);
    }

    void update_fov(float offset_y);
};
 
}
