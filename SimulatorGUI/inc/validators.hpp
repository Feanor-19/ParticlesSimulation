#pragma once

#include <wx/wx.h>
#include <wx/valnum.h>

class DoubleGreaterThanZeroValidator : public wxFloatingPointValidator<double> 
{
public:
    DoubleGreaterThanZeroValidator(int precision, double *value_ptr);

    bool IsInRange(double value) const override;
    
    wxObject* Clone() const override {
        return new DoubleGreaterThanZeroValidator(*this);
    }
};
    