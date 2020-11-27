#pragma once

#include <unordered_set>

#ifdef _WIN32
#include <stdint.h>

struct HWND__;
typedef struct HWND__* HWND;
struct HINSTANCE__;
typedef struct HINSTANCE__* HINSTANCE;
#endif

class WindowSystem;

struct Window {
public:
	void CreateWindow();
	void DestroyWindow();

	bool CloseRequested() const;

	friend WindowSystem;

public:
	static void PollEvents();

private:
	static bool InitWindows();

private:
#ifdef _WIN32	// Windows specific functions.
	static int64_t __stdcall WindowProc(HWND hwnd, uint32_t uMsg, uint64_t wParam, int64_t lParam);
#endif

private:
#ifdef _WIN32	// Windows specific Window data.
	HWND hwnd = nullptr;
#endif

	bool closeRequested = false;

private:
#ifdef _WIN32	// Windows specific Window data.
	static uint16_t wca;
	static HINSTANCE instance;
#endif

	static std::unordered_set<Window*> createdWindows;
};