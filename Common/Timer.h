#include <boost/timer.hpp>

class CountDown
{
public:
    CountDown() {}

    void restart() { _start_time = std::clock(); }

    double elapsed() const 
    {
        return  double(std::clock() - _start_time) / CLOCKS_PER_SEC;
    }

private:
    std::clock_t _start_time;
};