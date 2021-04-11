#pragma once

#include "config.h"
#include "animation.h"
#include <cctype>
#include <vector>
#include <map>
#include <string>

using namespace std;

static constexpr int fs = 8;

class Text : public Paintable {
	vector<pair<Texture *, int>> sprites;
public:
	static map<char, Layer *> textures;
	Text(char const *content, Layer &foreground) {
		char c;
		for(int i = 0, x = 0; c = content[i]; ++i, x += fs) {
			if(!isalnum(c))
				continue;
			if(isupper(c))
				c += 'a' - 'A';
			Layer *fg_copy = new Layer(foreground);
			Texture *sprite = new Texture(
				{ fs, fs }, { 0, 0 },
				fg_copy, Text::textures[c]
			);
			sprites.push_back(pair(sprite, x));
		}
	}
	~Text() {
		for(auto p : sprites) {
			Texture *s = p.first;
			delete s->foreground;
			delete s;
		}
	}
	void paintOn(HDC &hdc, Int2 position) const {
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
map<char, Layer *> Text::textures = {
	tx('a'), tx('b'), tx('c'), tx('d'), tx('e'), tx('f'), tx('g'),
	tx('h'), tx('i'), tx('j'), tx('k'), tx('l'), tx('m'), tx('n'),
	tx('o'), tx('p'), tx('q'), tx('r'), tx('s'), tx('t'), tx('u'),
	tx('v'), tx('w'), tx('x'), tx('y'), tx('z'), tx('0'), tx('1'),
	tx('2'), tx('3'), tx('4'), tx('5'), tx('6'), tx('7'), tx('8'),
	tx('9')
};
#undef tx
