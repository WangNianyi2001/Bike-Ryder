#pragma once

#include "sprite.h"
#include <cctype>
#include <vector>
#include <map>

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
				{ 1, 1 },
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

map<char, Texture *> Text::textures = {
	{ 'a', new Bitmap(L"../font/a.bmp") },
	{ 'b', new Bitmap(L"../font/b.bmp") },
	{ 'c', new Bitmap(L"../font/c.bmp") },
	{ 'd', new Bitmap(L"../font/d.bmp") },
	{ 'e', new Bitmap(L"../font/e.bmp") },
	{ 'f', new Bitmap(L"../font/f.bmp") },
	{ 'g', new Bitmap(L"../font/g.bmp") },
	{ 'h', new Bitmap(L"../font/h.bmp") },
	{ 'i', new Bitmap(L"../font/i.bmp") },
	{ 'j', new Bitmap(L"../font/j.bmp") },
	{ 'k', new Bitmap(L"../font/k.bmp") },
	{ 'l', new Bitmap(L"../font/l.bmp") },
	{ 'm', new Bitmap(L"../font/m.bmp") },
	{ 'n', new Bitmap(L"../font/n.bmp") },
	{ 'o', new Bitmap(L"../font/o.bmp") },
	{ 'p', new Bitmap(L"../font/p.bmp") },
	{ 'q', new Bitmap(L"../font/q.bmp") },
	{ 'r', new Bitmap(L"../font/r.bmp") },
	{ 's', new Bitmap(L"../font/s.bmp") },
	{ 't', new Bitmap(L"../font/t.bmp") },
	{ 'u', new Bitmap(L"../font/u.bmp") },
	{ 'v', new Bitmap(L"../font/v.bmp") },
	{ 'w', new Bitmap(L"../font/w.bmp") },
	{ 'x', new Bitmap(L"../font/x.bmp") },
	{ 'y', new Bitmap(L"../font/y.bmp") },
	{ 'z', new Bitmap(L"../font/z.bmp") }
};