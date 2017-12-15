#pragma once

#ifdef DEBUG
    const int MAX_AGENTS = 5000;
    const int AGENTS_PER_SPAWN = 10;
#elif RELEASE
    const int MAX_AGENTS = 50000;
    const int AGENTS_PER_SPAWN = 10;
#endif

const float AGENT_MAX_SPEED = 25;
const float AGENT_MAX_STEER = 1;

const float AGENT_MAX_SPEED_SQR = AGENT_MAX_SPEED * AGENT_MAX_SPEED;
const float AGENT_MAX_STEER_SQR = AGENT_MAX_STEER * AGENT_MAX_STEER;

const float AGENT_DESIRED_SEPARATION = 1;
const float AGENT_DESIRED_SEPARATION_SQR = AGENT_DESIRED_SEPARATION * AGENT_DESIRED_SEPARATION;
