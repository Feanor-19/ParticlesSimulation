#include "dlg_impl_params.hpp"

ImplParamsDialog::ImplParamsDialog(wxWindow *parent, const std::vector<ImplParam> &params)
    : wxDialog(parent, wxID_ANY, "Parameters") 
{
    wxGridBagSizer* sizer = new wxGridBagSizer(5, 5);
    entries_.reserve(params.size());
    
    wxIntegerValidator<int> validator_index;
    validator_index.SetRange(0, 1000);
    
    for(size_t i = 0; i < params.size(); ++i) {
        const auto& p = params[i];
        
        sizer->Add(new wxStaticText(this, wxID_ANY, p.name + ":"),
                    wxGBPosition(i, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
        
        wxTextCtrl* ctrl = new wxTextCtrl(this, wxID_ANY, 
            wxString::FromDouble(p.value));
        ctrl->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
        
        sizer->Add(ctrl, wxGBPosition(i, 1), wxDefaultSpan, wxEXPAND);
        entries_.push_back(ctrl);
    }
    
    wxSizer* sizer_buttons = CreateButtonSizer(wxOK | wxCANCEL);
    sizer->Add(sizer_buttons, wxGBPosition(params.size(), 0), wxGBSpan(1, 2), wxEXPAND);
    
    SetSizerAndFit(sizer);
}

std::vector<double> ImplParamsDialog::GetValues() const
{
    std::vector<double> result;
    for(const auto& ctrl : entries_) {
        double value;
        ctrl->GetValue().ToDouble(&value);
        result.push_back(value);
    }
    return result;
}
