#include "SwarmAgent.h"
#include "GameData.h"
#include "JTime.h"


SwarmAgent::SwarmAgent(AgentInstanceData& _data, AgentSettings& _settings)
    : instance_data(_data)
    , settings(_settings)
    , velocity(0, 0, 0)
    , acceleration(0, 0, 0)
    , current_tile_index(-1)
{
    setColor(settings.color);
}


void SwarmAgent::tick(const float _dt)
{
    move(_dt);

    // Reset for next tick.
    acceleration.x = 0;
    acceleration.y = 0;
    acceleration.z = 0;
}


void SwarmAgent::steerFromNeighbourAgents(const std::vector<SwarmAgent*>& _neighbours)
{
    DirectX::XMFLOAT3 steer { 0, 0, 0 };

    for (auto* neighbour : _neighbours)
    {
        // Don't try to steer away from self.
        if (neighbour == this)
            continue;

        auto& neighbour_pos = neighbour->getPos();
        float distance_sqr = DirectX::Float3DistanceSquared(instance_data.pos, neighbour_pos);

        if (distance_sqr > settings.agent_separation_sqr)
            continue;

        if (distance_sqr == 0)
        {
            // Prevent agents occupying the same space.
            float rand_x = static_cast<float>(rand() % 1) - 0.5f;
            float rand_y = static_cast<float>(rand() % 1) - 0.5f;

            steer = DirectX::XMFLOAT3(rand_x, rand_y, 0);
        }
        else
        {
            DirectX::XMFLOAT3 diff = DirectX::Float3SubtractBfromA(instance_data.pos, neighbour_pos);
            diff = DirectX::Float3Normalized(diff);
            diff = DirectX::Float3Div(diff, distance_sqr);

            steer = DirectX::Float3Add(steer, diff);
        }
    }

    applySteer(steer);
}


const DirectX::XMFLOAT3& SwarmAgent::getPos() const
{
    return instance_data.pos;
}


void SwarmAgent::setPos(const DirectX::XMFLOAT3& _pos)
{
    instance_data.pos = _pos;
}


void SwarmAgent::setPos(const float _x, const float _y, const float _z)
{
    setPos(DirectX::XMFLOAT3(_x, _y, _z));
}


const DirectX::XMFLOAT4& SwarmAgent::getCol() const
{
    return instance_data.color;
}


void SwarmAgent::setColor(const DirectX::XMFLOAT4& _color)
{
    instance_data.color = _color;
}


void SwarmAgent::setColor(const float _r, const float _g, const float _b, const float _a)
{
    setColor(DirectX::XMFLOAT4(_r, _g, _b, _a));
}


int SwarmAgent::getCurrentTileIndex() const
{
    return current_tile_index;
}


void SwarmAgent::setCurrentTileIndex(const int _tile_index)
{
    if (current_tile_index != _tile_index)
    {
        for (auto& listener : listeners)
        {
            listener->onTileIndexChanged(this, current_tile_index, _tile_index);
        }
    }

    current_tile_index = _tile_index;
}


void SwarmAgent::applySteer(const DirectX::XMFLOAT3& _force)
{
    DirectX::XMFLOAT3 desired = DirectX::Float3Mul(_force, settings.agent_speed);
    DirectX::XMFLOAT3 change = DirectX::Float3SubtractBfromA(desired, velocity);
    change = DirectX::Float3Mul(change, settings.agent_steer / settings.agent_speed);

    acceleration = DirectX::Float3Add(acceleration, change);
}


void SwarmAgent::move(const float _dt)
{
    velocity = DirectX::Float3Add(velocity, acceleration);

    // Limit speed.
    if (DirectX::Float3MagnitudeSquared(velocity) > settings.agent_speed_sqr)
    {
        velocity = DirectX::Float3Normalized(velocity);
        velocity = DirectX::Float3Mul(velocity, settings.agent_speed);
    }

    DirectX::XMFLOAT3 step = DirectX::Float3Mul(velocity, _dt);
    instance_data.pos = DirectX::Float3Add(instance_data.pos, step);
}
