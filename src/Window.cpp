#include "Camera.hpp"
#include <core/logging.hpp>
#ifdef assert
#undef assert
#endif
#include <core/massert.hpp>
#include <core/common.hpp>
#include <Window.hpp>

namespace etugl {

template <typename T>
Window<T>::Window(const WindowProps props) : m_Props(props) 
{
    massert(glfwInit(), "GLFW init: FAILED");
    core::logging::info("GLFW init: SUCCESS");

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
    core::logging::info("Window create: SUCCESS", VAR(window));
    glfwMakeContextCurrent(m_Window);

    massert(gladLoadGL(), "GLAD loading: FAILED");
    core::logging::info("GLAD loading: SUCCESS");

    const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    core::logging::debug("GL Vendor", VAR(vendor));
    core::logging::debug("GL Renderer", VAR(renderer));
    core::logging::debug("GL Version", VAR(version));

    #ifdef __APPLE__
        const char *glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        core::logging::debug("GLSL Version", VAR(glslVersion));
    #endif

    glClearColor(m_Props.bg_r(), m_Props.bg_g(), m_Props.bg_b(), m_Props.bg_a());
    glfwSetWindowUserPointer(m_Window, this);

    core::logging::debug("Window: setup window resize callback", VAR(window));
    glfwSetWindowSizeCallback(this->m_Window, [](GLFWwindow* window, int width, int height) {
        auto* self = static_cast<Window<T>*>(glfwGetWindowUserPointer(window));
        self->m_Props.m_Width = width;
        self->m_Props.m_Height = height;

        if constexpr (HasT) {
            self->m_Camera.width() = width;
            self->m_Camera.height() = height;
        }

        glViewport(0, 0, width, height);
    });

    if constexpr (HasT) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        core::logging::debug("Window: setup cursor pos callback", VAR(window));
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double pos_x, double pos_y) {
            auto* self = static_cast<Window<T>*>(glfwGetWindowUserPointer(window));
            self->m_Camera.update_front(static_cast<float>(pos_x), static_cast<float>(pos_y));
        });


        if constexpr (std::is_same_v<T, Perspective>) {

            core::logging::debug("Window: setup scroll callback", VAR(window));
            glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double offset_x, double offset_y) {
                auto* self = static_cast<Window<T>*>(glfwGetWindowUserPointer(window));
                self->m_Camera.update_fov(offset_y);
            });
        } 
    }
}

template <typename T>
Window<T>::~Window() {
    const void* window = static_cast<const void*>(m_Window);
    glfwDestroyWindow(m_Window);
    core::logging::info("Window destroy: SUCCESS", VAR(window));
}

template class Window<void>;
template class Window<Ortographic>;
template class Window<Perspective>;

}
