#include "particle_canvas.hpp"

void ParticleCanvas::check_invariants() const
{
    assert(sim_particles_.size() == vis_particles_.size());
}

void ParticleCanvas::OnPaint(wxPaintEvent &event)
{
    check_invariants();

    wxPaintDC dc(this);
    //TODO - разобраться, почему создание двух частиц в одном месте приводит к их аннигиляции
    
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
}

void ParticleCanvas::OnSize(wxSizeEvent& event) 
{
    UpdateScaling();
    event.Skip();
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
}
