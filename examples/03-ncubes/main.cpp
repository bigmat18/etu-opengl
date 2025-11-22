#include "Camera.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <Buffers.hpp>
#include <Program.hpp>
#include <Types.hpp>
#include <VertexArray.hpp>
#include <VertexLayout.hpp>
#include <Window.hpp>
#include <filesystem>

static const std::vector<float> vertices = {
    // positions           // colors
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f, 
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f, 
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f, 
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 
    -0.5f,  0.5f,  0.5f,   0.2f, 0.2f, 0.2f  
}; 
 
static const std::vector<unsigned int> indices = {
    0, 1, 2,  2, 3, 0,
    4, 6, 5,  6, 4, 7,
    0, 3, 7,  7, 4, 0,
    1, 5, 6,  6, 2, 1,
    0, 4, 5,  5, 1, 0,
    3, 2, 6,  6, 7, 3
}; 
 
int main (int argc, char *argv[]) { 
    const fs::path path = fs::path(std::string(argv[argc-1])).parent_path();
 
    // Window creation with OpenGL context
    etugl::Window window = etugl::WinPerspective();
    etugl::Camera& camera = window.camera();

    // Create vao with VBO + EBO + Layout
    etugl::VerterArray vao(
        vertices, indices, 
        etugl::VertexLayout()
            .add<etugl::LayoutType::Float3>(0)
            .add<etugl::LayoutType::Float3>(1)
    );

    glEnable(GL_DEPTH_TEST);

    // Create program with VertexShader + FragmentShader
    etugl::Program program(path/"vs.glsl", path/"fs.glsl");

    // Renderer loop
    while (!window.is_closed()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        window.update();

        program.bind();    
        etugl::mat4f view = camera.view();
        etugl::mat4f projection = camera.projection();
            
        program.set_mat4f("u_View", view);
        program.set_mat4f("u_Projection", projection);

        const int cube_per_side = 10;
        const float scale_factor = 2.0f;

        for (int x = 0; x < cube_per_side; x++) {
            for (int y = 0; y < cube_per_side; y++) {
                for (int z = 0; z < cube_per_side; z++) {
                    etugl::mat4f model(1);
                    model = glm::translate(model, 
                                           etugl::vec3f(x * scale_factor, 
                                                        y * scale_factor, 
                                                        z * scale_factor));
                    program.set_mat4f("u_Model", model);
                    vao.draw();
                }
            }
        }

        window.swap();
        glfwPollEvents();
    }

    return 0;
}
