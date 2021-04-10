#pragma once

#include "config.h"
#include "animation.h"
#include <cctype>
#include <vector>
#include <map>
#include <string>

using namespace std;

static constexpr int fs = 8;

class Text {
	vector<pair<Frame *, int>> sprites;
public:
	static map<char, Texture *> textures;
	Text(char const *content, Texture &foreground) {
		char c;
		for(int i = 0, x = 0; c = content[i]; ++i, x += fs) {
			if(!isalpha(c))
				continue;
			if(isupper(c))
				c += 'a' - 'A';
			Texture *fg_copy = new Texture(foreground);
			Frame *sprite = new Frame(
				{ fs, fs }, { 0, 0 },
				fg_copy, Text::textures[c]
			);
			sprites.push_back(pair(sprite, x));
		}
	}
	~Text() {
		for(auto p : sprites) {
			Frame *s = p.first;
			delete s->foreground;
			delete s;
		}
	}
	void paintOn(HDC hdc, Int2 position) {
		for(auto p : sprites) {
			auto sprite = p.first;
			auto x = p.second;
			Int2 current = position;
			current.x += x;
			sprite->paintOn(hdc, current);
		}
	}
};

wstring makeFontDir(char c) {
	wchar_t buf[2] = { (wchar_t)c, 0 };
	wstring res = font_root;
	res.replace(res.find(L'$'), 1, buf);
	return res;
}

#define tx(c) { c, new Bitmap(makeFontDir(c).c_str()) }
map<char, Texture *> Text::textures = {
	tx('a'), tx('b'), tx('c'), tx('d'), tx('e'), tx('f'), tx('g'),
	tx('h'), tx('i'), tx('j'), tx('k'), tx('l'), tx('m'), tx('n'),
	tx('o'), tx('p'), tx('q'), tx('r'), tx('s'), tx('t'), tx('u'),
	tx('v'), tx('w'), tx('x'), tx('y'), tx('z')
};
#undef tx
