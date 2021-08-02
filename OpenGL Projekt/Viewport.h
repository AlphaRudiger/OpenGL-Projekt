#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utility>

#include "Utils.h"

Player* p;

static float tagetWidth, targetHeight;
float currentWidth, currentHeight;

enum class ViewportType {
    Stretch, Fit
};

ViewportType currentViewport = ViewportType::Stretch;



namespace Viewport {
    Vector3G finalSize() {
        float sx = currentWidth / tagetWidth;
        float sy = currentHeight / targetHeight;

        float scl = std::min(sx, sy);

        float fx = tagetWidth * scl;
        float fy = targetHeight * scl;

        return { fx, fy, scl };
    }

    Vector2G transform(float x, float y) {
        Vector3G f = Viewport::finalSize();
        return { x / f.z, y / f.z };
    }
}


void updateCurrentViewport() {
    if (currentViewport == ViewportType::Fit) {
        Vector3G f = Viewport::finalSize();
        
        glViewport((int) (currentWidth - f.x) / 2, (int) (currentHeight - f.y) / 2, (int) f.x, (int) f.y);
    }
    if (currentViewport == ViewportType::Stretch) {
        glViewport(0, 0, (int) currentWidth, (int) currentHeight);
    }
}

void fitViewport(int width, int height) {
    tagetWidth = (float) width;
    targetHeight = (float) height;
    currentViewport = ViewportType::Fit;
    updateCurrentViewport();
}

void stretchViewport() {
    currentViewport = ViewportType::Stretch;
    updateCurrentViewport();
}

inline void window_size_callback(GLFWwindow* window, int width, int height) {
    currentWidth = (float) width;
    currentHeight = (float) height;
    p->cam.width = (float)width;
    p->cam.height = (float)height;
    updateCurrentViewport();
}

