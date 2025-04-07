#pragma once

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
    
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void UpdateScaling();
public:
    ParticleCanvas(wxWindow* parent, const ParticlesStateView& particles, 
                   double field_size_x = 200.0, double field_size_y = 200.0);
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

    void CreateControls();
    void OnTimer(wxTimerEvent& event);
    void OnPause(wxCommandEvent& event);
public:
    MainFrame();
};
