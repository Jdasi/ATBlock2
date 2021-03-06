#pragma once

#include "SimpleTimer.h"

/* Static class that measures common useful time-related information.
 * The tick function assumes it is being called only once per frame.
 */
class JTime
{
public:
    static float getUnscaledDeltaTime();
    static float getUnscaledTime();

    static float getDeltaTime();
    static float getTime();

    static float getTimeScale();
    static void setTimeScale(float _time_scale);

    static void tick();

private:
    static float unscaled_delta_time;
    static float unscaled_time;

    static float delta_time;
    static float time;

    static float time_scale;

    static SimpleTimer timer;

};
