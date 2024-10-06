#include "TheMainFrame.h"
#include <wx/wx.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <map>



enum IDS {
	BUTTON_ID = 2,
	SLIDER_ID = 3,
	TEXT_ID = 4,
	CHOICE_ID = 5,
};
//These are for debugging
wxBEGIN_EVENT_TABLE(TheMainFrame, wxFrame)
	EVT_BUTTON(BUTTON_ID,TheMainFrame::OnButtonClicked)
	EVT_SLIDER(SLIDER_ID,TheMainFrame::OnSliderChanged)
	EVT_TEXT(TEXT_ID, TheMainFrame::OnTextChanged)
	EVT_CHOICE(CHOICE_ID, TheMainFrame::OnChoiceSelected)
wxEND_EVENT_TABLE()

//This generates the window and has the skeleton for diffrerent widgets
TheMainFrame::TheMainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	/*wxButton* button = new wxButton(panel, wxID_ANY, "Button", wxPoint(150, 50), wxSize(100, 35));
	wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "CheckBox", wxPoint(550, 55));
	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Static Text", wxPoint(120, 150));
	wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "TextCTRL, - ediable", wxPoint(500, 145), wxSize(200, -1));
	wxSlider* slider = new wxSlider(panel, wxID_ANY, 25, 0, 100, wxPoint(100, 250), wxSize(200, -1));
	wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(500, 255), wxSize(200, -1));
	gauge->SetValue(0);*/

	//song names
	wxArrayString choices;
	choices.Add("Final Fantasy Victory");
	choices.Add("Hot Line Bling");
	choices.Add("Tee Hee Hee");
	choices.Add("greenhillzone");
	choices.Add("Symphony");

	//The song choices

	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Choose a song to play", wxPoint(435, 50), wxSize(500,20));

	wxChoice* choice = new wxChoice(panel, CHOICE_ID, wxPoint(250, 100), wxSize(500, 300), choices);
	choice->Select(0);

	CreateStatusBar();

	// The volume slider
	wxStaticText* taticText = new wxStaticText(panel, wxID_ANY, "Volume", wxPoint(480, 470), wxSize(500, 20));

	volumeSlider = new wxSlider(panel, wxID_ANY, 50, 0, 100, wxPoint(375, 550), wxSize(250,-1), wxSL_HORIZONTAL);
	volumeSlider->Bind(wxEVT_SLIDER, &TheMainFrame::OnVolumeChange, this);

	// You can make the voulme quieter or louder by changing this number
	SetMasterVolume(0.5f);
}

//This gives volume a perecent
void TheMainFrame::OnVolumeChange(wxScrollEvent& event) {
	int sliderValue = volumeSlider->GetValue();
	float volume = sliderValue / 100.0f;
	SetMasterVolume(volume);
}

// This is a bunch of windows stuff that is needed to change the volume. I had help from ChatGPT on this part in particular
void TheMainFrame::SetMasterVolume(float volume) {
	HRESULT hr;
	CoInitialize(NULL); 
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));

	IMMDevice* defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);

	IAudioEndpointVolume* endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&endpointVolume);

	endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);

	// Cleanup
	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();
}


std::map<int, wxString> soundFiles = {
	{ 0, "Final Fantasy Victory Fanfare - Sound Effect [HQ].wav" },
	{ 1, "Drake - Hotline Bling (Ringtone)" },
	{ 2, "goofy ahh laugh sfx.wav" },
	{ 3, "Sonic The Hedgehog OST - Green Hill Zone.wav" },
	{ 4, "Guilty Gear Strive OST - Symphony (A.B.A's Theme) (1).wav" }
};

void TheMainFrame::OnChoiceSelected(wxCommandEvent & evt) {
	int selection = evt.GetSelection();  // Get selected song

	auto it = soundFiles.find(selection);

	// I had a simpler way to do this but there was an error with the wav files. 
	if (it != soundFiles.end()) {
		wxString soundFile = it->second;

		if (!PlaySound(soundFile.wx_str(), NULL, SND_FILENAME | SND_ASYNC)) {
			wxLogError("Failed to play %s", soundFile);
		}
	}
}

	
	
	
//These allow you to change what happens on a default widget

	
void TheMainFrame::OnButtonClicked(wxCommandEvent& evt) {
	wxLogStatus("Button Clicked");
}

void TheMainFrame::OnSliderChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Slider Value: %d", evt.GetInt());
	wxLogStatus(str);
}
void TheMainFrame::OnTextChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Text: %s", evt.GetString());
	wxLogStatus(str);
}
