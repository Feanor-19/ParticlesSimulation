#include "simulator.hpp"

namespace Simulation
{

void Simulator::push_back_particle(Particle particle)
{
    particles_.push_back_particle(particle);
}

void Simulator::remove_particle(size_t index)
{
    particles_.remove_particle(index);
}

void Simulator::step(scalar_t dt)
{
    integrator_->integrate(particles_, *force_calc_, dt);
}

void Simulator::load_particles(ParticlesState&& new_particles)
{
    particles_ = std::move(new_particles);
}

void Simulator::clear_particles()
{
    particles_.clear();
}

} // namespace Simulation