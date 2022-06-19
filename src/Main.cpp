#include "Window.hpp"

using namespace MazeSim;
int main () {
	Window win = Window("Test", 800, 600);
	
	while (win.active()) {
		win.clear();
		win.draw();
	}
	return 0;
}