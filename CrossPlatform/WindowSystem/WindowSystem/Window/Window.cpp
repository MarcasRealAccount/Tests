#include "Window.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#undef CreateWindow

#ifdef _WIN32
uint16_t Window::wca = 0;
HINSTANCE Window::instance = nullptr;
#endif

std::unordered_set<Window*> Window::createdWindows;

void Window::CreateWindow() {
#ifdef _WIN32
	if (Window::wca) return;

	HWND hwnd = CreateWindowExA(
		0x0,									// dwExStyle
		reinterpret_cast<LPCSTR>(Window::wca),	// lpClassName
		"Window",								// lpWindowName
		WS_OVERLAPPEDWINDOW,					// dwStyle
		CW_USEDEFAULT,							// X
		CW_USEDEFAULT,							// Y
		CW_USEDEFAULT,							// nWidth
		CW_USEDEFAULT,							// nHeight
		nullptr,								// hWndParent
		nullptr,								// hMenu
		Window::instance,						// hInstance
		this									// lpParam
	);

	if (!hwnd) return;

	ShowWindow(hwnd, SW_SHOW);
#endif

	Window::createdWindows.insert(this);
}

void Window::DestroyWindow() {
#ifdef _WIN32
	if (!this->hwnd) return;

	::DestroyWindow(this->hwnd);
#endif
}

bool Window::CloseRequested() const {
	return this->closeRequested;
}

void Window::PollEvents() {
#ifdef _WIN32
	MSG msg{};
	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			for (Window* window : Window::createdWindows) window->DestroyWindow();
		} else {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
#endif
}

bool Window::InitWindows() {
#ifdef _WIN32
	HMODULE mod;
	if (!GetModuleHandleExA(0x0, nullptr, &mod)) return false;
	Window::instance = reinterpret_cast<HINSTANCE>(mod);

	WNDCLASSEXA wc{
		sizeof(WNDCLASSEX),							// cbSize
		0x0,										// style
		Window::WindowProc,							// lpfnWndProc
		0x0,										// cbClsExtra
		0x0,										// cbWndExtra
		Window::instance,							// hInstance
		nullptr,									// hIcon
		nullptr,									// hCursor
		reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),	// hbrBackground
		nullptr,									// lpszMenuName
		"WindowSystemClass",						// lpszClassName
		nullptr										// hIconSm
	};
	Window::wca = RegisterClassExA(&wc);
	return Window::wca;
#endif
}

#ifdef _WIN32
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
		switch (uMsg) {
		case WM_CLOSE:
			window->closeRequested = true;
			return 0;
		case WM_DESTROY:
			window->hwnd = nullptr;
			window->closeRequested = false;
			Window::createdWindows.erase(window);
			return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(window->hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(window->hwnd, &ps);
			return 0;
		}
		}
	}

	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}
#endif