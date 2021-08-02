#include "TextureAtlas.h"

#include <fstream>
#include <iostream>

using namespace std;

TextureAtlas::TextureAtlas(std::string imagePath, std::string atlasPath) : map(), texture(imagePath) {
    string line;

    string name;

    int x, y, width, height;

    int c = 0;

    ifstream stream(atlasPath);
    while (getline(stream, line)) {
        switch (c++) {
        case 0:
            x = stoi(line);
            break;
        case 1:
            y = stoi(line);
            break;
        case 2:
            width = stoi(line);
            break;
        case 3:
            height = stoi(line);
            break;
        case 4:
            c = 0;
            map.insert(std::pair<std::string, TextureRegion>(line, { &texture, x, y, width, height }));
        }
    }
    stream.close();

}
