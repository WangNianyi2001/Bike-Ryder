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
	NPC->z = 5;
	NPC->vz = 0.5;
	NPC->x = rand() % 200 - 100;
	NPCs.push_back(NPC);
}

PureColor background(RGB(255, 255, 255));

LRESULT paint(EventHandler *self, DrawingContext dc) {
	background.paintOn(vscreen.hdc, { 0, 0 });
	for(auto NPC : NPCs)
		NPC->render(vscreen.hdc);
	player.render(vscreen.hdc);
	projectOnto(dc.hdc);
	return 0;
}

bool accelerate = false, decelerate = false, move_left = false, move_right = false;

LRESULT timer(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if(accelerate) {
		player.vz += 0.01;
		if(player.vz > 1)
			player.vz = 1;
	}
	if(decelerate) {
		player.vz -= 0.01;
		if(player.vz < 0)
			player.vz = 0;
	}
	if(move_left) {
		player.x -= 5;
		if(player.x < -300)
			player.x = -300;
	}
	if(move_right) {
		player.x += 5;
		if(player.z > 300)
			player.z = 300;
	}
	player.updatePhysics();
	player.updateAnimation();
	for(auto NPC : NPCs) {
		NPC->z -= player.z - 1;
		NPC->updatePhysics();
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
	if(rand() % 100 <= 10 * player.vz)
		generateNPC();
	player.z = 1;
	self->markDirty();
	return 0;
}

LRESULT keyup(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch(wParam) {
	case 'W':
		accelerate = false;
		break;
	case 'S':
		decelerate = false;
		break;
	case 'A':
		move_left = false;
		break;
	case 'D':
		move_right = false;
		break;
	}
	return 0;
}

LRESULT keydown(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch(wParam) {
	case 'W':
		accelerate = true;
		break;
	case 'S':
		decelerate = true;
		break;
	case 'A':
		move_left = true;
		break;
	case 'D':
		move_right = true;
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
	event_handler.addHandler(WM_KEYUP, keyup);
	event_handler.addHandler(WM_DESTROY, EventHandler::defaultDestroyHandler);

	// Animations
	player.z = 1.0f;

	// Set the window to run
	int result = window->run();

	// Cleaning
	unloadImages();
	return result;
}