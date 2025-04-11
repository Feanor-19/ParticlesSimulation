#pragma once

#include "common.hpp"

#include "simulator.hpp"
#include "LJ_force_calc.hpp"
#include "RK4_integrator.hpp"

#include "particle_templates.hpp"

#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/tglbtn.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
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
    Simulator simulator_{std::make_unique<RungeKutta4Integrator>(RungeKutta4Integrator{}),
                        std::make_unique<LennardJonesForceCalc>(LennardJonesForceCalc{1.0, 1.0})};
    
    wxPanel* panel_;
    ParticleCanvas* canvas_;
    wxToggleButton* btn_pause_;

    wxComboBox* combo_type_;
    wxTextCtrl* ctrl_pos_x_;
    wxTextCtrl* ctrl_pos_y_;
    wxTextCtrl* ctrl_vel_x_;
    wxTextCtrl* ctrl_vel_y_;

    wxRadioBox* radio_creation_mode_;
    wxPanel* panel_predefined_;
    wxPanel* panel_manual_;

    wxTextCtrl*         ctrl_manual_mass_;
    wxTextCtrl*         ctrl_manual_charge_;
    wxColourPickerCtrl* ctrl_manual_color_;
    wxSpinCtrl*         ctrl_manual_size_;

    double man_mass_value = 1;
    double man_charge_value = 0;
    double man_pos_x = 0;
    double man_pos_y = 0;
    double man_vel_x = 0;
    double man_vel_y = 0;

    enum {TIMER_ID = 1000};
    wxTimer timer_{this, TIMER_ID};
    bool paused_ = false;
    std::chrono::time_point<std::chrono::steady_clock> last_step_;
    
    const int timer_period_ms_ = 16;
    const wxSize min_client_size{800, 600};

    void CreateControls();
    void UpdateTemplatesCombo();

    void OnTimer(wxTimerEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnManageTemplates(wxCommandEvent& event);
    void OnCreationModeChanged(wxCommandEvent& event);
    void OnAddParticle(wxCommandEvent& event);

public:
    MainFrame();
};
