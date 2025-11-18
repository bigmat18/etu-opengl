#include <Window.hpp>
#include <utils.hpp>

namespace etugl {

Window::Window(const WindowProps props) : m_Props(props) 
{
    massert(glfwInit(), "Could not initialize GLFW");
    LOG_INFO("GLFW sucessfully inizialized");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    massert(m_Window = glfwCreateWindow(m_Props.m_Width, 
                                        m_Props.m_Width,
                                        m_Props.m_Title.c_str(), 
                                        NULL, NULL),
             "Error in window creation");


    LOG_INFO("Window sucessfully created");
    glfwMakeContextCurrent(m_Window);

    massert(gladLoadGL(), "Error to initialize GLAD");
    LOG_INFO("GLAD successfully loaded");

    const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    LOG_INFO("GL Vendor          : {}", vendor);
    LOG_INFO("GL Renderer        : {}", renderer);
    LOG_INFO("GL Version (string): {}", version);
    
    #ifdef __APPLE__
        const char *glslVersion = reinterpret_cast<const char*>(glGetStringI(GL_SHADING_LANGUAGE_VERSION));
        LOG_INFO("GLSL Version       : {}", glslVersion);
    #endif

    clear_color(m_Props.bg_r(), m_Props.bg_g(), m_Props.bg_b(), m_Props.bg_a());
}


Window::~Window() {
    glfwDestroyWindow(m_Window);
    LOG_INFO("Window sucessfully destroyed");
}

}
