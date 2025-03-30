#include <gtest/gtest.h>
#include <ui/fps_counter.h>
#include <chrono>
#include <thread>

class FPSCounterTest : public ::testing::Test {
protected:
    FPSCounter fpsCounter;
};

TEST_F(FPSCounterTest, InitialValues) {
    EXPECT_EQ(fpsCounter.getCurrentFPS(), 0.0f);
    EXPECT_EQ(fpsCounter.getAverageFPS(), 0.0f);
    EXPECT_EQ(fpsCounter.getMinFPS(), 0.0f);
    EXPECT_EQ(fpsCounter.getMaxFPS(), 0.0f);
}

TEST_F(FPSCounterTest, UpdateFPS) {
    // Simulate 60 FPS (16.67ms per frame)
    for (int i = 0; i < 60; ++i) {
        fpsCounter.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(17)); // Slightly longer to account for processing time
    }

    // Check if FPS is close to 60
    float currentFPS = fpsCounter.getCurrentFPS();
    EXPECT_NEAR(currentFPS, 60.0f, 5.0f); // Allow 5 FPS margin of error

    // Check if average FPS is being calculated
    float avgFPS = fpsCounter.getAverageFPS();
    EXPECT_GT(avgFPS, 0.0f);
    EXPECT_NEAR(avgFPS, 60.0f, 5.0f);

    // Check if min/max FPS are being tracked
    float minFPS = fpsCounter.getMinFPS();
    float maxFPS = fpsCounter.getMaxFPS();
    EXPECT_GT(maxFPS, minFPS);
}

TEST_F(FPSCounterTest, FPSHistory) {
    // Simulate varying FPS
    for (int i = 0; i < 30; ++i) {
        fpsCounter.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(17)); // ~60 FPS
    }
    for (int i = 0; i < 30; ++i) {
        fpsCounter.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    }

    float minFPS = fpsCounter.getMinFPS();
    float maxFPS = fpsCounter.getMaxFPS();
    float avgFPS = fpsCounter.getAverageFPS();

    // Check if min/max FPS reflect the variation
    EXPECT_NEAR(minFPS, 30.0f, 5.0f);
    EXPECT_NEAR(maxFPS, 60.0f, 5.0f);
    EXPECT_NEAR(avgFPS, 45.0f, 5.0f);
} 