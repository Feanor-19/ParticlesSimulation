#pragma once

#include "common.hpp"

#include "simulator.hpp"
#include "sim_impl_manager.hpp"
#include "impl_force_calc.hpp"
#include "impl_integrator.hpp"
#include "sim_gui_wrapper.hpp"

#include "particle_templates.hpp"
#include "particle_canvas.hpp"
#include "dlg_impl_params.hpp"
#include "dlg_save_load.hpp"

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
using Simulation::Particle;

using SimGUI::SimGUIWrapper;

class MainFrame : public wxFrame 
{
private:
    SimGUIWrapper sim_gui_wrapper_;
    
    wxPanel* panel_;
    ParticleCanvas* canvas_;
    wxToggleButton* btn_pause_;

    wxButton* btn_delete;
    wxStaticText* label_info_mass_;
    wxStaticText* label_info_charge_;
    wxStaticText* label_info_color_;
    wxStaticText* label_info_size_;
    wxStaticText* label_info_pos_;
    wxStaticText* label_info_vel_;

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
    const wxSize field_size_{500, 500};

    const std::string msg_help = "CHANGE ME";

    void CreateControls();
    void CreateMenu();
    
    template<typename SelectHandler>
    void CreateImplConfMenu(wxMenu* menu, 
        const std::vector<std::string>& items,
        SelectHandler select_handler,
        std::function<void()> params_handler);
        
    void ShowIntegratorParams();
    void ShowForceCalcParams();
    
    void UpdateTemplatesCombo();
    void UpdateParticleInfo();

    void OnTimer(wxTimerEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnManageTemplates(wxCommandEvent& event);
    void OnCreationModeChanged(wxCommandEvent& event);
    void OnAddParticle(wxCommandEvent& event);
    void OnParticleSelected(wxCommandEvent& event);
    void OnDeleteParticle(wxCommandEvent& event);
    void OnDeleteAll(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnLoad(wxCommandEvent& event);

public:
    MainFrame();
};
