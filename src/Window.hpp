#ifndef MAZESIM_WINDOW
#define MAZESIM_WINDOW
#include "Dep.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
namespace MazeSim {
	class Window {
		public:
			bool active();
			void clear();
			void draw();
			Window(std::string title, int width, int height);
			~Window();
			GLFWwindow* window;
			double startTime, time, dt;
		private:
			static void glfwErrorCallback(int error, const char* description);
			void init();
			std::string title;
			int width, height;
			void glfwWindowHints();

			bool showDemo = false;
			bool showOther = false;
	};
}
#endif