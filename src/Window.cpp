#include "Camera.hpp"
#include "logging.hpp"
#include <Window.hpp>

namespace etugl {

template <typename T>
Window<T>::Window(const WindowProps props) : m_Props(props) 
{
    massert(glfwInit(), "GLFW init: FAILED");
    LOG_INFO("GLFW init: SUCCESS");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    massert(m_Window = glfwCreateWindow(
        m_Props.m_Width, m_Props.m_Height,
        m_Props.m_Title.c_str(), NULL, NULL),
            "Window creation: FAILED");

    const void* window = static_cast<const void*>(m_Window);
    LOG_INFO("Window {} create: SUCCESS", window);
    glfwMakeContextCurrent(m_Window);

    massert(gladLoadGL(), "GLAD loading: FAILED");
    LOG_INFO("GLAD loading: SUCCESS");

    const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    LOG_DEBUG("GL Vendor: {}", vendor);
    LOG_DEBUG("GL Renderer: {}", renderer);
    LOG_DEBUG("GL Version (string): {}", version);

    #ifdef __APPLE__
        const char *glslVersion = reinterpret_cast<const char*>(glGetStringI(GL_SHADING_LANGUAGE_VERSION));
        LOG_DEBUG("GLSL Version: {}", glslVersion);
    #endif

    glClearColor(m_Props.bg_r(), m_Props.bg_g(), m_Props.bg_b(), m_Props.bg_a());
    glfwSetWindowUserPointer(m_Window, this);

    if constexpr (HasT) {

        LOG_DEBUG("Window {}: setup set key callback", window);
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (action != GLFW_PRESS) 
                return;

            auto* self = static_cast<Window<T>*>(glfwGetWindowUserPointer(window));
            self->m_Camera.update_position(0, key);
        });
    }
}

template <typename T>
Window<T>::~Window() {
    const void* window = static_cast<const void*>(m_Window);
    glfwDestroyWindow(m_Window);
    LOG_INFO("Window {} destroy: SUCCESS", window);
}

template class Window<void>;
template class Window<Ortographic>;
template class Window<Perspective>;

}
