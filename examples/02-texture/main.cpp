#include "Texture.hpp"
#include <Buffers.hpp>
#include <Program.hpp>
#include <Types.hpp>
#include <VertexArray.hpp>
#include <VertexLayout.hpp>

#include <Window.hpp>
#include <filesystem>
#include <print>
#include <vector>

static const std::vector<float> vertices = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

static const std::vector<etugl::u32> indices = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

int main (int argc, char *argv[]) { 
    const fs::path path = fs::path(std::string(argv[argc-1])).parent_path();

    // Window creation with OpenGL context
    etugl::Window window = etugl::WinNoCamera();

    // Create vao with VBO + EBO + Layout
    etugl::VerterArray vao(
        vertices, indices, 
        etugl::VertexLayout()
            .add<etugl::LayoutType::Float3>(0)
            .add<etugl::LayoutType::Float3>(1)
            .add<etugl::LayoutType::Float2>(2)
    );

    // Create program with VertexShader + FragmentShader
    etugl::Program program(path/"vs.glsl", path/"fs.glsl");

    // Load and create texture
    etugl::Texture2D texture(path/"assets/awesomeface.png");
   
    // Tell openglfor each sampler to which texture unit it belogs to
    program.bind();
    program.set_int("u_Texture", 0);

    // Renderer loop
    while (!window.is_closed()) {
        glClear(GL_COLOR_BUFFER_BIT);

        texture.bind();
        program.bind();
        vao.draw();

        window.swap();
        glfwPollEvents();
    }

    return 0;
}
