#pragma once

#include <stdint.h>
#include <unordered_set>

struct HWND__;
typedef struct HWND__* HWND;
struct HINSTANCE__;
typedef struct HINSTANCE__* HINSTANCE;

class Window {
public:
	void CreateWindow();
	void DestroyWindow();

	bool CloseRequested() const;

private:
	int64_t HandleMessage(uint32_t uMsg, uint64_t wParam, int64_t lParam);

public:
	static bool Init();
	static void UpdateWindows();

private:
	static int64_t __stdcall WindowProc(HWND hwnd, uint32_t uMsg, uint64_t wParam, int64_t lParam);

private:
	HWND hwnd = nullptr;
	bool closeRequested = false;

private:
	static uint16_t wca;
	static HINSTANCE instance;
	static std::unordered_set<Window*> createdWindows;
};