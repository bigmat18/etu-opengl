#include <Window.hpp>

int main (void) {
    etugl::Window window = etugl::Window();
    
    while (!window.is_closed()) {
        window.clear();

        window.swap_buffers();
    }

    return 0;
}
