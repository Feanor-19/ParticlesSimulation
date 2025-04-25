#pragma once

#include <wx/colour.h>

namespace GUI_CONSTS
{

const int INPUT_DOUBLE_PRECISION = 6;

const int MIN_PARTICLE_SIZE = 3;
const int MAX_PARTICLE_SIZE = 30;

const char * const DB_PATH = "sim_gui.db";
    
} // namespace GUI_CONSTS

namespace SimGUI
{

struct ParticleVisual
{
    wxColour colour;
    int size;
};
    
} // namespace SimGUI
