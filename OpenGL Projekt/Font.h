#pragma once

#include "Utils.h"
#include "Texture.h"
#include "SpriteBatch.h"

class Font {
	Texture texture;
	TextureRegion re[16][16];
	float scl = 1;
	float height = 0;

public:

	inline float getHeight() {
		return height * scl;
	}

	inline void setScale(float scl) {
		this->scl = scl;
	}

	inline void setHeight(float height) {
		scl = height / this->height;
	}

	static Font defaultFont;

	string idk(string& s, string&& t) {
		for(int i = 0; i < t.length(); i++) {
			if(s[i] != t[i]) {
				return "";
			}
		}
		return s.substr(t.length());
	}

	Font(string path, string dataPath) : texture(path) {
		int cellSize = 0;
		int wt[256] {};
		for(string& s : split(read_file(dataPath), '\n')) {
			string r;
			r = idk(s, "Cell Width,");
			if(r.length()) {
				cellSize = std::stoi(r);
				continue;
			}
			r = idk(s, "Font Height,");
			if(r.length()) {
				height = (float) std::stoi(r);
				continue;
			}
			vector<string> sr = split(s, ' ');
			if(sr.size() == 4) {
				if(sr[0].compare("Char") == 0 && sr[2].compare("Base") == 0) {
					wt[std::stoi(sr[1])] = std::stoi(idk(sr[3], "Width,"));
				}
			}
		}
		for(int x = 0; x < 16; x++) {
			for(int y = 0; y < 16; y++) {
				int i = (y + 2) * 16 + x;
				int w = i < _countof(wt) ? wt[i] : cellSize;
				re[y][x] = { &texture, cellSize * x, texture.getHeight() - cellSize * (y + 1) + cellSize - (int) height, wt[i], (int) height
			};
			}
		}
		
	}
	
	inline TextureRegion& get(int i) {
		return re[i / 16 - 2][i % 16];
	}

	inline TextureRegion& operator[](int i) {
		return get(i);
	}
	
	void print(SpriteBatch& batch, string s, Vector2G pos) {
		texture.bind();
		for(char c : s) {
			TextureRegion& tr = get(c);
			Vector2G size = { tr.getWidth(), tr.getHeight() };
			size *= scl;
			batch.drawTexture(tr, pos.x, pos.y, size.x, size.y);
			pos.x += size.x;
		}
	}
};
