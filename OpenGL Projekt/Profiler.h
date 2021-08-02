#pragma once

#include "Utils.h"
#include "Color.h"
#include "ShapeRenderer.h"
#include <unordered_map>

class Category {
public:
	string name;
	Color color;
	long long startTime = currentTimeMicros();
	int result = 0;

	inline Category() : name(""), color(Color::WHITE) {

	}

	inline Category(string name, Color color) : name(name), color(color) {

	}

	inline void start() {
		startTime = currentTimeMicros();
	}

	inline void stop() {
		result = (int) (currentTimeMicros() - startTime);
	}

	inline int getResult() {
		return result;
	}
};

class Profiler {
	std::unordered_map<string, Category> a;
	
public:
	inline Profiler(vector<Category> l) {
		ASSERT(l.size());
		for(Category& c : l) {
			a.insert({ c.name, c });
		}
	}

	inline void start(string name) {
		a[name].start();
	}

	inline void stop(string name) {
		a[name].stop();
		//cout << "sopped with; " << a[name].getResult() << endl;
	}

	inline void drawResult(ShapeRenderer& sr, float x, float y, float radius) {
		float result = 0;
		for(pair<string, Category> p : a) {
			result += p.second.getResult();
		}
		if(result > 0) {
			int offset = 0;
			for(pair<string, Category> p : a) {
				if(p.second.getResult() > 0) {
					int idk = (int) round(p.second.getResult() / result * 360);
					sr.setColor(p.second.color);
					sr.circle(x, y, radius, 360, offset, offset + idk);
					offset += idk;
				}
			}
		}
		else {
			//cout << "nop" << endl;
		}
	}
};

#define PROFILE(p, name, x) p.start(name); x; p.stop(name);