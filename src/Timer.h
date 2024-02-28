#pragma once
#include <chrono>

class Timer
{
public:
    Timer();
    Timer(const Timer& timer) = delete;
    Timer& operator=(const Timer& timer) = delete;
    double Mark();
    unsigned int now() const;
    std::chrono::duration<double> Peek() const;
    std::chrono::duration<double> nextFps();
    void setFps(unsigned short frame);
    ~Timer() = default;
private:
    std::chrono::time_point<std::chrono::steady_clock> _last;
    std::chrono::duration<long long, std::ratio<1, 1000000000>> _fps;
};
