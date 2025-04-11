#pragma once

#include "simulator.hpp"

#include <wx/wx.h>

using Simulation::ParticlesStateView;

struct ParticleVisual
{
    wxColour colour;
    int size;
};

class ParticleCanvas : public wxPanel 
{    
private:
    const ParticlesStateView& sim_particles_;

    // corresponding visual properties for each particle from "sim_particles_"
    const std::vector<ParticleVisual> &vis_particles_;

    double scale_x_ = 1.0;
    double scale_y_ = 1.0;

    const double field_size_x_;
    const double field_size_y_;

    void check_invariants() const;
    
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    
    void UpdateScaling();
public:
    ParticleCanvas(wxWindow* parent, const ParticlesStateView& sim_particles, 
                   const std::vector<ParticleVisual> &vis_particles,
                   double field_size_x = 200.0, double field_size_y = 200.0);
};
