#include "App.h"
#include "TheMainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	TheMainFrame* theMainFrame = new TheMainFrame("C++ GUI");
	theMainFrame->SetClientSize(1000, 600);
	theMainFrame->Center();
	theMainFrame->Show();
	return true;
}
