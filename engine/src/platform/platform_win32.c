#include "platform.h"
#include "core/logger.h"

// Windows platform layer
#if PLATFORM_WINDOWS

#include <Windows.h>
#include <windowsx.h> // param input extraction

typedef struct {
	HINSTANCE instance;
	HWND window;
} internal_state;

// Clock
static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(
	HWND window,
	UINT message,
	WPARAM wparam,
	LPARAM lparam
) {
	switch (message) {
		case WM_ERASEBKGND:
			// Notify the OS that erasing will be handled by the application to prevent flicker.
			return 1;
		case WM_CLOSE:
			// TODO: Fire an event for the application to quit.
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE: {
			// Get the updated size.
			// RECT r;
			// GetClientRect(hwnd, &r);
			// u32 width = r.right - r.left;
			// u32 height = r.bottom - r.top;

			// TODO: Fire an event for window resize.
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			// Key pressed/released
			// b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
			// TODO: input processing
		} break;
		case WM_MOUSEMOVE: {
			// Mouse move
			// i32 x_position = GET_X_LPARAM(l_param);
			// i32 y_position = GET_Y_LPARAM(l_param);
			// TODO: input processing.
		} break;
		case WM_MOUSEWHEEL: {
			// i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
			// if (z_delta != 0) {
			// 	// Flatten the input to an OS-independent (-1, 1)
			//	z_delta = (z_delta < 0) ? -1 : 1;
			//	// TODO: input processing.
			// }
		}
		break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP: {
			// b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
			//  TODO: input processing.
		} break;
	}

	return DefWindowProcA(window, message, wparam, lparam);
}

b8 platform_startup(
	platform_state* plat_state,
	const char* app_name,
	i32 x, i32 y,
	i32 width,
	i32 height
) {
	plat_state->internal_state = malloc(sizeof(internal_state));
	internal_state* state = (internal_state*)plat_state->internal_state;
	
	state->instance = GetModuleHandleA(0);

	// Setup and register window class.
	WNDCLASSA wc = {0};
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = win32_process_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = state->instance;
	wc.hIcon = LoadIconA(state->instance, IDI_APPLICATION);
	wc.hCursor = LoadCursorA(state->instance, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = "shitasswindowclass";

	if (!RegisterClassA(&wc)) {
		MessageBoxA(0, "Window registration failed", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

	// Create window
	//u32 client_x = x;
	//u32 client_y = y;
	//u32 client_width = width;
	//u32 client_height = height;

	u32 window_x = x;
	u32 window_y = y;
	u32 window_width = width;
	u32 window_height = height;

	u32 window_style = WS_OVERLAPPEDWINDOW;
	u32 window_ex_style = WS_EX_APPWINDOW;

	RECT border_rect = {0};
	AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

	window_x += border_rect.left;
	window_y += border_rect.top;

	window_width += border_rect.right - border_rect.left;
	window_height += border_rect.bottom - border_rect.top;

	HWND window_handle = CreateWindowExA(
		window_ex_style, "shitasswindowclass", app_name,
		window_style, window_x, window_y, window_width, window_height,
		0, 0, state->instance, 0
	);

	if (!window_handle) {
		MessageBoxA(0, "Window creation failed!", "Error", MB_ICONERROR | MB_OK);
		LOG_FATAL("Window creation failed");
		return false;
	} else {
		state->window = window_handle;
	}

	// Show the window
	b32 should_activate = true; // NOTE: if the window should not accept input, this should be false
	i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWMINNOACTIVE;
	// If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE
	// If initially minimized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
	ShowWindow(state->window, show_window_command_flags);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.0 / (f64)frequency.QuadPart;
	QueryPerformanceCounter(&start_time);

	return true;
}

void platform_shutdown(platform_state* plat_state) {
	// Simply cold cast to the known type
	internal_state* state = (internal_state*)plat_state->internal_state;

	if (state->window) {
		DestroyWindow(state->window);
		state->window = NULL;
	}
}

b8 platform_pump_messages(platform_state* plat_state) {
	MSG message;
	while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}

	return true;
}

void* platform_allocate(u64 size, b8 aligned) {
	return malloc(size);
}

void platform_free(void* block, b8 aligned) {
	free(block);
}

void* platform_zero_memory(void* block, u64 size) {
	return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
	return memcpy(dest, source, size);
}

void* platform_set_memory(void* block, i32 value, u64 size) {
	return memset(block, value, size);
}

void platform_console_write(const char* message, u8 color) {
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// FATAL, ERROR, WARN, INFO, DEBUG, TRACE
	static u8 levels[] = {64, 4, 6, 2, 1, 8};
	SetConsoleTextAttribute(console_handle, levels[color]);

	OutputDebugStringA(message);
	u64 length = strlen(message);
	LPDWORD number_written = 0;
	WriteConsoleA(console_handle, message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char* message, u8 color) {
	HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
	// FATAL, ERROR, WARN, INFO, DEBUG, TRACE
	static u8 levels[] = {64, 4, 6, 2, 1, 8};
	SetConsoleTextAttribute(console_handle, levels[color]);

	OutputDebugStringA(message);
	u64 length = strlen(message);
	LPDWORD number_written = 0;
	WriteConsoleA(console_handle, message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time() {
	LARGE_INTEGER now_time;
	QueryPerformanceCounter(&now_time);
	return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
	Sleep(ms);
}

#endif
