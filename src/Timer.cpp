#include "Timer.h"
#include <thread>

Timer::Timer(): _last(std::chrono::steady_clock::now())
{
}

double Timer::Mark()
{
    const std::chrono::time_point<std::chrono::steady_clock> old = this->_last;
    this->_last = std::chrono::steady_clock::now();
    const std::chrono::duration<double> useTime = this->_last - old;
    return useTime.count();
}

unsigned int Timer::now() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

std::chrono::duration<double> Timer::Peek() const
{
    const std::chrono::duration<double> peek(std::chrono::steady_clock::now() - this->_last);
    return peek;
}

std::chrono::duration<double> Timer::nextFps()
{
    std::chrono::time_point<std::chrono::steady_clock> end = this->_last + this->_fps;
    std::chrono::time_point<std::chrono::steady_clock> old = this->_last;
    std::this_thread::sleep_until(end);
    this->_last = end;
    return(end - old);
}

void Timer::setFps(unsigned short frame)
{
    this->_fps = std::chrono::round<std::chrono::steady_clock::duration>(std::chrono::duration<double>{1./frame});
}