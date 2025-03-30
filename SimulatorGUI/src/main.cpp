#include "simulator.hpp"
#include "LJ_force_calc.hpp"
#include "RK4_integrator.hpp"

#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/tglbtn.h>
#include <vector>
#include <chrono>
#include <stdexcept>

using Simulation::Simulator;
using Simulation::ParticlesStateView;
using Simulation::scalar_t;

using Simulation::RungeKutta4Integrator;
using Simulation::LennardJonesForceCalc;

class ParticleCanvas : public wxPanel 
{    
private:
    const ParticlesStateView& particles_;
    double scale_x_ = 1.0;
    double scale_y_ = 1.0;

    const double field_size_x_;
    const double field_size_y_;
    
    void OnPaint(wxPaintEvent& event) 
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
    
    void OnSize(wxSizeEvent& event) 
    {
        UpdateScaling();
        event.Skip();
    }
    
    void UpdateScaling() 
    {
        wxSize size = GetClientSize();
        scale_x_ = size.GetWidth() / field_size_x_;
        scale_y_ = size.GetHeight() / field_size_y_;
    }
    
public:
    ParticleCanvas(wxWindow* parent, const ParticlesStateView& particles, 
                   double field_size_x = 200.0, double field_size_y = 200.0)
        : wxPanel(parent), particles_(particles), field_size_x_(field_size_x), field_size_y_(field_size_y)
    {
        if (field_size_x <= 0.0 || field_size_y_ <= 0)
            throw std::invalid_argument("Field size must be > 0.");
        SetDoubleBuffered(true);
        Bind(wxEVT_PAINT, &ParticleCanvas::OnPaint, this);
        Bind(wxEVT_SIZE, &ParticleCanvas::OnSize, this);
    }
};

class MainFrame : public wxFrame 
{
private:
    Simulator simulator{std::make_unique<RungeKutta4Integrator>(RungeKutta4Integrator{}),
                        std::make_unique<LennardJonesForceCalc>(LennardJonesForceCalc{1.0, 1.0})};
    ParticleCanvas* canvas;
    wxToggleButton* pauseButton;
    wxTimer timer{this, TIMER_ID};
    const int timer_period_ms = 16;
    bool paused = false;
    std::chrono::time_point<std::chrono::steady_clock> last_step;

    enum {TIMER_ID = 1000};

    void CreateControls() {
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        
        canvas = new ParticleCanvas(this, simulator.particles());
        sizer->Add(canvas, 1, wxEXPAND | wxALL, 5);
        
        pauseButton = new wxToggleButton(this, wxID_ANY, "Pause");
        pauseButton->Bind(wxEVT_TOGGLEBUTTON, &MainFrame::OnPause, this);
        sizer->Add(pauseButton, 0, wxALIGN_TOP | wxALL, 5);
        
        SetSizerAndFit(sizer);
        SetMinClientSize(wxSize(400, 300));
    }

    void OnTimer(wxTimerEvent& event) {
        if (!paused) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> dt = now - last_step;
            simulator.step(dt.count());
            last_step = now;
            canvas->Refresh();
        }
    }

    void OnPause(wxCommandEvent& event) {
        paused = event.IsChecked();
        if (paused) {
            timer.Stop();
        } else {
            timer.Start();
            last_step = std::chrono::steady_clock::now();
        }
    }

public:
    MainFrame() 
    : wxFrame(nullptr, wxID_ANY, "Physics Simulation")
    {
        CreateControls();

        Bind(wxEVT_TIMER, &MainFrame::OnTimer, this, TIMER_ID);
        timer.Start(timer_period_ms);

        last_step = std::chrono::steady_clock::now();
    }
};

class MyApp : public wxApp 
{
public:
    bool OnInit() override 
    {
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);