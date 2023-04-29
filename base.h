/*
Nilesh Hegde
#include "base.h"
1 February 2023
*/
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <dirent.h>
#include <limits.h>
#include <bits/stdc++.h>
#include <stdarg.h>
// MainApp is the class for our application, it just acts
// as a container for the window or frame in MainFrame.
class MainApp: public wxApp
{
public:
	virtual bool OnInit();
};

// MainFrame is the class for our window, it
// contains the window and all objects in it.
class MainFrame: public wxFrame
{
public:
    //The main frame where everything will be displayed
	MainFrame( const wxString &title, const wxPoint &pos, const wxSize &size );

    //All the functions that will be called on interacting with elements of frame
	void Quit( wxCommandEvent& event );
	void OpenFile( wxCommandEvent& event );
	void Baseline_Matching( wxCommandEvent& event );
	void Histogram_Matching( wxCommandEvent& event );
    void Multi_Histogram_Matching( wxCommandEvent& event );
    void Texture_Color( wxCommandEvent& event );
    void Red_Toy( wxCommandEvent& event );
    void Green_Trash( wxCommandEvent& event );
    void Select_Count( wxCommandEvent& event );

    //All the variables that stores value after interaction
	wxTextCtrl *MainEditBox;
	wxMenuBar *MainMenu;
	wxString targetpath;
	wxString targetfolder;
	wxString numberofimages;
	wxButton *baselinebutton;
	wxButton *histogrambutton;
	wxButton *multihistogrambutton;
	wxButton *texturecolor;
	wxButton *redtoy;
	wxButton *greentrash;
	wxTextCtrl *textnumberofimages;

	DECLARE_EVENT_TABLE()
};

enum
{
	//IDs of all buttons that are used in the application
	TEXT_Main = wxID_HIGHEST + 1,
	MENU_Open,
	MENU_Quit,
	BUTTON_baseline = wxID_HIGHEST + 2,
	BUTTON_histogram = wxID_HIGHEST + 3,
	BUTTON_multihistogram = wxID_HIGHEST + 4,
	BUTTON_texturecolor = wxID_HIGHEST + 5,
	BUTTON_redtoy = wxID_HIGHEST + 6,
	BUTTON_greentrash = wxID_HIGHEST + 7
};

//All the events associated with the application
BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
	EVT_MENU(MENU_Open, MainFrame::OpenFile)
	EVT_MENU(MENU_Quit, MainFrame::Quit)
	EVT_BUTTON (BUTTON_baseline, MainFrame::Baseline_Matching)
	EVT_BUTTON (BUTTON_histogram, MainFrame::Histogram_Matching)
	EVT_BUTTON (BUTTON_multihistogram, MainFrame::Multi_Histogram_Matching)
	EVT_BUTTON (BUTTON_texturecolor, MainFrame::Texture_Color)
	EVT_BUTTON (BUTTON_redtoy, MainFrame::Red_Toy)
	EVT_BUTTON (BUTTON_greentrash, MainFrame::Green_Trash)
END_EVENT_TABLE()


