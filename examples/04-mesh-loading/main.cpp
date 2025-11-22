#include "Camera.hpp"
#include "Model.hpp"
#include <Buffers.hpp>
#include <Program.hpp>
#include <Types.hpp>
#include <VertexArray.hpp>
#include <VertexLayout.hpp>
#include <Window.hpp>
#include <filesystem>

 
int main (int argc, char *argv[]) { 
    const fs::path path = fs::path(std::string(argv[argc-1])).parent_path();
 
    // Window creation with OpenGL context
    etugl::Window window = etugl::WinPerspective();
    etugl::Camera& camera = window.camera();

    etugl::Model mesh(path/"assets/scene.obj"); 
    mesh.bind();

    glEnable(GL_DEPTH_TEST);

    // Create program with VertexShader + FragmentShader
    etugl::Program program(path/"vs.glsl", path/"fs.glsl");

    // Renderer loop
    while (!window.is_closed()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        window.update();

        program.bind();    
        etugl::mat4f model(1.0f);
        etugl::mat4f view = camera.view();
        etugl::mat4f projection = camera.projection();
           
        program.set_mat4f("u_Model", model);
        program.set_mat4f("u_View", view);
        program.set_mat4f("u_Projection", projection);
        mesh.draw();

        window.swap();
        glfwPollEvents();
    }

    return 0;
}
