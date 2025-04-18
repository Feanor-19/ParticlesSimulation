#include "sim_gui_wrapper.hpp"

using namespace SimGUI;

void SimGUI::SimGUIWrapper::sim_step(scalar_t dt)
{
    simulator_.step(dt);
}

void SimGUIWrapper::push_back_particle(const Particle &sim_part, const ParticleVisual &vis_part)
{
    simulator_.push_back_particle(sim_part);
    vis_particles_.push_back(vis_part);

    assert(simulator_.particles().size() == vis_particles_.size());
}

void SimGUIWrapper::remove_particle(size_t index)
{
    assert(index < vis_particles_.size() && index < simulator_.particles().size());

    simulator_.remove_particle(index);
    vis_particles_.erase(vis_particles_.begin() + index);
}
