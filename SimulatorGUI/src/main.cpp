#include "main_frame.hpp"

class App : public wxApp 
{
public:
    bool OnInit() override 
    {
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(App);