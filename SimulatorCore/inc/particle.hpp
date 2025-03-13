#pragma once

#include "utilities.hpp"

namespace Simulation
{

using Geom::Vec2;
using Geom::scalar_t;

class Particle 
{
private:
    scalar_t mass_;
    Vec2 pos_;
    Vec2 vel_;
public:
    Particle(double mass, Vec2 pos, Vec2 vel) 
        : mass_(mass), pos_(pos), vel_(vel) {};
    
    Vec2 pos() const {return pos_;};
    Vec2 vel() const {return vel_;};
    scalar_t mass() const {return mass_;};
    
    void setPosition(Vec2 pos) {pos_ = pos;};
    void setVelocity(Vec2 vel) {vel_ = vel;};
};

} // namespace Simulation