#pragma once

#include "simulator.hpp"

#include <wx/wx.h>
#include <optional>

using Simulation::ParticlesStateView;

struct ParticleVisual
{
    wxColour colour;
    int size;
};

wxDECLARE_EVENT(EVT_PARTICLE_SELECTED, wxCommandEvent);

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

    std::optional<size_t> selected_particle_ = std::nullopt;

    void check_invariants() const;
    
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnLeftClick(wxMouseEvent& event);
    
    void UpdateScaling();

    inline wxPoint sim_pos2canvas_point(Geom::Vec2 sim_pos)
    {   
        return wxPoint{
            static_cast<int>(sim_pos.x() * scale_x_),
            static_cast<int>(sim_pos.y() * scale_y_)
        };
    }
public:
    ParticleCanvas(wxWindow* parent, const ParticlesStateView& sim_particles, 
                   const std::vector<ParticleVisual> &vis_particles,
                   double field_size_x = 200.0, double field_size_y = 200.0);
    
    const std::optional<size_t> &selected_particle() const { return selected_particle_; }
    void unselect_particle() { selected_particle_.reset(); };
};
