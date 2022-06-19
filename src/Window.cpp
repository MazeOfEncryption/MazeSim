#include "Window.hpp"
void MazeSim::Window::glfwWindowHints() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
}
void MazeSim::Window::glfwErrorCallback (int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
void MazeSim::Window::init() {
    glfwInit();
	glfwSetErrorCallback(glfwErrorCallback);
	Window::glfwWindowHints();
	const char *title = this->title.c_str();
	this->window = glfwCreateWindow(this->width, this->height, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glEnable(GL_DEBUG_OUTPUT);

	glfwSwapInterval(1); // Enable vsync

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

	this->showDemo = true;
} 
bool MazeSim::Window::active() {
	return !glfwWindowShouldClose(this->window);
}
void MazeSim::Window::clear() {

}
void MazeSim::Window::draw() {
	this->time = glfwGetTime();
	this->dt = this->time - this->startTime;
	this->startTime = this->time;

	glfwPollEvents();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (this->showDemo)
            ImGui::ShowDemoWindow(&this->showDemo);

	ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(this->window);
}
MazeSim::Window::Window(std::string title, int width, int height) {
	std::cout << "Window constructor called." << std::endl;
	this->title = title;
	this->width = width;
	this->height = height;
	this->startTime = 0.0;
	this->dt = 0.0;
	this->time = 0.0;
	this->init();
}
MazeSim::Window::~Window() {
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->window);
    glfwTerminate();
}