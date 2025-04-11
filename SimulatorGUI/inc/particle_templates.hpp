#pragma once

#include "common.hpp"

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>

#include "validators.hpp"

struct ParticleTemplate 
{
    wxString name;
    double mass;
    double charge;
    wxColour color;
    int size;
};

class ParticleTemplatesManager 
{
private:
    std::vector<ParticleTemplate> templates_;
    ParticleTemplatesManager() = default;
public:
    static ParticleTemplatesManager& Get() {
        static ParticleTemplatesManager instance;
        return instance;
    }

    void AddTemplate(const ParticleTemplate& type);
    void RemoveTemplate(size_t index);

    const std::vector<ParticleTemplate>& GetTemplates() const { return templates_; }
};


class AddParticleTemplateDialog : public wxDialog 
{
private:
    wxTextCtrl*         ctrl_name_;
    wxTextCtrl*         ctrl_mass_;
    wxTextCtrl*         ctrl_charge_;
    wxColourPickerCtrl* ctrl_color_;
    wxSpinCtrl*         ctrl_size_;

    double value_mass   = 1;
    double value_charge = 0;
public:
    AddParticleTemplateDialog(wxWindow* parent);
    
    ParticleTemplate GetResult() const;
};


class ManageTemplatesDialog : public wxDialog 
{
private:
    wxListBox* list_box_;
    wxPanel* panel_info_;
    wxStaticText* label_mass_;
    wxStaticText* label_charge_;
    wxStaticText* label_color_;
    wxStaticText* label_size_;

    void ReloadTemplates();
    void UpdatePanelInfo();
    void OnTemplateSelected(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnAdd(wxCommandEvent& event);

public:
    ManageTemplatesDialog(wxWindow* parent);
};
    