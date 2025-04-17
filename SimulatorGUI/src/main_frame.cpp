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
    sizer_control->Add(btn_pause_, 0, wxEXPAND | wxBOTTOM, 10);
    
    // -- Group selected particle info
    wxStaticBoxSizer* sizer_info = new wxStaticBoxSizer(wxVERTICAL, panel_, "Selected Particle");
    
    label_info_mass_    = new wxStaticText(sizer_info->GetStaticBox(), wxID_ANY, "Mass: ");
    label_info_charge_  = new wxStaticText(sizer_info->GetStaticBox(), wxID_ANY, "Charge: ");
    label_info_color_   = new wxStaticText(sizer_info->GetStaticBox(), wxID_ANY, "Color: ");
    label_info_size_    = new wxStaticText(sizer_info->GetStaticBox(), wxID_ANY, "Size: ");
    
    sizer_info->Add(label_info_mass_, 0, wxEXPAND | wxTOP, 5);
    sizer_info->Add(label_info_charge_, 0, wxEXPAND | wxTOP, 5);
    sizer_info->Add(label_info_color_, 0, wxEXPAND | wxTOP, 5);
    sizer_info->Add(label_info_size_, 0, wxEXPAND | wxTOP, 5);
    
    btn_delete = new wxButton(sizer_info->GetStaticBox(), wxID_ANY, "Delete");
    sizer_info->Add(btn_delete, 0, wxEXPAND | wxTOP, 10);
    
    sizer_control->Add(sizer_info, 0, wxEXPAND | wxALL, 10);
    
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
    ctrl_manual_mass_->SetValidator(
        DoubleGreaterThanZeroValidator(GUI_CONSTS::INPUT_DOUBLE_PRECISION, &man_mass_value)
    );
    sizer_manual->Add(ctrl_manual_mass_, 1, wxEXPAND);
    
    sizer_manual->Add(new wxStaticText(panel_manual_, wxID_ANY, "Charge:"));
    ctrl_manual_charge_ = new wxTextCtrl(panel_manual_, wxID_ANY, "");
    ctrl_manual_charge_->SetValidator(
        wxFloatingPointValidator<double>{GUI_CONSTS::INPUT_DOUBLE_PRECISION, &man_charge_value}
    );
    sizer_manual->Add(ctrl_manual_charge_, 1, wxEXPAND);
    
    sizer_manual->Add(new wxStaticText(panel_manual_, wxID_ANY, "Color:"));
    ctrl_manual_color_ = new wxColourPickerCtrl(panel_manual_, wxID_ANY);
    sizer_manual->Add(ctrl_manual_color_, 1, wxEXPAND);
    
    sizer_manual->Add(new wxStaticText(panel_manual_, wxID_ANY, "Size:"));
    ctrl_manual_size_ = new wxSpinCtrl(panel_manual_, wxID_ANY);
    ctrl_manual_size_->SetRange(GUI_CONSTS::MIN_PARTICLE_SIZE, GUI_CONSTS::MAX_PARTICLE_SIZE);
    sizer_manual->Add(ctrl_manual_size_, 1, wxEXPAND);
    
    panel_manual_->SetSizer(sizer_manual);
    sizer_add_particle->Add(panel_manual_, 0, wxEXPAND|wxALL, 5);
    
    // -- Position
    wxGridSizer* sizer_pos = new wxGridSizer(2, 5, 5);
    sizer_pos->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Position X:"));
    ctrl_pos_x_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_pos_x_->SetValidator(wxFloatingPointValidator<double>{GUI_CONSTS::INPUT_DOUBLE_PRECISION, &man_pos_x});
    sizer_pos->Add(ctrl_pos_x_, 1, wxEXPAND);
    
    sizer_pos->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Position Y:"));
    ctrl_pos_y_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_pos_y_->SetValidator(wxFloatingPointValidator<double>{GUI_CONSTS::INPUT_DOUBLE_PRECISION, &man_pos_y});
    sizer_pos->Add(ctrl_pos_y_, 1, wxEXPAND);
    sizer_add_particle->Add(sizer_pos, 0, wxEXPAND|wxALL, 5);
    
    // -- Velocity
    wxGridSizer* sizer_vel = new wxGridSizer(2, 5, 5);
    sizer_vel->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Velocity X:"));
    ctrl_vel_x_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_vel_x_->SetValidator(wxFloatingPointValidator<double>{GUI_CONSTS::INPUT_DOUBLE_PRECISION, &man_vel_x});
    sizer_vel->Add(ctrl_vel_x_, 1, wxEXPAND);
    
    sizer_vel->Add(new wxStaticText(sizer_add_particle->GetStaticBox(), wxID_ANY, "Velocity Y:"));
    ctrl_vel_y_ = new wxTextCtrl(sizer_add_particle->GetStaticBox(), wxID_ANY, "0.0");
    ctrl_vel_y_->SetValidator(wxFloatingPointValidator<double>{GUI_CONSTS::INPUT_DOUBLE_PRECISION, &man_vel_y});
    sizer_vel->Add(ctrl_vel_y_, 1, wxEXPAND);
    sizer_add_particle->Add(sizer_vel, 0, wxEXPAND|wxALL, 5);
    
    // -- Button Add
    wxButton* btn_add = new wxButton(sizer_add_particle->GetStaticBox(), wxID_ANY, "Add");
    sizer_add_particle->Add(btn_add, 0, wxEXPAND | wxALL, 10);
    
    // Sizer ending
    sizer_control->Add(sizer_add_particle, 0, wxEXPAND|wxALL, 10);
    sizer_main->Add(sizer_control, 0, wxEXPAND | wxALL, 5);
    panel_->SetSizerAndFit(sizer_main);
    
    // Init
    UpdateTemplatesCombo();
    panel_->TransferDataToWindow();
    
    // Event
    btn_delete->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteParticle, this);
    radio_creation_mode_->Bind(wxEVT_RADIOBOX, &MainFrame::OnCreationModeChanged, this);
    btn_pause_->Bind(wxEVT_TOGGLEBUTTON, &MainFrame::OnPause, this);
    btn_manage->Bind(wxEVT_BUTTON, &MainFrame::OnManageTemplates, this);
    btn_add->Bind(wxEVT_BUTTON, &MainFrame::OnAddParticle, this);
    
    SetMinClientSize(min_client_size);
}

template <typename SelectHandler>
inline void MainFrame::CreateImplConfMenu(wxMenu *menu, 
                                          const std::vector<std::string> &items, 
                                          SelectHandler select_handler, 
                                          std::function<void()> params_handler)
{
    wxMenu* subMenu = new wxMenu();
    for(size_t i = 0; i < items.size(); ++i) {
        subMenu->AppendRadioItem(wxID_ANY, items[i]);
        subMenu->Bind(wxEVT_MENU, [=](wxCommandEvent&) { select_handler(i); }, 
                        subMenu->FindItemByPosition(i)->GetId());
    }
    
    menu->AppendSubMenu(subMenu, "Select");
    menu->AppendSeparator();
    menu->Append(wxID_PREFERENCES, "&Configure...");
    menu->Bind(wxEVT_MENU, [=](wxCommandEvent&) { params_handler(); }, wxID_PREFERENCES);
}

void MainFrame::CreateMenu()
{
        wxMenuBar* menu_bar = new wxMenuBar();
        
        wxMenu* menu_integrator_ = new wxMenu();
        CreateImplConfMenu(menu_integrator_, sim_manager_.get_all_integrator_names(), 
            [this](int index) { sim_manager_.set_integrator(index); },
            [this]() { ShowIntegratorParams(); });
        
        wxMenu* menu_force_calc_ = new wxMenu();
        CreateImplConfMenu(menu_force_calc_, sim_manager_.get_all_force_calc_names(),
            [this](int index) { sim_manager_.set_force_calc(index); },
            [this]() { ShowForceCalcParams(); });
        
        wxMenu* menu_help_ = new wxMenu();
        menu_help_->Append(wxID_ABOUT);
        Bind(wxEVT_MENU, [this](wxCommandEvent&) {
            wxMessageBox(msg_help, "About");
        }, wxID_ABOUT);
        
        menu_bar->Append(menu_integrator_, "&Integrator");
        menu_bar->Append(menu_force_calc_, "&Force Calculator");
        menu_bar->Append(menu_help_, "&Help");
        
        SetMenuBar(menu_bar);
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

void MainFrame::UpdateParticleInfo()
{
    if(canvas_->selected_particle().has_value()) {
        size_t pt_ind = *(canvas_->selected_particle());
        const auto &sim_pts = simulator_.particles();
        ParticleVisual pt_vis = vis_particles_[pt_ind];

        label_info_mass_->SetLabel(wxString::Format("Mass: %.2f", sim_pts.mass(pt_ind)));
        label_info_charge_->SetLabel(wxString::Format("Charge: %.2f", sim_pts.charge(pt_ind)));
        label_info_color_->SetLabel(wxString::Format("Color: %s", pt_vis.colour.GetAsString()));
        label_info_size_->SetLabel(wxString::Format("Size: %d", pt_vis.size));
        btn_delete->Enable();
    }
    else {
        label_info_mass_->SetLabel("Mass: ");
        label_info_charge_->SetLabel("Charge: ");
        label_info_color_->SetLabel("Color: ");
        label_info_size_->SetLabel("Size: ");
        btn_delete->Disable();
    }
    Layout();
}

// TODO подумать, как объединить с ShowForceCalcParams()
void MainFrame::ShowIntegratorParams()
{
    auto params = simulator_.integrator()->get_params();
        
    if (params.empty()) {
        wxMessageBox("No parameters for this integrator", "Info");
        return;
    }
    
    ImplParamsDialog dlg(this, params);
    if(dlg.ShowModal() == wxID_OK) {
        auto values = dlg.GetValues();
        simulator_.integrator()->set_params(values);
    }
}

void MainFrame::ShowForceCalcParams()
{
    auto params = simulator_.force_calc()->get_params();
    if(params.empty()) {
        wxMessageBox("No parameters for this force calculator", "Info");
        return;
    }
    
    ImplParamsDialog dlg(this, params);
    if(dlg.ShowModal() == wxID_OK) {
        auto values = dlg.GetValues();
        simulator_.force_calc()->set_params(values);
    }
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
    : wxFrame(nullptr, wxID_ANY, "Physics Simulation"), sim_manager_(simulator_)
{
    CreateControls();
    CreateMenu();

    Bind(EVT_PARTICLE_SELECTED, &MainFrame::OnParticleSelected, this);

    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this, TIMER_ID);
    timer_.Start(timer_period_ms_);
    last_step_ = std::chrono::steady_clock::now();

    sim_manager_.add_integrator<RungeKutta4Integrator>();
    sim_manager_.add_force_calc<LennardJonesForceCalc>();

    sim_manager_.set_force_calc(0);
    sim_manager_.set_integrator(0);
}

void MainFrame::OnParticleSelected(wxCommandEvent &event)
{
    UpdateParticleInfo();
}

void MainFrame::OnDeleteParticle(wxCommandEvent &event)
{
    if(canvas_->selected_particle().has_value()) {
        RemoveParticle(*(canvas_->selected_particle()));
        canvas_->unselect_particle();
        UpdateParticleInfo();
        canvas_->Refresh();
    }
}
