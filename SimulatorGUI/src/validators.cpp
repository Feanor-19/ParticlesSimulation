#include "validators.hpp"

DoubleGreaterThanZeroValidator::DoubleGreaterThanZeroValidator(int precision, double *value_ptr)
    : wxFloatingPointValidator<double>(precision, value_ptr)
{
    SetMin(std::numeric_limits<double>::min());
}

bool DoubleGreaterThanZeroValidator::IsInRange(double value) const
{
    return value > 0;
}
