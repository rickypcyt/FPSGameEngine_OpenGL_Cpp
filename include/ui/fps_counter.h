#pragma once

#include <string>
#include <chrono>
#include <deque>
#include <glm/glm.hpp>

class FPSCounter {
public:
    FPSCounter();
    void update();
    void render();
    float getCurrentFPS() const;
    float getAverageFPS() const;
    float getMinFPS() const;
    float getMaxFPS() const;

private:
    static const int SAMPLE_SIZE = 60;  // Number of frames to average
    std::deque<float> fpsHistory;
    float currentFPS;
    float averageFPS;
    float minFPS;
    float maxFPS;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    
    void updateStats();
    void renderBackground();
    void renderText(const std::string& text, float x, float y);
}; 