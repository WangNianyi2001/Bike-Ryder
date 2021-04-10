#include "game.h"
#include "resources.h"

using namespace SimpleWin32;

Window *window;
EventHandler event_handler;
PureColor vscreen(RGB(255, 255, 255), { vwidth, vheight });

void projectOnto(HDC hdc) {
	vscreen.paintOn(
		hdc,
		{ 0, 0 },
		{ vwidth * pixel_scale, vheight * pixel_scale },
		SRCCOPY
	);
}

Character player(self_ride, self_kick, self_fall);
vector<Character *> NPCs;
void generateNPC() {
	Character *NPC = new Character(self_ride, self_kick, self_fall);
	NPC->z = 10;
	NPC->x = rand() % 300 - 150;
	NPCs.push_back(NPC);
}

PureColor background(RGB(255, 255, 255));

LRESULT paint(EventHandler *self, DrawingContext dc) {
	background.paintOn(vscreen.hdc, { 0, 0 });
	player.render(vscreen.hdc);
	for(auto NPC : NPCs)
		NPC->render(vscreen.hdc);
	projectOnto(dc.hdc);
	return 0;
}

LRESULT timer(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	player.updateAnimation();
	player.updatePhysics();
	for(auto NPC : NPCs) {
		NPC->z -= player.z - 1;
		NPC->updateAnimation();
	}
	while(true) {
		auto it = find_if(
			NPCs.begin(), NPCs.end(),
			[&](Character *NPC) { return NPC->z <= 0 || NPC->z >= 100; }
		);
		if(it == NPCs.end())
			break;
		NPCs.erase(it);
	}
	if(rand() % 100 <= 10)
		generateNPC();
	player.z = 1;
	self->markDirty();
	return 0;
}

LRESULT keydown(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch(wParam) {
	case 'W':
		player.vz += 0.05;
		if(player.vz > 1)
			player.vz = 1;
		break;
	case 'S':
		player.vz -= 0.05;
		if(player.vz < 0)
			player.vz = 0;
		break;
	case 'Q':
		player.kick(1);
		break;
	case 'E':
		player.kick(-1);
		break;
	case VK_SPACE:
		player.ride();
		break;
	}
	return 0;
}

LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
	return event_handler(hWnd, type, wParam, lParam);
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	// Initializing environment
	srand((unsigned)GetTickCount64());

	// Setting up the window
	window = new Window(hInstance, Window::InitArg{
		.title = L"µÅ³µ Bike Ryder",
		.width = vwidth * pixel_scale,
		.height = vheight * pixel_scale,
		.style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
		.event_processor = eventProcessor,
	});
	SetTimer(window->window, 0, 1000 / fps, NULL);
	event_handler.setMedian(WM_PAINT, EventHandler::defaultPaintMedian);
	event_handler.addHandler(WM_PAINT, paint);
	event_handler.addHandler(WM_TIMER, timer);
	event_handler.addHandler(WM_KEYDOWN, keydown);
	event_handler.addHandler(WM_DESTROY, EventHandler::defaultDestroyHandler);

	// Animations
	player.z = 1.0f;

	// Set the window to run
	int result = window->run();

	// Cleaning
	unloadImages();
	return result;
}