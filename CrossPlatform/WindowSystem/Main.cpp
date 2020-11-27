#include "WindowSystem/WindowSystem.h"

int main() {
	if (!WindowSystem::Init()) return 1;

	Window window1, window2;
	window1.CreateWindow();
	window2.CreateWindow();
	while (!window1.CloseRequested() || !window2.CloseRequested()) {
		Window::PollEvents();
	}
	window1.DestroyWindow();
	window2.DestroyWindow();

	return 0;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, const char* lpCmdLine, int nCmdShow) {
	return main();
}