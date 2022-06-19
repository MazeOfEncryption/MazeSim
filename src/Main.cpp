#include "Window.hpp"

using namespace MazeSim;
int main() {
    Window win = Window("Test", 1280, 720);
    bool showDemo = false;
    while (win.active()) {
        win.clear();
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Help")) {
                ImGui::MenuItem("Show Demo Window", NULL, &showDemo);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        if (showDemo) ImGui::ShowDemoWindow(&showDemo);
        win.draw();
    }
    return 0;
}