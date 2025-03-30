#include "../../include/ui/fps_counter.h"
#include <GL/freeglut.h>
#include <sstream>
#include <algorithm>

FPSCounter::FPSCounter() 
    : currentFPS(0.0f)
    , averageFPS(0.0f)
    , minFPS(0.0f)
    , maxFPS(0.0f)
    , lastFrameTime(std::chrono::high_resolution_clock::now()) {
}

void FPSCounter::update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>
        (currentTime - lastFrameTime).count();
    
    currentFPS = 1.0f / deltaTime;
    fpsHistory.push_back(currentFPS);
    
    if (fpsHistory.size() > SAMPLE_SIZE) {
        fpsHistory.pop_front();
    }
    
    updateStats();
    lastFrameTime = currentTime;
}

void FPSCounter::updateStats() {
    if (fpsHistory.empty()) return;
    
    averageFPS = 0.0f;
    for (float fps : fpsHistory) {
        averageFPS += fps;
    }
    averageFPS /= fpsHistory.size();
    
    minFPS = *std::min_element(fpsHistory.begin(), fpsHistory.end());
    maxFPS = *std::max_element(fpsHistory.begin(), fpsHistory.end());
}

void FPSCounter::render() {
    // Save current matrix state
    glPushMatrix();
    glLoadIdentity();
    
    // Switch to orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    
    // Render background
    renderBackground();
    
    // Render FPS information
    std::ostringstream ss;
    ss << "FPS: " << static_cast<int>(currentFPS) << "\n"
       << "Avg: " << static_cast<int>(averageFPS) << "\n"
       << "Min: " << static_cast<int>(minFPS) << "\n"
       << "Max: " << static_cast<int>(maxFPS);
    
    renderText(ss.str(), -0.95f, 0.95f);
    
    // Restore matrix state
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void FPSCounter::renderBackground() {
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);  // Semi-transparent black
    glBegin(GL_QUADS);
    glVertex2f(-0.98f, 0.98f);
    glVertex2f(-0.85f, 0.98f);
    glVertex2f(-0.85f, 0.85f);
    glVertex2f(-0.98f, 0.85f);
    glEnd();
}

void FPSCounter::renderText(const std::string& text, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);  // White text
    glRasterPos2f(x, y);
    for (char c : text) {
        if (c == '\n') {
            y -= 0.03f;  // Reduced line spacing
            glRasterPos2f(x, y);
        } else {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);  // Smaller font
        }
    }
}

float FPSCounter::getCurrentFPS() const { return currentFPS; }
float FPSCounter::getAverageFPS() const { return averageFPS; }
float FPSCounter::getMinFPS() const { return minFPS; }
float FPSCounter::getMaxFPS() const { return maxFPS; } 