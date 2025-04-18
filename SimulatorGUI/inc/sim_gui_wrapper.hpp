#pragma once

#include "simulator.hpp"
#include "sim_impl_manager.hpp"

#include <wx/wx.h>

namespace SimGUI
{

using Simulation::Simulator;
using Simulation::ParticlesStateView;
using Simulation::scalar_t;
using Simulation::Particle;

using Simulation::IntegratorPtr;
using Simulation::ForceCalcPtr;

struct ParticleVisual
{
    wxColour colour;
    int size;
};

class SimGUIWrapper
{
private:
    Simulator simulator_;
    SimImplManager::SimImplManager sim_impl_manager_;

    std::vector<ParticleVisual> vis_particles_;

    // здесь будет БД
public:
    SimGUIWrapper() : sim_impl_manager_(simulator_) {};
    
    SimImplManager::SimImplManager &sim_impl_manager() { return sim_impl_manager_; }

    const std::vector<ParticleVisual> &vis_particles() const { return vis_particles_; }
    const ParticlesStateView& sim_particles() const {return simulator_.particles();}

    const IntegratorPtr& integrator() const { return simulator_.integrator(); }
    const ForceCalcPtr&  force_calc() const { return simulator_.force_calc(); }

    void sim_step(scalar_t dt);
    
    void push_back_particle(const Particle &sim_part, const ParticleVisual &vis_part);
    void remove_particle(size_t index);
};

} // namespace SimGUI