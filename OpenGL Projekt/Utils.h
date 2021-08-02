#pragma once

#include <stdint.h>

#include <chrono>
#include <iostream>

#include <vec2.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include <fstream>

#include <algorithm>

#include <vector>
#include <unordered_map>
#include <sstream>

#include "Settings.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef DEBUG
	#define GLCall(x)	GLClearError();\
						x;\
						ASSERT(!(GLLogCall(#x, __FILE__, __LINE__)));
#else
	#define GLCall(x) x;
#endif

#define VecCS(v) v.x, v.y, v.z

#define PrintVar(x) std::cout << #x << ": " << x << std::endl;

#define synchronized(m) for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())

#define roundz(x,d) ((floor(((x)*pow(10,d))+.5))/pow(10,d))

#define TestSpeedIf(x, m) {\
							auto time = currentTimeMicros();\
							x;\
							time = currentTimeMicros() - time;\
							if(time >= m) std::cout << #x << ": " << roundz(time / 1000.0f, 2) << "ms" << std::endl;\
						}

#define TestSpeed(x) TestSpeedIf(x, 0);

typedef glm::vec2 Vector2G;
typedef glm::vec3 Vector3G;
typedef glm::vec<3, int, glm::defaultp> Vector3IG;
typedef glm::mat4 Matrix4G;

typedef uint8_t u8;

template<typename... T>
void mD3FOR(void (*f)(Vector3G), T... t) {
	float a[] = { (t, ...) };
	for(int i = 0; i < sizeof...(t) / 3; i++) {
		f(Vector3G(a[i], a[i + 1], a[i + 2]));
	}
}

#define D3FOR(v, f, a) mD3FOR([](Vector3G v) { f }, a);

//inline void loopAround(int sx, int sy, int sz, void (*lfunc)(int, int, int)) {
//	for (int ix = sx - 1; ix < sx + 2; ix += 2) {
//		for (int iy = sy - 1; iy < sy + 2; iy += 2) {
//			for (int iz = sz - 1; iz < sz + 2; iz += 2) {
//				lfunc(ix, iy, iz);
//			}
//		}
//	}
//}

using std::min;
using std::max;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::ifstream;
using std::vector;
using std::unordered_map;
using std::pair;

inline string read_file(const string& filepath) {
	using std::ios;
	ifstream ifs(filepath, ios::binary | ios::ate);
	ASSERT(ifs.is_open());
	int len = (int) ifs.tellg();
	ifs.seekg(0);
	string str(len, ' ');
	ifs.read(&str[0], len);
	return std::move(str);
}

inline vector<string> split(const string& s, char delim) {
	vector<string> result;
	std::stringstream ss(s);
	string item;

	while(std::getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

inline void GLClearError() {
	//while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line) {
	bool erroro = false;
	while (GLenum error = glGetError()) {
		erroro = true;
		std::cout << "GL Error(" << error << ") in: " << file << "; Line: " << line << std::endl;
		return erroro;
	}
	return erroro;
}

inline long long currentTimeMilis() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

inline long long currentTimeMicros() {
	using namespace std::chrono;
	return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

inline int getRealBlockNumber(float n) {
	int nn = (int)n;
	return nn - (nn > n ? 1 : 0);
}

inline int getChunkNumber(int n) {
	bool ne = n < 0;
	if (ne) {
		n++;

	}
	return n / ChunkSize - (ne ? 1 : 0);
}
inline int getChunkNumber(float n) {
	return getChunkNumber((int) n);
}

template<typename T>
inline int mapN(T t) {
	return t < 0 ? -1 : 1;
}


template<typename... Args>
struct Data {
	int data[2];
public:
	Data(Args... args) {
		int temp[] = { args... };
		for(int i = 0; i < _countof(temp); i++) {
			data[i] = temp[i];
		}
	}
};

template<typename T, typename F, typename... Args>
inline auto forEach(F f, std::vector<T>& data, Args... args) {
	using std::invoke_result_t;
	using std::is_same_v;
	using returnType = invoke_result_t<F, T, Args...>;
	for(int i = 0; i < data.size(); i++) {
		if constexpr(is_same_v <returnType, void>) {
			f(data[i], args...);
		}
		else {
			returnType r = f(data.data[i], args...);
			if(r) {
				return r;
			}
		}
	}
}
template<typename T, typename F, typename... Args>
inline auto forEach(F f, std::vector<T>&& data, Args... args) {
	return forEach(f, data, args...);
}

inline int dist(Vector3IG block, Vector3IG chunk) {
	int cbx = getChunkNumber(block.x);
	int cby = getChunkNumber(block.y);
	int cbz = getChunkNumber(block.z);
	if(cbx == chunk.x && cby == chunk.y && cbz == chunk.z) return 0;
	int cx = chunk.x * ChunkSize;
	int cy = chunk.y * ChunkSize;
	int cz = chunk.z * ChunkSize;

	if(cx < block.x) {
		cx += ChunkSize - 1;
		if(cx > block.x) cx = block.x;
	}
	if(cy < block.y) {
		cy += ChunkSize - 1;
		if(cy > block.y) cy = block.y;
	}
	if(cz < block.z) {
		cz += ChunkSize - 1;
		if(cz > block.z) cz = block.z;
	}

	return abs(cx - block.x) + abs(cy - block.y) + abs(cz - block.z);
}

//using namespace std;

//#define new (cout << "Alloc: file: " << __FILE__ << " line: " << __LINE__ << endl;) new 
