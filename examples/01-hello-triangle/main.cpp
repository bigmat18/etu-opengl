#include <Buffers.hpp>
#include <Program.hpp>
#include <Types.hpp>
#include <VertexArray.hpp>
#include <VertexLayout.hpp>
#include <Window.hpp>
#include <filesystem>
#include <vector>

static const std::vector<float> vertices = {
    -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

static const std::vector<etugl::u32> indices = { 0, 1, 2 };


int main (int argc, char** argv) {
    const fs::path path = fs::path(std::string(argv[argc-1])).parent_path();

    // Window creation with OpenGL context
    etugl::Window window = etugl::WinNoCamera();

    // Create vao with VBO + EBO + Layout
    etugl::VertexArray vao(
        vertices, indices, 
        etugl::VertexLayout()
            .add<etugl::LayoutType::Float3>(0)
            .add<etugl::LayoutType::Float3>(1)
    );

    // Create program with VertexShader + FragmentShader
    etugl::Program program(path/"vs.glsl", path/"fs.glsl");
    
    // Renderer loop
    while (!window.is_closed()) {
        glClear(GL_COLOR_BUFFER_BIT);

        program.bind();
        vao.draw();

        window.swap();
        glfwPollEvents();
    }

    return 0;
}
