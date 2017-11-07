#include "JTime.h"


float JTime::unscaled_delta_time = 0;
float JTime::unscaled_time = 0;
float JTime::delta_time = 0;
float JTime::time = 0;
float JTime::time_scale = 1;
SimpleTimer JTime::timer;


float JTime::getUnscaledDeltaTime()
{
    return unscaled_delta_time;
}


float JTime::getUnscaledTime()
{
    return unscaled_time;
}


float JTime::getDeltaTime()
{
    return delta_time;
}


float JTime::getTime()
{
    return time;
}


float JTime::getTimeScale()
{
    return time_scale;
}


void JTime::setTimeScale(float _time_scale)
{
    time_scale = _time_scale;
}


void JTime::tick()
{
    unscaled_delta_time = timer.getTimeDifference();
    unscaled_time += JTime::unscaled_delta_time;

    delta_time = JTime::unscaled_delta_time * JTime::time_scale;
    time += JTime::delta_time;

    timer.reset();
}
