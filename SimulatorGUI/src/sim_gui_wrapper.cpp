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

void SimGUI::SimGUIWrapper::clear_particles()
{
    simulator_.clear_particles();
    vis_particles_.clear();
}

void SimGUI::SimGUIWrapper::save_particles(const std::string &save_name)
{
    db_particles_.save_state(save_name, simulator_.particles(), vis_particles_);
}

void SimGUI::SimGUIWrapper::load_particles(const std::string &save_name)
{
    Simulation::ParticlesState tmp_sim;
    std::vector<ParticleVisual> tmp_vis;

    db_particles_.load_state(save_name, tmp_sim, tmp_vis);

    simulator_.load_particles(std::move(tmp_sim));
    vis_particles_ = std::move(tmp_vis);
}

void SimGUI::SimGUIWrapper::delete_save(const std::string &save_name)
{
    db_particles_.delete_save(save_name);
}

std::vector<std::string> SimGUI::SimGUIWrapper::get_save_names()
{
    return db_particles_.get_saves();
}
