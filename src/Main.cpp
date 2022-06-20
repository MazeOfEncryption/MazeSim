#include "Dep.hpp"
#include <set>
#include <iostream>
#include <random>
#include "Point.hpp"
#include "Window.hpp"
using namespace MazeSim;
// using Point = std::pair<int, int>;

Point toRectangular(Point in) {
    return {in.second % 2 == 0 ? in.first * 2 : in.first * 2 + 1, in.second};
}

void ShowCanvas(bool *p_open) {
    if (!ImGui::Begin("Example: Custom rendering", p_open)) {
        ImGui::End();
        return;
    }

    static ImVector<ImVec2> points;
    static ImVec2 scrolling(0.0f, 0.0f);
    static float zoom = 1.0;
    static bool opt_enable_grid = true;
    static bool opt_enable_context_menu = true;
    static bool adding_line = true;

    static std::set<Point> board;
    static std::mt19937 gen(0);
    static std::uniform_int_distribution<int> dis(-4, 4);

    // board.insert({ 0,  0});
    // board.insert({ 0,  1});
    // board.insert({ 1,  2});
    // board.insert({ 1,  1});
    // board.insert({ 1, -1});

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
    if (ImGui::Button("Re-gen")) {
        adding_line = true;
    }
    ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.\nScroll wheel: Up to zoom in, Down to scroll out.");

    if(adding_line) {
        board.clear();
        points.clear();
        for (int i = 0; i < 10; i++) {
            board.insert({dis(gen), dis(gen)});
        }
    }

    // Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
    // Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
    // To use a child window instead we could use, e.g:
    //      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
    //      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
    //      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_NoMove);
    //      ImGui::PopStyleColor();
    //      ImGui::PopStyleVar();
    //      [...]
    //      ImGui::EndChild();

    // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
    const float boxSize = 32.0f;


    if (adding_line) {
        for (Point point : board) {
            Point rect = toRectangular(point);
            std::cout << point << " -> " << rect << std::endl;
            if (point.second % 2 == 0) {
                points.push_back(ImVec2(rect.first * boxSize + boxSize + boxSize, -rect.second * boxSize));
                points.push_back(ImVec2(rect.first * boxSize + boxSize - boxSize, -rect.second * boxSize));
            } else {
                points.push_back(ImVec2(rect.first * boxSize + boxSize, -rect.second * boxSize + boxSize));
                points.push_back(ImVec2(rect.first * boxSize + boxSize, -rect.second * boxSize - boxSize));
            }
        }
        adding_line = false;
    }

    // Add first and second point
    // if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    //     points.push_back(mouse_pos_in_canvas);
    //     points.push_back(mouse_pos_in_canvas);
    //     adding_line = true;
    // }
    // if (adding_line) {
    //     points.back() = mouse_pos_in_canvas;
    //     if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
    //         adding_line = false;
    // }

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
    const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan)) {
        scrolling.x += io.MouseDelta.x;
        scrolling.y += io.MouseDelta.y;
    }

    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    if (ImGui::BeginPopup("context")) {
        if (adding_line)
            points.resize(points.size() - 2);
        adding_line = false;
        if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
        if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
        ImGui::EndPopup();
    }

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid) {
        const float GRID_STEP = 64.0f;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
    }
    for (int n = 0; n < points.Size; n += 2)
        draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
    draw_list->PopClipRect();
    ImGui::End();
}
bool showCanvas = true;
int main() {
    // Point w{1, 1};

// std::cout << w << std::endl;


// for (Point point : board) {
//     std::cout << point << " -> " << toRectangular(point) << std::endl;
// }

    Window win = Window("Test", 1280, 720);
    bool showDemo = false;
    while (win.active()) {
        win.clear();
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Help")) {
                ImGui::MenuItem("Show Demo Window", NULL, &showDemo);
                ImGui::MenuItem("Show Canvas Window", NULL, &showCanvas);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        if (showDemo) ImGui::ShowDemoWindow(&showDemo);
        if (showCanvas) ShowCanvas(&showCanvas);
        win.draw();
    }
    return 0;
}
