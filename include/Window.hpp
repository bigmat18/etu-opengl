#pragma once
#include "Camera.hpp"
#include <cwctype>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include <utils.hpp>
#include <Types.hpp>
#include <type_traits>

namespace etugl {

namespace detail {

template <typename T>
requires (std::is_same_v<T, void> || std::is_base_of_v<Camera, T>)
class Win { 
protected:
    T m_Camera;

public:
    T& camera() { return m_Camera; }
};

template <>
class Win<void> {};

}

struct WindowProps {
    u32 m_Width = 800;
    u32 m_Height = 600;
    std::string m_Title = "ETGL Window";
    u32 m_BGColor = 0x000000FF;

    float bg_r() const { return ((m_BGColor >> 24) & 0xFF) / 255.0f; }
    float bg_g() const { return ((m_BGColor >> 16) & 0xFF) / 255.0f; }
    float bg_b() const { return ((m_BGColor >> 8) & 0xFF) / 255.0f; }
    float bg_a() const { return ((m_BGColor >> 0) & 0xFF) / 255.0f; }
};

template <typename T>
class Window : public detail::Win<T> {
    static constexpr bool HasT = !std::is_same_v<T, void>;

    WindowProps m_Props;
    GLFWwindow* m_Window;
    float m_DeltaTime = 0;

public:
    Window(const WindowProps props = WindowProps());

   ~Window();

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    inline bool is_closed() const { return glfwWindowShouldClose(m_Window); }

    inline void swap() const { glfwSwapBuffers(m_Window); }

    inline float delta_time() const { return m_DeltaTime; }

    void update() {
        static float last_time = 0.0f;

        float current_time = static_cast<float>(glfwGetTime());
        m_DeltaTime = current_time - last_time;
        last_time = current_time;

        if constexpr (HasT) {
            static constexpr int keys[] = {
                GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D,
                GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT
            };
    
            auto get_pressed = [&](GLFWwindow* w) -> int {
                for (int k : keys)
                    if (glfwGetKey(w, k) == GLFW_PRESS) return k;
                return -1;
            };
    
            if (int key = get_pressed(m_Window); key != -1)
                detail::Win<T>::m_Camera.update_position(m_DeltaTime, key);
        }
    }
};


using WinNoCamera       = Window<void>;
using WinOrtographic    = Window<Ortographic>;
using WinPerspective    = Window<Perspective>;

}
