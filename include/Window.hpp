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

public:
    Window(const WindowProps props = WindowProps());

   ~Window();

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    inline bool is_closed() const {
        return glfwWindowShouldClose(m_Window);
    }

    inline void swap() const { glfwSwapBuffers(m_Window); }
};


    
using WinNoCamera       = Window<void>;
using WinOrtographic    = Window<Ortographic>;
using WinPerspective    = Window<Perspective>;

}
