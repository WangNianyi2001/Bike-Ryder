#pragma once

#include "sprite.h"
#include <vector>
#include <map>
#include <algorithm>

namespace Game {
	using namespace std;
	class Scene {
	public:
		vector<Sprite *> sprites;
		using Handler = LRESULT (*)(HWND, WPARAM, LPARAM);
		map<UINT, Handler> handlers;
		void (*onEnter)();
		Scene() = default;
		Scene(initializer_list<Sprite *> const &sprites) {
			for(Sprite *sprite : sprites)
				this->sprites.push_back(sprite);
		}
		void addSprite(Sprite *sprite) {
			sprites.push_back(sprite);
		}
		void removeSprite(Sprite *sprite) {
			for(auto it = sprites.begin(); it != sprites.end(); ++it) {
				if(*it == sprite) {
					sprites.erase(it);
					return;
				}
			}
		}
		void sortSprites() {
			stable_sort(
				sprites.begin(), sprites.end(),
				[](Sprite *a, Sprite *b) { return a->z_index > b->z_index; }
			);
		}
		void addHandler(UINT type, Handler handler) {
			this->handlers.insert(pair(type, handler));
		}
		void paintOn(HDC &hdc) {
			for(Sprite *sprite : sprites) {
				sprite->paintOn(hdc);
			}
		}
	};
}