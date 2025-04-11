#include "main_frame.hpp"

// TODO ПРОВЕРИТЬ ВЕЗДЕ НЕЙМИНГ, В ЧАСТНОСТИ: ПРАВИЛЬНОЕ НАЛИЧИЕ _ В КОНЦЕ

void MainFrame::PushBackParticle(const Particle &sim_part, const ParticleVisual &vis_part)
{
    simulator_.push_back_particle(sim_part);
    vis_particles_.push_back(vis_part);
}

void MainFrame::RemoveParticle(size_t index)
{
    assert(index < vis_particles_.size() && index < simulator_.particles().size());

    simulator_.remove_particle(index);
    vis_particles_.erase(vis_particles_.begin() + index);
}

void MainFrame::CreateControls()
{
    panel_ = new wxPanel(this);

    wxBoxSizer* sizer_main = new wxBoxSizer(wxHORIZONTAL);
    
    // Canvas panel
    canvas_ = new ParticleCanvas(panel_, simulator_.particles(), vis_particles_);
    sizer_main->Add(canvas_, 1, wxEXPAND | wxALL, 5);

    // Control panel
    wxBoxSizer* sizer_control = new wxBoxSizer(wxVERTICAL);
    
    btn_pause_ = new wxToggleButton(panel_, wxID_ANY, "Pause");
    btn_pause_->Bind(wxEVT_TOGGLEBUTTON, &MainFrame::OnPause, this);
    sizer_control->Add(btn_pause_, 0, wxEXPAND | wxBOTTOM, 10);

    // TODO может вынести в верхнее меню?
    wxButton* btn_manage = new wxButton(panel_, wxID_ANY, "Manage particle templates...");
    sizer_control->Add(btn_manage, 0, wxEXPAND, 10);

    // -- Group add particle
    wxStaticBoxSizer* sizer_add_particle = new wxStaticBoxSizer(wxVERTICAL, panel_, "Add particle");

    wxArrayString choices;
    choices.Add("Use predefined type");
    choices.Add("Manual input");
    radio_creation_mode_ = new wxRadioBox(sizer_add_particle->GetStaticBox(), wxID_ANY, 
                                    "Creation mode", wxDefaultPosition, 
                                    wxDefaultSize, choices, 1, wxRA_SPECIFY_ROWS);
    radio_creation_mode_->Bind(wxEVT_RADIOBOX, &MainFrame::OnCreationModeChanged, this);
    sizer_add_particle->Add(radio_creation_mode_, 0, wxEXPAND|wxALL, 5);

    // -- -- Panel for adding particle using predefined templates
    panel_predefined_ = new wxPanel(sizer_add_particle->GetStaticBox());
    combo_type_ = new wxComboBox(panel_predefined_, wxID_ANY);
    panel_predefined_->SetSizer(new wxBoxSizer(wxVERTICAL));
    panel_predefined_->GetSizer()->Add(combo_type_, 1, wxEXPAND);
    sizer_add_particle->Add(panel_predefined_, 0, wxEXPAND|wxALL, 5);

    // -- -- Panel for adding particle using manual input
    panel_manual_ = new wxPanel(sizer_add_particle->GetStaticBox());
    panel_manual_->Hide();
    wxGridSizer* sizer_manual = new wxGridSizer(2, 5, 5);

    sizer_manual->Add(new wxStaticText(panel_manual_, wxID_ANY, "Mass:"));
    ctrl_manual_mass_ = new wxTextCtrl(panel_manual_, wxID_ANY, "");
    ctrl_manual_mass_->SetValidator(DoubleGreaterThanZeroValidator(input_double_precision, &man_mass_value));
    sizer_manual->Add(ctrl_manual_mass_, 1, wxEXPAND);

    sizer_manual->Add(new wxStaticText(panel_manual_, wxID_ANY, "Charge:"));
    ctrl_manual_charge_ = new wxTextCtrl(panel_manual_, wxID_ANY, "");
    ctrl_manual_charge_->SetValidator(wxFloatingPointValidator<double>{input_double_precision, &man_charge_value});
    sizer_manual->Add(ctrl_manual_charge_, 1, wxEXPAND);

    sizer_manual->Add(new wxStaticText(panel_manual_, wxID_ANY, "Color:"));
    ctrl_manual_color_ = new wxColourPickerCtrl(panel_manual_, wxID_ANY);
    sizer_manual->Add(ctrl_manual_color_, 1, wxEXPAND);

    sizer_manual->Add(new wxStaticText(panel_manual_, wxID_ANY, "Size:"));
    ctrl_manual_size_ = new wxSpinCtrl(panel_manual_, wxID_ANY);
    ctrl_manual_size_->SetRange(1, 100);
    sizer_manual->Add(ctrl_manual_size_, 1, wxEXPAND);

    panel_manual_->SetSizer(sizer_manual);
    sizer_add_particle->Add(panel_manual_, 0, wxEXPAND|wxALL, 5);

    // -- Position
    wxGridSizer* sizer_pos = new wxGridSizer(2, 5, 5);
    sizer_pos->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Position X:"));
    ctrl_pos_x_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_pos_x_->SetValidator(wxFloatingPointValidator<double>{input_double_precision, &man_pos_x});
    sizer_pos->Add(ctrl_pos_x_, 1, wxEXPAND);

    sizer_pos->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Position Y:"));
    ctrl_pos_y_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_pos_y_->SetValidator(wxFloatingPointValidator<double>{input_double_precision, &man_pos_y});
    sizer_pos->Add(ctrl_pos_y_, 1, wxEXPAND);
    sizer_add_particle->Add(sizer_pos, 0, wxEXPAND|wxALL, 5);

    // -- Velocity
    wxGridSizer* sizer_vel = new wxGridSizer(2, 5, 5);
    sizer_vel->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Velocity X:"));
    ctrl_vel_x_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_vel_x_->SetValidator(wxFloatingPointValidator<double>{input_double_precision, &man_vel_x});
    sizer_vel->Add(ctrl_vel_x_, 1, wxEXPAND);

    sizer_vel->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Velocity Y:"));
    ctrl_vel_y_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_vel_y_->SetValidator(wxFloatingPointValidator<double>{input_double_precision, &man_vel_y});
    sizer_vel->Add(ctrl_vel_y_, 1, wxEXPAND);
    sizer_add_particle->Add(sizer_vel, 0, wxEXPAND|wxALL, 5);

    // -- Button Add
    wxButton* btn_add = new wxButton(sizer_add_particle->GetStaticBox(), wxID_ANY, "Add");
    sizer_add_particle->Add(btn_add, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 5);
    
    // Sizer ending
    sizer_control->Add(sizer_add_particle, 0, wxEXPAND|wxALL, 10);
    sizer_main->Add(sizer_control, 0, wxEXPAND | wxALL, 5);
    panel_->SetSizer(sizer_main);

    // Init
    UpdateTemplatesCombo();
    panel_->TransferDataToWindow();
    
    // Eventd
    btn_manage->Bind(wxEVT_BUTTON, &MainFrame::OnManageTemplates, this);
    btn_add->Bind(wxEVT_BUTTON, &MainFrame::OnAddParticle, this);
    
    //SetSizerAndFit(sizer_main);
    SetMinClientSize(min_client_size);
}

void MainFrame::OnTimer(wxTimerEvent& event) 
{
    if (!paused_) 
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> dt = now - last_step_;
        simulator_.step(dt.count());
        canvas_->Refresh();
        last_step_ = now;
    }
}

void MainFrame::OnPause(wxCommandEvent& event) 
{
    paused_ = event.IsChecked();
    if (paused_) {
        timer_.Stop();
    } else {
        timer_.Start();
        last_step_ = std::chrono::steady_clock::now();
    }
}

void MainFrame::UpdateTemplatesCombo()
{
    combo_type_->Clear();
    const auto& types = ParticleTemplatesManager::Get().GetTemplates();
    for(const auto& t : types) {
        combo_type_->Append(t.name);
    }

    if(!types.empty()) 
        combo_type_->SetSelection(0);
}

void MainFrame::OnManageTemplates(wxCommandEvent &event)
{
    ManageTemplatesDialog dlg(this);
    dlg.ShowModal();
    UpdateTemplatesCombo();
}

void MainFrame::OnCreationModeChanged(wxCommandEvent &event)
{
    const int selection = radio_creation_mode_->GetSelection();
    
    panel_predefined_->Show(selection == 0);
    panel_manual_->Show(selection == 1);
    panel_->Layout();
}

void MainFrame::OnAddParticle(wxCommandEvent &event)
{
    const bool use_predefined = radio_creation_mode_->GetSelection() == 0;

    double mass = 0, charge = 0;
    wxColour color;
    int size;

    if (use_predefined) 
    {
        int selection = combo_type_->GetSelection();
        if (selection == wxNOT_FOUND) 
        {
            wxMessageBox("Please select a particle type!", "Error", wxOK|wxICON_ERROR);
            return;
        }

        const auto& pt = ParticleTemplatesManager::Get().GetTemplates()[selection];
        mass = pt.mass;
        charge = pt.charge;
        color = pt.color;
        size = pt.size;
    } 
    else 
    {
        if(panel_manual_->Validate() && panel_manual_->TransferDataFromWindow()) 
        {
            mass = man_mass_value;
            charge = man_charge_value;
            color = ctrl_manual_color_->GetColour();
            size = ctrl_manual_size_->GetValue();
        }
        else
        {
            wxMessageBox("Invalid parameters in manual mode!", "Error", wxOK|wxICON_ERROR);
            return;
        }

    }

    if (panel_->Validate() && panel_->TransferDataFromWindow())
    {
        PushBackParticle({mass, charge, {man_pos_x, man_pos_y}, {man_vel_x, man_vel_y}}, {color, size});
        canvas_->Refresh();
    }
    else
    {
        wxMessageBox("Invalid values of position or velocity!", "Error", wxOK|wxICON_ERROR);
        return;
    }

}

MainFrame::MainFrame() 
    : wxFrame(nullptr, wxID_ANY, "Physics Simulation")
{
    CreateControls();

    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this, TIMER_ID);
    timer_.Start(timer_period_ms_);

    last_step_ = std::chrono::steady_clock::now();
}