#pragma once
#include <wx/wx.h>
class TheMainFrame : public wxFrame
{
public: 
	TheMainFrame(const wxString& title);
private:
	void OnButtonClicked(wxCommandEvent& evt);
	void OnSliderChanged(wxCommandEvent& evt);
	void OnTextChanged(wxCommandEvent& evt);
	void OnChoiceSelected(wxCommandEvent& evt);
	void OnVolumeChange(wxScrollEvent& evt);
	wxSlider* volumeSlider;
	void SetMasterVolume(float);


	wxDECLARE_EVENT_TABLE();
};

