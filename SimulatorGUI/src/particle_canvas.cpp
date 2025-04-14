#include "particle_canvas.hpp"


using Simulation::Vec2;


wxDEFINE_EVENT(EVT_PARTICLE_SELECTED, wxCommandEvent);



void ParticleCanvas::check_invariants() const
{
    assert(sim_particles_.size() == vis_particles_.size());
}

void ParticleCanvas::OnPaint(wxPaintEvent &event)
{
    check_invariants();

    wxPaintDC dc(this);
    
    for (size_t ind = 0; ind < sim_particles_.size(); ind++) 
    {
        auto pos = sim_particles_.pos(ind);
        wxPoint point(
            static_cast<int>(pos.x() * scale_x_),
            static_cast<int>(pos.y() * scale_y_)
        );

        ParticleVisual particle_vis = vis_particles_[ind];
        dc.SetBrush(wxBrush{particle_vis.colour});
        dc.DrawCircle(point, particle_vis.size);
    }

    if(selected_particle_.has_value()) 
    {
        const auto& pos = sim_particles_.pos(*selected_particle_);
        ParticleVisual particle_vis = vis_particles_[*selected_particle_];
        wxPoint point(
            static_cast<int>(pos.x() * scale_x_),
            static_cast<int>(pos.y() * scale_y_)
        );
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxRED_PEN);
        dc.DrawCircle(point, static_cast<int>((particle_vis.size + 1) * 1.5));
    }
}

void ParticleCanvas::OnSize(wxSizeEvent& event) 
{
    UpdateScaling();
    event.Skip();
}

void ParticleCanvas::OnLeftClick(wxMouseEvent &event)
{
    wxPoint click_pos_window = event.GetPosition();
    Vec2 click_pos_sim {
        click_pos_window.x / scale_x_,
        click_pos_window.y / scale_y_
    };
    
    double min_dist = std::numeric_limits<double>::max();
    std::optional<size_t> closest;
    
    const auto& positions = sim_particles_.positions();
    for(size_t ind = 0; ind < positions.size(); ++ind) 
    {
        const auto& cur_pos = positions[ind];
        double dx = cur_pos.x() - click_pos_sim.x();
        double dy = cur_pos.y() - click_pos_sim.y();
        double dist = std::sqrt(dx*dx + dy*dy);
        
        if (dist < min_dist) 
        {
            min_dist = dist;
            closest = ind;
        }
    }

    selected_particle_.reset();

    if (closest.has_value()) 
    {
        const double select_radius = vis_particles_[*closest].size * 1.5;
        if (min_dist < (select_radius / std::min(scale_x_, scale_y_)))
            selected_particle_ = *closest;
    }

    wxCommandEvent evt(EVT_PARTICLE_SELECTED, GetId());
    evt.SetEventObject(this);
    ProcessWindowEvent(evt);
    
    Refresh();
}

void ParticleCanvas::UpdateScaling() 
{
    wxSize size = GetClientSize();
    scale_x_ = size.GetWidth() / field_size_x_;
    scale_y_ = size.GetHeight() / field_size_y_;
}

ParticleCanvas::ParticleCanvas(wxWindow* parent, const ParticlesStateView& sim_particles, 
                               const std::vector<ParticleVisual> &vis_particles,
                               double field_size_x, double field_size_y)
    : wxPanel(parent), sim_particles_(sim_particles), vis_particles_(vis_particles),
      field_size_x_(field_size_x), field_size_y_(field_size_y)
{
    if (field_size_x <= 0 || field_size_y_ <= 0)
        throw std::invalid_argument("Field size must be > 0.");

    if (sim_particles_.size() != vis_particles_.size())
        throw std::invalid_argument("sim_particles and vis_particles must be of same size and coherent");

    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &ParticleCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &ParticleCanvas::OnSize, this);
    Bind(wxEVT_LEFT_DOWN, &ParticleCanvas::OnLeftClick, this);
}
