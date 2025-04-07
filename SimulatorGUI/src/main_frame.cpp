#include "main_frame.hpp"

void ParticleCanvas::OnPaint(wxPaintEvent& event) 
{
    wxPaintDC dc(this);
    dc.SetBrush(*wxBLUE_BRUSH);
    
    for (const auto& pos : particles_.positions()) 
    {
        wxPoint point(
            static_cast<int>(pos.x() * scale_x_),
            static_cast<int>(pos.y() * scale_y_)
        );
        dc.DrawCircle(point, 3);
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

ParticleCanvas::ParticleCanvas(wxWindow* parent, const ParticlesStateView& particles, 
                               double field_size_x, double field_size_y)
    : wxPanel(parent), particles_(particles), 
      field_size_x_(field_size_x), field_size_y_(field_size_y)
{
    if (field_size_x <= 0 || field_size_y_ <= 0)
        throw std::invalid_argument("Field size must be > 0.");
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &ParticleCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &ParticleCanvas::OnSize, this);
}

void MainFrame::CreateControls() 
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    
    canvas = new ParticleCanvas(this, simulator.particles());
    sizer->Add(canvas, 1, wxEXPAND | wxALL, 5);
    
    pauseButton = new wxToggleButton(this, wxID_ANY, "Pause");
    pauseButton->Bind(wxEVT_TOGGLEBUTTON, &MainFrame::OnPause, this);
    sizer->Add(pauseButton, 0, wxALIGN_TOP | wxALL, 5);
    
    SetSizerAndFit(sizer);
    SetMinClientSize(wxSize(400, 300));
}

void MainFrame::OnTimer(wxTimerEvent& event) 
{
    if (!paused) 
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> dt = now - last_step;
        simulator.step(dt.count());
        canvas->Refresh();
        last_step = now;
    }
}

void MainFrame::OnPause(wxCommandEvent& event) 
{
    paused = event.IsChecked();
    if (paused) {
        timer.Stop();
    } else {
        timer.Start();
        last_step = std::chrono::steady_clock::now();
    }
}

MainFrame::MainFrame() 
    : wxFrame(nullptr, wxID_ANY, "Physics Simulation")
{
    CreateControls();

    // TODO
    simulator.add_particle({1,1,{100,100},{10,10}});

    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this, TIMER_ID);
    timer.Start(timer_period_ms);

    last_step = std::chrono::steady_clock::now();
}