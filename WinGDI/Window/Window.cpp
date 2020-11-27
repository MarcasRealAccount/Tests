#include "Window.h"

#include <Windows.h>

#undef CreateWindow

uint16_t Window::wca = 0;
HINSTANCE Window::instance = nullptr;
std::unordered_set<Window*> Window::createdWindows;

void Window::CreateWindow() {
	if (!Window::wca) return;

	HWND hwnd = CreateWindowExA(
		0x0,					// dwExStyle
		reinterpret_cast<LPCSTR>(Window::wca),	// lpClassName
		"Window",				// lpWindowName
		WS_OVERLAPPEDWINDOW,	// dwStyle
		CW_USEDEFAULT,			// X
		CW_USEDEFAULT,			// Y
		CW_USEDEFAULT,			// nWidth
		CW_USEDEFAULT,			// nHeight
		nullptr,				// hWndParent
		nullptr,				// hMenu
		Window::instance,		// hInstance
		this					// lpParam
		);
	
	if (!hwnd) return;

	Window::createdWindows.insert(this);

	ShowWindow(hwnd, SW_SHOW);
}

void Window::DestroyWindow() {
	if (!this->hwnd) return;

	::DestroyWindow(this->hwnd);
}

bool Window::CloseRequested() const {
	return this->closeRequested || !this->hwnd;
}

int64_t Window::HandleMessage(uint32_t uMsg, uint64_t wParam, int64_t lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		this->closeRequested = true;
		return 0;
	case WM_DESTROY:
		this->hwnd = nullptr;
		this->closeRequested = false;
		Window::createdWindows.erase(this);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(this->hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(this->hwnd, &ps);
		return 0;
	}
	default:
		return DefWindowProcA(this->hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

bool Window::Init() {
	HMODULE mod;
	if (!GetModuleHandleExA(0x0, nullptr, &mod)) return false;
	Window::instance = reinterpret_cast<HINSTANCE>(mod);

	WNDCLASSEXA wc{
		sizeof(WNDCLASSEX),	// cbSize
		0x0,				// style
		Window::WindowProc,	// lpfnWndProc
		0x0,				// cbClsExtra
		0x0,				// cbWndExtra
		Window::instance,	// hInstance
		nullptr,			// hIcon
		nullptr,			// hCursor
		reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),	// hbrBackground
		nullptr,			// lpszMenuName
		"WindowClass",		// lpszClassName
		nullptr				// hIconSm
	};
	Window::wca = RegisterClassExA(&wc);
	return Window::wca;
}

void Window::UpdateWindows() {
	MSG msg{};
	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			for (Window* window : Window::createdWindows) {
				window->DestroyWindow();
			}
		} else {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}

int64_t __stdcall Window::WindowProc(HWND hwnd, uint32_t uMsg, uint64_t wParam, int64_t lParam) {
	Window* window = nullptr;

	if (uMsg == WM_NCCREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		window = reinterpret_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

		window->hwnd = hwnd;
	} else {
		window = reinterpret_cast<Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
	}

	if (window) {
		return window->HandleMessage(uMsg, wParam, lParam);
	} else {
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}
}