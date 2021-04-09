#pragma once

#pragma warning(disable:4302)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <map>
#include <set>

namespace SimpleWin32 {
	class Window {
	public:
		static LRESULT CALLBACK defaultEventProcessor(
			HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam
		);
		struct InitArg {
			LPCWSTR class_name = L"Window";
			LPCWSTR title;
			UINT class_style = CS_HREDRAW | CS_VREDRAW;
			HICON icon = NULL, icon_small = NULL;
			HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
			HBRUSH background_brush = (HBRUSH)(COLOR_WINDOW + 1);
			LPCWSTR menu_name = MAKEINTRESOURCE(IDC_ICON);
			int x = 0, y = 0;
			int width = 640, height = 480;
			DWORD style = WS_OVERLAPPEDWINDOW;
			WNDPROC event_processor = &Window::defaultEventProcessor;
		};
	private:
		LPCWSTR const class_name, title;
		WNDCLASSEX const classex;
		HINSTANCE const instance;
		HWND const window;
		HWND createWindow(InitArg const &args);
	public:
		Window(HINSTANCE hInstance, InitArg const args);
		int run();
	};
	class DrawingContext;	// pre-declaration
	class EventHandler {
	public:
		using Handler = LRESULT(*)(EventHandler *self, ...);	// Safety might be a choice
		using Median = LRESULT(*)(EventHandler *self, Handler, HWND, WPARAM, LPARAM);
	private:
		HWND hWnd;
		static std::map<UINT, std::set<Handler>> handlers;
		static std::map<UINT, Median> medians;
	public:
		EventHandler() = default;
		LRESULT CALLBACK operator()(
			HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam
			);
		bool setMedian(UINT type, Median f);
		template<typename T>
		bool setMedian(UINT type, T f) {
			setMedian(type, (Median)f);
		}
		void addHandler(UINT type, Handler handler);
		template<typename T>
		void addHandler(UINT type, T handler) {
			addHandler(type, (Handler)handler);
		}
		DrawingContext getDrawingContext();
		static LRESULT defaultPaintMedian(
			EventHandler *self, Handler handler, HWND hWnd, WPARAM, LPARAM
		);
		RECT getWindowRect();
		void setWindowRect(RECT rect);
		BOOL triggerRepaint();
		BOOL triggerRepaint(RECT rect);
		static LRESULT defaultDestroyHandler(HWND, WPARAM, LPARAM);
	};
	class DrawingContext {
		HWND &hWnd;
		HDC const hdc;
		static PAINTSTRUCT ps;
		// HOBJ
		static HBRUSH hBrush;
		static HPEN hPen;
		static HFONT hFont;
	public:
		void setSolidBrush(COLORREF color);
		void setPen(int style, int width, COLORREF color);
		struct FontArg {
			LPCWSTR name;
			int
				height, width = 0,
				escapement = 0, orientation = 0,
				weight = FW_NORMAL;
			DWORD
				italic = FALSE,
				underline = FALSE,
				strikeout = FALSE,
				charset = DEFAULT_CHARSET,
				out_precision = OUT_DEFAULT_PRECIS,
				clip_precision = CLIP_DEFAULT_PRECIS,
				quality = DEFAULT_QUALITY,
				family = FF_DONTCARE;
		};
		void setFont(FontArg args);
		void drawRect(RECT rect);
		void drawText(LPCWSTR text, RECT rect, UINT format = 0);
		SIZE measureText(LPCWSTR text, int count);
		DrawingContext(HWND &hWnd);
		~DrawingContext();
	};
}