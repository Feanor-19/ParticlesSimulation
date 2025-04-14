#include "particle_templates.hpp"

#include <stdexcept>

void ParticleTemplatesManager::AddTemplate(const ParticleTemplate &type)
{
    templates_.push_back(type);
}

void ParticleTemplatesManager::RemoveTemplate(size_t index)
{
    if(index >= templates_.size()) 
        throw std::out_of_range("Template index out of range");

    templates_.erase(templates_.begin() + index);
}

AddParticleTemplateDialog::AddParticleTemplateDialog(wxWindow* parent) 
    : wxDialog(parent, wxID_ANY, "Add New Particle Template") {
    
    wxBoxSizer* main_sizer_ = new wxBoxSizer(wxVERTICAL);
    
    wxGridSizer* grid_sizer_ = new wxGridSizer(2, 5, 5);
    
    grid_sizer_->Add(new wxStaticText(this, wxID_ANY, "Name:"));
    ctrl_name_ = new wxTextCtrl(this, wxID_ANY, "");
    ctrl_name_->SetValidator(wxTextValidator(wxTextValidatorStyle::wxFILTER_EMPTY));
    grid_sizer_->Add(ctrl_name_, 1, wxEXPAND);
    
    grid_sizer_->Add(new wxStaticText(this, wxID_ANY, "Mass:"));
    ctrl_mass_ = new wxTextCtrl(this, wxID_ANY, "");
    ctrl_mass_->SetValidator(DoubleGreaterThanZeroValidator(GUI_CONSTS::INPUT_DOUBLE_PRECISION, &value_mass));
    grid_sizer_->Add(ctrl_mass_, 1, wxEXPAND);
    
    grid_sizer_->Add(new wxStaticText(this, wxID_ANY, "Charge:"));
    ctrl_charge_ = new wxTextCtrl(this, wxID_ANY, "");
    ctrl_charge_->SetValidator(wxFloatingPointValidator<double>{GUI_CONSTS::INPUT_DOUBLE_PRECISION, &value_charge});
    grid_sizer_->Add(ctrl_charge_, 1, wxEXPAND);
    
    grid_sizer_->Add(new wxStaticText(this, wxID_ANY, "Color:"));
    ctrl_color_ = new wxColourPickerCtrl(this, wxID_ANY);
    grid_sizer_->Add(ctrl_color_, 1, wxEXPAND);
    
    grid_sizer_->Add(new wxStaticText(this, wxID_ANY, "Size:"));
    ctrl_size_ = new wxSpinCtrl(this, wxID_ANY);
    ctrl_size_->SetRange(GUI_CONSTS::MIN_PARTICLE_SIZE, GUI_CONSTS::MAX_PARTICLE_SIZE);
    grid_sizer_->Add(ctrl_size_, 1, wxEXPAND);
    
    main_sizer_->Add(grid_sizer_, 1, wxEXPAND|wxALL, 10);
    
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    button_sizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxRIGHT, 10);
    button_sizer->Add(new wxButton(this, wxID_OK, "Add"), 0);
    
    main_sizer_->Add(button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 10);
    
    SetSizerAndFit(main_sizer_);
}

ParticleTemplate AddParticleTemplateDialog::GetResult() const
{
    ParticleTemplate pt;

    pt.name     = ctrl_name_->GetValue();
    pt.mass     = value_mass;
    pt.charge   = value_charge;
    pt.color    = ctrl_color_->GetColour();
    pt.size     = ctrl_size_->GetValue();
    
    return pt;
}

void ManageTemplatesDialog::ReloadTemplates()
{
    list_box_->Clear();
    const auto& types = ParticleTemplatesManager::Get().GetTemplates();
    for(const auto& t : types) {
        list_box_->Append(t.name);
    }
    
    if(!types.empty()) 
        list_box_->SetSelection(0);
    
    UpdatePanelInfo();
}

void ManageTemplatesDialog::UpdatePanelInfo()
{
    int selection = list_box_->GetSelection();
    if (selection == wxNOT_FOUND) {
        label_mass_->SetLabel("");
        label_charge_->SetLabel("");
        label_color_->SetLabel("");
        label_size_->SetLabel("");

        panel_info_->Layout();
        return;
    }
    
    const auto& types = ParticleTemplatesManager::Get().GetTemplates();
    if (selection >= 0 && selection < static_cast<int>(types.size())) 
    {
        const ParticleTemplate& pt = types[selection];
        label_mass_->SetLabel(wxString::Format("%.6g", pt.mass));
        label_charge_->SetLabel(wxString::Format("%.6g", pt.charge));
        label_color_->SetLabel(pt.color.GetAsString());
        label_size_->SetLabel(wxString::Format("%d", pt.size));
    }

    panel_info_->Layout();
}

void ManageTemplatesDialog::OnTemplateSelected(wxCommandEvent &event)
{
    UpdatePanelInfo();
}

void ManageTemplatesDialog::OnDelete(wxCommandEvent &event)
{
    int selection = list_box_->GetSelection();
    if (selection != wxNOT_FOUND) {
        ParticleTemplatesManager::Get().RemoveTemplate(selection);
        ReloadTemplates();
    }
}

void ManageTemplatesDialog::OnAdd(wxCommandEvent &event)
{
    AddParticleTemplateDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK) {
        ParticleTemplatesManager::Get().AddTemplate(dlg.GetResult());
        ReloadTemplates();
    }
}

ManageTemplatesDialog::ManageTemplatesDialog(wxWindow* parent) 
    : wxDialog(parent, wxID_ANY, "Manage Particle Templates") {
    
    wxBoxSizer* main_sizer_ = new wxBoxSizer(wxHORIZONTAL);
    
    list_box_ = new wxListBox(this, wxID_ANY);
    main_sizer_->Add(list_box_, 1, wxEXPAND|wxALL, 10);
    
    wxBoxSizer* right_sizer_ = new wxBoxSizer(wxVERTICAL);
    
    panel_info_ = new wxPanel(this);
    wxGridSizer* gridSizer = new wxGridSizer(2, 5, 5);
    gridSizer->Add(new wxStaticText(panel_info_, wxID_ANY, "Mass:"));
    label_mass_ = new wxStaticText(panel_info_, wxID_ANY, "");
    gridSizer->Add(label_mass_);
    
    gridSizer->Add(new wxStaticText(panel_info_, wxID_ANY, "Charge:"));
    label_charge_ = new wxStaticText(panel_info_, wxID_ANY, "");
    gridSizer->Add(label_charge_);
    
    gridSizer->Add(new wxStaticText(panel_info_, wxID_ANY, "Color:"));
    label_color_ = new wxStaticText(panel_info_, wxID_ANY, "");
    gridSizer->Add(label_color_);
    
    gridSizer->Add(new wxStaticText(panel_info_, wxID_ANY, "Size:"));
    label_size_ = new wxStaticText(panel_info_, wxID_ANY, "");
    gridSizer->Add(label_size_);
    
    panel_info_->SetSizer(gridSizer);
    right_sizer_->Add(panel_info_, 0, wxEXPAND|wxBOTTOM, 10);
    
    wxButton* delete_btn_ = new wxButton(this, wxID_ANY, "Delete chosen type");
    wxButton* add_btn_ = new wxButton(this, wxID_ANY, "Add new type...");
    
    right_sizer_->Add(delete_btn_, 0, wxEXPAND|wxBOTTOM, 5);
    right_sizer_->Add(add_btn_, 0, wxEXPAND);
    
    main_sizer_->Add(right_sizer_, 1, wxEXPAND|wxALL, 10);
    
    SetSizerAndFit(main_sizer_);
    
    list_box_->Bind(wxEVT_LISTBOX, &ManageTemplatesDialog::OnTemplateSelected, this);
    delete_btn_->Bind(wxEVT_BUTTON, &ManageTemplatesDialog::OnDelete, this);
    add_btn_->Bind(wxEVT_BUTTON, &ManageTemplatesDialog::OnAdd, this);
    
    ReloadTemplates();
}