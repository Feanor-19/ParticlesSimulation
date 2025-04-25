#include "dlg_save_load.hpp"

#include <wx/valtext.h>

SaveDialog::SaveDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Save") 
{        
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    
    wxStaticText* label = new wxStaticText(this, wxID_ANY, "Save name:");
    ctrl_text_ = new wxTextCtrl(this, wxID_ANY);
    ctrl_text_->SetValidator(wxTextValidator(wxTextValidatorStyle::wxFILTER_EMPTY));
    
    wxBoxSizer* sizer_button = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btn_ok = new wxButton(this, wxID_OK, "OK");
    wxButton* btn_cancel = new wxButton(this, wxID_CANCEL, "Cancel");
    
    sizer_button->Add(btn_ok, 0, wxRIGHT, 5);
    sizer_button->Add(btn_cancel, 0, wxLEFT, 5);
    
    sizer_main->Add(label, 0, wxALL, 5);
    sizer_main->Add(ctrl_text_, 0, wxEXPAND | wxALL, 5);
    sizer_main->Add(sizer_button, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 5);
    
    SetSizer(sizer_main);
    SetSize(GetBestSize());
}

LoadDialog::LoadDialog(wxWindow *parent, 
                       std::vector<std::string> save_names,
                       std::function<void(std::string)> del_save_func)
    : wxDialog(parent, wxID_ANY, "Load"), del_save_func_(del_save_func)
{    
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    
    wxStaticText* label = new wxStaticText(this, wxID_ANY, "Select save:");
    list_box_ = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
                              0, NULL, wxLB_SINGLE | wxLB_HSCROLL);
    
    for (const auto& save : save_names)
        list_box_->Append(save);
    
    wxBoxSizer* sizer_button = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btn_ok = new wxButton(this, wxID_OK, "OK");
    wxButton* btn_cancel = new wxButton(this, wxID_CANCEL, "Cancel");
    wxButton* btn_delete = new wxButton(this, wxID_ANY, "Delete");
    
    sizer_button->Add(btn_ok, 0, wxRIGHT, 5);
    sizer_button->Add(btn_cancel, 0, wxLEFT | wxRIGHT, 5);
    sizer_button->Add(btn_delete, 0, wxLEFT, 5);

    sizer_main->Add(label, 0, wxALL, 5);
    sizer_main->Add(list_box_, 1, wxEXPAND | wxALL, 5);
    sizer_main->Add(sizer_button, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 5);
    
    SetSizer(sizer_main);

    btn_delete->Bind(wxEVT_BUTTON, &LoadDialog::OnDelete, this);
}

void LoadDialog::OnDelete(wxCommandEvent &event)
{
    int selection = list_box_->GetSelection();
    if (selection == wxNOT_FOUND) {
        wxMessageBox("Please select a save to delete", "Information", wxOK | wxICON_INFORMATION);
        return;
    }
    
    wxMessageDialog confirmDialog(this, 
        "Are you sure you want to delete this save?", 
        "Confirm Delete", 
        wxYES_NO | wxICON_QUESTION);
    
    if (confirmDialog.ShowModal() == wxID_YES) {
        del_save_func_(std::string(list_box_->GetStringSelection()));
        
        list_box_->Delete(selection);

        // TODO
        wxLogMessage("Deleted save at index %d", selection);
    }
}
