#include "WindowSystem.h"

bool WindowSystem::Init() {
	if (!Window::InitWindows()) return false;
	return true;
}
