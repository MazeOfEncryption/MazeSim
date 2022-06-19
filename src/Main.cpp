#include "Window.hpp"

using namespace MazeSim;
int main() {
    Window win = Window("Test", 1280, 720);

    while (win.active()) {
        win.clear();
        win.draw();
    }
    return 0;
}