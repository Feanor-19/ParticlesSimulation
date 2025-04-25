#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <vector>

class SaveDialog : public wxDialog 
{
private:
    wxTextCtrl* ctrl_text_;
public:
    SaveDialog(wxWindow* parent);
    
    wxString GetEnteredName() const { return ctrl_text_->GetValue(); }
};

class LoadDialog : public wxDialog 
{
private:
    wxListBox* list_box_;
    std::function<void(std::string)> del_save_func_;

public:
    LoadDialog(wxWindow* parent, 
               std::vector<std::string> save_names, 
               std::function<void(std::string)> del_save_func);
    
    wxString GetSelection() const { return list_box_->GetStringSelection(); }

    void OnDelete(wxCommandEvent& event);
};