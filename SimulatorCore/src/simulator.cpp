#include "simulator.hpp"

namespace Simulation
{

void Simulator::addParticle(const scalar_t &mass, const Vec2 &pos, const Vec2 &vel)
{
    particles_.add_particle(mass, pos, vel);
}

void Simulator::removeParticle(size_t index)
{
    particles_.remove_particle(index);
}

void Simulator::step(scalar_t dt)
{
    integrator_->integrate(particles_, *force_calc_, dt);
}

} // namespace Simulation