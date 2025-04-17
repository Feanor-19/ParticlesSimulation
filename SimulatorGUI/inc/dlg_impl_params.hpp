#pragma once

#include "simulator.hpp"

#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/valnum.h>

using Simulation::ImplParam;

class ImplParamsDialog : public wxDialog {
private:
    std::vector<wxTextCtrl*> entries_;
public:
    ImplParamsDialog(wxWindow* parent, const std::vector<ImplParam>& params);
    
    std::vector<double> GetValues() const;
};