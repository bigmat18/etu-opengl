#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include <Types.hpp>

namespace etugl {

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

class Window {
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

    inline void swap_buffers() const {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    inline void clear_color(float r, float g, float b, float a) const {
        glClearColor(r, g, b, a);
    }

    inline void clear() const {
        glClear(GL_COLOR_BUFFER_BIT);
    }

};

}
