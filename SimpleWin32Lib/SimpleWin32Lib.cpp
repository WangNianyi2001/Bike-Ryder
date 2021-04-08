#include "SimpleWin32Lib.h"

using namespace SimpleWin32;

// Window

LRESULT CALLBACK Window::defaultEventProcessor(
	HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam
) {
	if(type == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, type, wParam, lParam);
}

HWND Window::createWindow(InitArg const &args) {
	RegisterClassExW(&this->classex);
	HWND hWnd = CreateWindow(
		this->class_name, this->title, args.style,
		args.x, args.y,
		args.width, args.height,
		nullptr, nullptr, this->instance, nullptr
	);
	return hWnd;
}

Window::Window(HINSTANCE hInstance, InitArg const args) :
	class_name(args.class_name),
	title(args.title),
	classex({
		.cbSize = sizeof(WNDCLASSEX),
		.style = args.class_style,
		.lpfnWndProc = args.event_processor,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = args.icon,
		.hCursor = args.cursor,
		.hbrBackground = args.background_brush,
		.lpszMenuName = args.menu_name,
		.lpszClassName = args.class_name,
		.hIconSm = args.icon_small,
	}),
	instance(hInstance),
	window(createWindow(args))
{
	// Not a single line is really needed here.
}

int Window::run() {
	if(!this->window)
		return FALSE;
	ShowWindow(this->window, SW_SHOW);
	UpdateWindow(this->window);
	// Message handling
	HACCEL accelerate_table = LoadAccelerators(
		this->instance,
		MAKEINTRESOURCE(IDC_ICON)
	);
	MSG *message = new MSG;
	while(GetMessage(message, nullptr, 0, 0)) {
		if(!TranslateAccelerator(
			message->hwnd, accelerate_table, message
		)) {
			TranslateMessage(message);
			DispatchMessage(message);
		}
	}
	return message->wParam;
}

// EventHandler

std::map<UINT, std::set<EventHandler::Handler>> EventHandler::handlers{};
std::map<UINT, EventHandler::Median> EventHandler::medians{};

LRESULT CALLBACK EventHandler::operator()(
	HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam
) {
	this->hWnd = hWnd;
	auto &median_functions = EventHandler::medians;
	auto &handlers = EventHandler::handlers;
	if(!handlers.count(type))
		return DefWindowProc(hWnd, type, wParam, lParam);
	LRESULT res = 0;
	if(median_functions.count(type)) {
		Median const median = median_functions[type];
		for(Handler const handler : handlers[type])
			res |= median(this, handler, hWnd, wParam, lParam);
		return res;
	}
	for(Handler const handler : handlers[type])
		res |= handler(this, hWnd, wParam, lParam);
	return res;
}

bool EventHandler::setMedian(UINT type, Median f) {
	auto &median_functions = EventHandler::medians;
	if(median_functions.count(type)) {
		median_functions[type] = f;
		return false;
	}
	median_functions.insert(std::pair(type, f));
	return true;
}
void EventHandler::addHandler(UINT type, Handler handler) {
	if(!this->handlers.count(type))
		this->handlers.insert(std::pair(type, std::set<Handler>()));
	this->handlers[type].insert(handler);
}
DrawingContext EventHandler::getDrawingContext() {
	return DrawingContext(this->hWnd);
}
LRESULT EventHandler::defaultPaintMedian(
	EventHandler *self, Handler handler, HWND hWnd, WPARAM, LPARAM
) {
	return handler(self, DrawingContext(hWnd));
}

BOOL EventHandler::triggerRepaint() {
	return InvalidateRect(this->hWnd, NULL, true);
}

LRESULT EventHandler::defaultDestroyHandler(HWND, WPARAM, LPARAM) {
	PostQuitMessage(0);
	return 0;
}

// DrawingContext

PAINTSTRUCT DrawingContext::ps{};
HBRUSH DrawingContext::hBrush{};
HPEN DrawingContext::hPen{};
HFONT DrawingContext::hFont{};

void DrawingContext::setSolidBrush(COLORREF color) {
	DrawingContext::hBrush = CreateSolidBrush(color);
	SelectObject(this->hdc, DrawingContext::hBrush);
}

void DrawingContext::setPen(int style, int width, COLORREF color) {
	DrawingContext::hPen = CreatePen(style, width, color);
	SelectObject(this->hdc, DrawingContext::hPen);
}

void DrawingContext::setFont(FontArg args) {
	DrawingContext::hFont = CreateFontW(
		args.height, args.width,
		args.escapement, args.orientation,
		args.weight,
		args.italic, args.underline, args.strikeout,
		args.charset,
		args.out_precision, args.clip_precision,
		args.quality,
		args.family,
		args.name
	);
	SelectObject(this->hdc, DrawingContext::hFont);
}

void DrawingContext::drawRect(RECT rect) {
	Rectangle(this->hdc,
		rect.left, rect.top,
		rect.right, rect.bottom
	);
}

void DrawingContext::drawText(
	LPCWSTR text, RECT rect, UINT format
) {
	DrawText(this->hdc, text, -1, &rect, format);
}

SIZE DrawingContext::measureText(LPCWSTR text, int count = -1) {
	SIZE res;
	GetTextExtentPoint32(this->hdc, text, count, &res);
	return res;
}

DrawingContext::DrawingContext(HWND &hWnd) :
	hWnd(hWnd),
	hdc(BeginPaint(hWnd, &ps)) {
 }

DrawingContext::~DrawingContext() {
	DeleteObject(DrawingContext::hBrush);
	DeleteObject(DrawingContext::hPen);
	DeleteObject(DrawingContext::hFont);
	EndPaint(hWnd, &ps);
}
