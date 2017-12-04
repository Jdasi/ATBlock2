#pragma once

#ifdef DEBUG
    const int MAX_AGENTS = 5000;
    const int AGENTS_PER_SPAWN = 10;
#elif RELEASE
    const int MAX_AGENTS = 100000;
    const int AGENTS_PER_SPAWN = 100;
#endif

const float DESIRED_SEPARATION = 1;
const float DESIRED_SEPARATION_SQR = DESIRED_SEPARATION * DESIRED_SEPARATION;
