// FPSLimiter.hpp
#include <chrono>
#include <thread>

class FPSLimiter {
private:
    std::chrono::high_resolution_clock::time_point lastFrameTime;

public:
    void beginFrame() {
        lastFrameTime = std::chrono::high_resolution_clock::now();
    }

    void endFrame(int targetFPS) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto frameTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();

        if (frameTime < 1.0 / targetFPS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((1.0f / targetFPS - frameTime) * 1000)));
        }
    }
};
