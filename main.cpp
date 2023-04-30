#include "base.h"
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

using namespace cv;
using namespace std;

/*
Use - This function returns an image of type wxImage which is used in displaying target image in application
Arguments - An image of openCV Mat type
Return - An image of wxImage type
*/
wxImage wx_from_mat(Mat &img)
{
    Mat im2;
    cvtColor(img,im2,COLOR_BGR2RGB);
    long imsize = im2.rows*im2.cols*im2.channels();
    wxImage wx(im2.cols, im2.rows,(unsigned char*)malloc(imsize), false);
    unsigned char* s=im2.data;
    unsigned char* d=wx.GetData();
    for (long i = 0; i < imsize; i++)
    {
        d[i] = s[i];
    }
    return wx;
}

/*
Use - This function is used to make a new window to display N images in a single window
Arguments - Window title of type string, number of images of type int, array of images of type Mat
Return - None
*/
void ShowManyImages(string title, int ni, Mat img[])
{
	int size;
	int i;
	int m, n;
	int x, y;
	// w - Maximum number of images in a row
	// h - Maximum number of images in a column
	int w, h;
	// scale - How much we have to resize the image
	float scale;
	int max;
	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying
	if(ni <= 0) {
		printf("Number of arguments too small....\n");
		return;
	}
	else if(ni > 14) {
		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (ni == 1) {
		w = h = 1;
		size = 300;
	}
	else if (ni == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (ni == 3 || ni == 4) {
		w = 2; h = 2;
		size = 300;
	}
	else if (ni == 5 || ni == 6) {
		w = 3; h = 2;
		size = 200;
	}
	else if (ni == 7 || ni == 8) {
		w = 4; h = 2;
		size = 200;
	}
	else {
		w = 4; h = 3;
		size = 150;
	}

	// Create a new 3 channel image
	Mat DispImage = Mat::zeros(Size(100 + size*w, 60 + size*h), CV_8UC3);

	// Loop for ni number of arguments
	for (i = 0, m = 20, n = 20; i < ni; i++, m += (20 + size))
	{
		// Check whether image is NULL or not
		// If it is NULL, release the image, and return
		if(img[i].empty()) {
		    printf("Invalid arguments");
		    return;
		}
		// Find the width and height of the image
		x = img[i].cols;
		y = img[i].rows;
		// Find whether height or width is greater in order to resize the image
		max = (x > y)? x: y;
		// Find the scaling factor to resize the image
		scale = (float) ( (float) max / size );
		// Used to Align the images
		if( i % w == 0 && m!= 20) {
		    m = 20;
		    n+= 20 + size;
		}
		// Set the image ROI to display the current image
		// Resize the input image and copy the it to the Single Big Image
		Rect ROI(m, n, (int)( x/scale ), (int)( y/scale ));
		Mat temp; resize(img[i],temp, Size(ROI.width, ROI.height));
		temp.copyTo(DispImage(ROI));
	}
	// Create a new window, and show the Single Big Image
	namedWindow( title, 1 );
	imshow( title, DispImage);
	waitKey();
}

/*
Use - Compare two integer values in a hashmap of string-int key-value pair
Arguments - two pairs of hash-key value
Return - boolean result of comparison
*/
bool cmpint(pair<string, int>& m,
        pair<string, int>& n)
{
    return m.second < n.second;
}

/*
Use - Compare two integer values in a hashmap of string-float key-value pair
Arguments - two pairs of hash-key value
Return - boolean result of comparison
*/
bool cmpfloat(pair<string, float>& m,
        pair<string, float>& n)
{
    return m.second < n.second;
}

/*
Use - Sort an string-int hashmap based on integer values
Arguments - string-integer hashmap, integer number of images that need to be displayed
Return - None
*/
void sortmapbaseline(map<string, int>& arr, int ct = 3)
{
    vector<pair<string, int> > A;
    for (auto& it : arr)
    {
        A.push_back(it);
    }
    sort(A.begin(), A.end(), cmpint);
    int c = 0;
    Mat img[ct];
    for (auto& it : A)
    {
    	img[c] = imread( it.first, IMREAD_COLOR );
        cout << it.first << ' '<< it.second << endl;
        putText(img[c],it.first.substr(it.first.find_last_of("/")+1),Point(10, 15),FONT_HERSHEY_DUPLEX,0.5,CV_RGB(0, 255, 0),1);
        c += 1;
        if(c == ct)
        {
        	break;
        }
    }
    ShowManyImages("Images Matched", c, img);
}

/*
Use - Sort an string-float hashmap based on integer values
Arguments - string-float hashmap, integer number of images that need to be displayed
Return - None
*/
void sortmaphistogram(map<string, float>& arr, int ct = 3)
{
    vector<pair<string, float> > A;
    for (auto& it : arr)
    {
        A.push_back(it);
    }
    sort(A.begin(), A.end(), cmpfloat);
    int c = 0;
    Mat img[ct];
    for (auto& it : A)
    {
    	img[c] = imread( it.first, IMREAD_COLOR );
        cout << it.first << ' '<< it.second << endl;
        putText(img[c],it.first.substr(it.first.find_last_of("/")+1),Point(10, 15),FONT_HERSHEY_DUPLEX,0.5,CV_RGB(0, 255, 0),1);
        c += 1;
        if(c == ct)
        {
        	break;
        }
    }
    ShowManyImages("Images Matched", c, img);
}

/*
Use - Applies the Sobel-X operator in X direction
Arguments - Mat target image, Mat destination image
Return - None
*/
void sobelX3x3( Mat &src, Mat &dst )
{
    Mat temp;
    dst.create(src.size(), CV_16SC3);
    //Create a temporary image matrix to hold values after applying gauss operator in horizontal direction
    temp.create(src.size(), CV_16SC3);
    //Horizontal direction convolution
    for (int i = 1; i < src.rows-1; i++)
    {
        for (int j = 1; j < src.cols-1; j++)
        {
            for (int c = 0; c < src.channels(); c++)
            {
            	//Convolve using 1x3 sobel operator on source image
                temp.at<Vec3s>(i,j)[c] = src.at<Vec3b>(i-1, j-1)[c]*(-1) + src.at<Vec3b>(i-1, j)[c]*0 + src.at<Vec3b>(i-1, j+1)[c];
            }
        }
    }
    //Vertical direction convolution
    for (int i = 1; i < temp.rows-1; i++)
    {
        for (int j = 1; j < temp.cols-1; j++)
        {
            for (int c = 0; c < temp.channels(); c++)
            {
            	//Convolve using 3x1 sobel operator on temporary image
                dst.at<Vec3s>(i,j)[c] = (temp.at<Vec3s>(i-1, j-1)[c] + temp.at<Vec3s>(i, j-1)[c]*2 + temp.at<Vec3s>(i+1, j-1)[c])/4 ;
            }
        }
    }
}

/*
Use - Applies the Sobel-Y operator in Y direction
Arguments - Mat target image, Mat destination image
Return - None
*/
void sobelY3x3( Mat &src, Mat &dst )
{
    Mat temp;
    dst.create(src.size(), CV_16SC3);
    temp.create(src.size(), CV_16SC3);
    //Horizontal direction convolution
    for (int i = 1; i < src.rows-1; i++)
    {
        for (int j = 1; j < src.cols-1; j++)
        {
            for (int c = 0; c < src.channels(); c++)
            {
            	//Convolve using 1x3 sobel operator on source image
                temp.at<Vec3s>(i,j)[c] = (src.at<Vec3b>(i-1, j-1)[c] + src.at<Vec3b>(i-1, j)[c]*2 + src.at<Vec3b>(i-1, j+1)[c])/4;
            }
        }
    }
    //Vertical direction convolution
    for (int i = 1; i < temp.rows-1; i++)
    {
        for (int j = 1; j < temp.cols-1; j++)
        {
            for (int c = 0; c < temp.channels(); c++)
            {
            	//Convolve using 3x1 sobel operator on temporary image
                dst.at<Vec3s>(i,j)[c] = temp.at<Vec3s>(i-1, j-1)[c] + temp.at<Vec3s>(i, j-1)[c]*0 + temp.at<Vec3s>(i+1, j-1)[c]*(-1) ;
            }
        }
    }
}

/*
Use - Finds the magnitude of an image
Arguments - Mat target image, Mat destination image
Return - None
*/
void magnitude( Mat &sx, Mat &sy, Mat &dst )
{
    dst.create(sx.size(), sx.type());
    for (int i = 0; i < sx.rows; i++)
    {
        for (int j = 0; j < sx.cols; j++)
        {
            for (int c = 0; c < sx.channels(); c++)
            {
            	//Find euclidean magnitude for each channel in each pixel
                dst.at<Vec3s>(i,j)[c] = sqrt(pow(sx.at<Vec3s>(i, j)[c], 2) + pow(sy.at<Vec3s>(i, j)[c], 2));
            }
        }
    }
}

// Initializes the MainApp class
IMPLEMENT_APP(MainApp)

// .. and tells our program to run it
bool MainApp::OnInit()
{
	// Create an instance of our frame, or window
	MainFrame *MainWin = new MainFrame(
		_("Project 2"), wxPoint(1, 1), wxSize(300, 200));
    // show the window
	MainWin->Show(TRUE);
    // and finally, set it as the main window
	SetTopWindow(MainWin);
	return TRUE;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame((wxFrame*) NULL, -1, title, pos, size)
{
	CreateStatusBar(2);
	MainMenu = new wxMenuBar();
	wxMenu *FileMenu = new wxMenu();
	FileMenu->Append(MENU_Open,
		_("&Select"), _("Select an Existing Image"));
	FileMenu->AppendSeparator();
	FileMenu->Append(MENU_Quit,
		_("&Quit"), _("Quit the Application"));
	MainMenu->Append(FileMenu, _("&Load"));
	SetMenuBar(MainMenu);
    // Maximize the window
	Maximize();
}

/*
Use - FUnction called on opening and loading an image
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::OpenFile(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Choose a file to open"), wxEmptyString, wxEmptyString,_("Image files (*.jpg)|*.jpg"),wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog with 1 file types
	if (OpenDialog->ShowModal() == wxID_OK)
	{
        //Get path and name of the file chosen
        wxFileName fname( OpenDialog->GetPath() );
        //Display path and name of the file
        wxStaticText* staticText1 = new wxStaticText(this, wxID_ANY, "Chosen File -", wxPoint(50,30), wxSize(100,20), wxALIGN_LEFT);
        wxStaticText* staticText2 = new wxStaticText(this, wxID_ANY, fname.GetFullName(), wxPoint(50,50), wxSize(250,50), wxALIGN_LEFT);
        staticText2->SetBackgroundColour((128,128,128));
        wxStaticText* staticText3 = new wxStaticText(this, wxID_ANY, "Absolute Directory -", wxPoint(50,100), wxSize(200,20), wxALIGN_LEFT);
        wxStaticText* staticText4 = new wxStaticText(this, wxID_ANY, fname.GetPath(), wxPoint(50,120), wxSize(250,50), wxALIGN_LEFT);
        staticText4->SetBackgroundColour((128,128,128));
        //Display Target image
        wxStaticText* staticText5 = new wxStaticText(this, wxID_ANY, "Target Image -", wxPoint(50,170), wxSize(100,20), wxALIGN_LEFT);
        targetfolder = fname.GetPath();
        targetpath = fname.GetPath()+"/"+fname.GetFullName();
        wxJPEGHandler *handler = new wxJPEGHandler;
        wxImage::AddHandler(handler);
        wxStaticBitmap *image;
        Mat target,rescaled;
        target = imread(targetpath.ToStdString(), IMREAD_COLOR);
        resize(target, rescaled, Size(300, 300), INTER_LINEAR);
        wxBitmap imwx1=wx_from_mat(rescaled);
        image = new wxStaticBitmap( this, wxID_ANY, imwx1, wxPoint(50,190), wxSize(300,300));
        //Display button for Baseline Matching
        baselinebutton = new wxButton(this, BUTTON_baseline, _T("Baseline Matching"),wxPoint(50,550), wxSize(200,20), 0);
        //Display button for Histogram Matching
        histogrambutton = new wxButton(this, BUTTON_histogram, _T("Histogram Matching"),wxPoint(50,580), wxSize(200,20), 0);
        //Display button for Multi-Histogram Matching
        multihistogrambutton = new wxButton(this, BUTTON_multihistogram, _T("Multi-Histogram Matching"),wxPoint(50,610), wxSize(200,20), 0);
        //Display button for Color-texture Matching
        texturecolor = new wxButton(this, BUTTON_texturecolor, _T("Texture and Color"),wxPoint(50,640), wxSize(200,20), 0);
        //Display button for Redtoy Matching
        redtoy = new wxButton(this, BUTTON_redtoy, _T("Find Red Toy"),wxPoint(50,700), wxSize(200,20), 0);
        //Display button for Green dumpster Matching
        greentrash = new wxButton(this, BUTTON_greentrash, _T("Find Green Dumpster"),wxPoint(50,670), wxSize(200,20), 0);
        //Display the top N number of matches
        textnumberofimages = new wxTextCtrl(this,wxID_ANY,"No. of Images (1-12)",wxPoint(50,520), wxSize(200,20), 0);
	}
}

/*
Use - Function quit the application
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::Quit(wxCommandEvent& WXUNUSED(event))
{
    // Close the window
	numberofimages = textnumberofimages->GetValue();
}

/*
Use - Function to perform baseline matching
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::Baseline_Matching(wxCommandEvent& WXUNUSED(event))
{
    char buffer[256];
    char dirname[256];
    struct dirent *dp;
    DIR *dirp;
    strcpy(dirname,targetfolder.ToStdString().c_str());
    dirp = opendir(dirname);
    Mat target;
    target = imread(targetpath.ToStdString(), IMREAD_COLOR);
    //Hashmap to store name-distance pair
    map<std::string, int> hashmapbaseline;
    while( (dp = readdir(dirp)) != NULL )
    {
        int ssd = 0;
        Mat image;
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") )
        {
            strcpy(buffer, targetfolder.ToStdString().c_str());
      		strcat(buffer, "/");
      		strcat(buffer, dp->d_name);
      		//Skip the image already chosen
      		if(strcmp(buffer,targetpath.ToStdString().c_str())==0)
      		{
      			continue;
      		}
			image = imread( buffer, IMREAD_COLOR );
			//Calculating SSD for 9x9 rectangle at the center
            for (int i = 252; i < 261; i++)
            {
                for (int j = 316; j < 325; j++)
                {
                    for (int c = 0; c < 3; c++)
                    {
                        int diff = image.at<cv::Vec3b>(i, j)[c] - target.at<cv::Vec3b>(i, j)[c];
                        ssd += (diff*diff);
                    }
                }
            }
            //Normalize the value
            ssd /= (9*9);
            hashmapbaseline.insert({targetfolder.ToStdString()+"/"+dp->d_name, ssd});
        }
    }
    //Sort the hashmap
    sortmapbaseline(hashmapbaseline, wxAtoi(textnumberofimages->GetValue()));
}

/*
Use - Function to perform Histogram Matching
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::Histogram_Matching(wxCommandEvent& WXUNUSED(event))
{
    char buffer[256];
    char dirname[256];
    struct dirent *dp;
    DIR *dirp;
    strcpy(dirname,targetfolder.ToStdString().c_str());
    dirp = opendir(dirname);
    Mat target;
    target = imread(targetpath.ToStdString(), IMREAD_COLOR);
    map<std::string, float> hashmaphistogram;
    while( (dp = readdir(dirp)) != NULL )
    {
        wxWindow::Update();
        Mat image;
        int dim[3] = {32,32,32};
        //Create histograms
		Mat targethistogram = Mat::zeros(3, dim, CV_32F);
		Mat imagehistogram = Mat::zeros(3, dim, CV_32F);
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") )
        {
            strcpy(buffer, targetfolder.ToStdString().c_str());
      		strcat(buffer, "/");
      		strcat(buffer, dp->d_name);
      		if(strcmp(buffer,targetpath.ToStdString().c_str())==0)
      		{
      			continue;
      		}
			image = imread( buffer, IMREAD_COLOR );
			//Compute histogram values for both target and images in directory
            for (int i = 0; i < image.rows; i++)
		    {
		        for (int j = 0; j < image.cols; j++)
		        {
		    		targethistogram.at<float>(target.at<cv::Vec3b>(i, j)[0]/32,target.at<cv::Vec3b>(i, j)[1]/32,target.at<cv::Vec3b>(i, j)[2]/32)++;
		    		imagehistogram.at<float>(image.at<cv::Vec3b>(i, j)[0]/32,image.at<cv::Vec3b>(i, j)[1]/32,image.at<cv::Vec3b>(i, j)[2]/32)++;
		    	}
		    }
		    float totalimage = 0;
		    //For each bin, perform histogram intersection
		    for (int i = 0; i < 8; i++)
		    {
		        for (int j = 0; j < 8; j++)
		        {
		            for (int k = 0; k < 8; k++)
		            {
		                totalimage += (targethistogram.at<float>(i, j, k) < imagehistogram.at<float>(i, j, k) ? targethistogram.at<float>(i, j, k) : imagehistogram.at<float>(i, j, k)) / (image.rows*image.cols);
		            }
		        }
        	}
        	float difference = 1- totalimage;
        	//Add name-distance metric to hashmap
        	hashmaphistogram.insert({targetfolder.ToStdString()+"/"+dp->d_name, difference});
        }
    }
    //Sort hashmap
    sortmaphistogram(hashmaphistogram, wxAtoi(textnumberofimages->GetValue()));
}

/*
Use - Function to perform multi-histogram matching
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::Multi_Histogram_Matching(wxCommandEvent& WXUNUSED(event))
{
    char buffer[256];
    char dirname[256];
    struct dirent *dp;
    DIR *dirp;
    strcpy(dirname,targetfolder.ToStdString().c_str());
    dirp = opendir(dirname);
    Mat target;
    target = imread(targetpath.ToStdString(), IMREAD_COLOR);
    map<std::string, float> hashmapmultihistogram;
    while( (dp = readdir(dirp)) != NULL )
    {
        wxWindow::Update();
        Mat image;
        //Create Histograms
        int dim[3] = {32,32,32};
		Mat toptargethistogram = Mat::zeros(3, dim, CV_32F);
		Mat topimagehistogram = Mat::zeros(3, dim, CV_32F);
		Mat bottomtargethistogram = Mat::zeros(3, dim, CV_32F);
		Mat bottomimagehistogram = Mat::zeros(3, dim, CV_32F);
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") )
        {
            strcpy(buffer, targetfolder.ToStdString().c_str());
      		strcat(buffer, "/");
      		strcat(buffer, dp->d_name);
      		if(strcmp(buffer,targetpath.ToStdString().c_str())==0)
      		{
      			continue;
      		}
			image = imread( buffer, IMREAD_COLOR );
			//Calculate Histogram for both target and image
            for (int i = 0; i < image.rows; i++)
		    {
		        for (int j = 0; j < image.cols; j++)
		        {
                    //Top half of image
		        	if(i<(int)(image.rows/2))
		        	{
						toptargethistogram.at<float>(target.at<cv::Vec3b>(i, j)[0]/32,target.at<cv::Vec3b>(i, j)[1]/32,target.at<cv::Vec3b>(i, j)[2]/32)++;
						topimagehistogram.at<float>(image.at<cv::Vec3b>(i, j)[0]/32,image.at<cv::Vec3b>(i, j)[1]/32,image.at<cv::Vec3b>(i, j)[2]/32)++;
					}
					//Bottom half of image
					else
					{
						bottomtargethistogram.at<float>(target.at<cv::Vec3b>(i, j)[0]/32,target.at<cv::Vec3b>(i, j)[1]/32,target.at<cv::Vec3b>(i, j)[2]/32)++;
						bottomimagehistogram.at<float>(image.at<cv::Vec3b>(i, j)[0]/32,image.at<cv::Vec3b>(i, j)[1]/32,image.at<cv::Vec3b>(i, j)[2]/32)++;
					}
		    	}
		    }
		    float toptotalimage = 0;
		    float bottomtotalimage = 0;
		    //Perform histogram intersection
		    for (int i = 0; i < 8; i++)
		    {
		        for (int j = 0; j < 8; j++)
		        {
		            for (int k = 0; k < 8; k++)
		            {
		                toptotalimage += (toptargethistogram.at<float>(i, j, k) < topimagehistogram.at<float>(i, j, k) ? toptargethistogram.at<float>(i, j, k) : topimagehistogram.at<float>(i, j, k)) / (0.5*image.rows*image.cols);
		                bottomtotalimage += (bottomtargethistogram.at<float>(i, j, k) < bottomimagehistogram.at<float>(i, j, k) ? bottomtargethistogram.at<float>(i, j, k) : bottomimagehistogram.at<float>(i, j, k)) / (0.5*image.rows*image.cols);
		            }
		        }
        	}
        	float topdifference =  1 - toptotalimage;
        	float bottomdifference =  1 - bottomtotalimage;
        	float difference =  0.5*topdifference + 0.5*bottomdifference;
        	//Store name and distance metric in hashmap
        	hashmapmultihistogram.insert({targetfolder.ToStdString()+"/"+dp->d_name, difference});
        }
    }
    //sorthashmap
    sortmaphistogram(hashmapmultihistogram, wxAtoi(textnumberofimages->GetValue()));
}

/*
Use - Function to perform texture color matching
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::Texture_Color(wxCommandEvent& WXUNUSED(event))
{
    char buffer[256];
    char dirname[256];
    struct dirent *dp;
    DIR *dirp;
    strcpy(dirname,targetfolder.ToStdString().c_str());
    dirp = opendir(dirname);
    Mat target;
    target = imread(targetpath.ToStdString(), IMREAD_COLOR);
    map<std::string, float> hashmaptexturecolor;
    Mat targetX, targetY, targettexture;
    //Compute sobel magnitude of target
    sobelX3x3( target, targetX );
    sobelY3x3( target, targetY );
    magnitude( targetX, targetY, targettexture );
    convertScaleAbs(targettexture, targettexture);
    while( (dp = readdir(dirp)) != NULL )
    {
        wxWindow::Update();
        Mat image;
        int dim[3] = {32,32,32};
        //create histograms
		Mat targetcolorhistogram = Mat::zeros(3, dim, CV_32F);
        Mat targettexturehistogram = Mat::zeros(3, dim, CV_32F);
        Mat imagecolorhistogram = Mat::zeros(3, dim, CV_32F);
        Mat imagetexturehistogram = Mat::zeros(3, dim, CV_32F);
        Mat imageX, imageY, imagetexture;
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") )
        {
            strcpy(buffer, targetfolder.ToStdString().c_str());
      		strcat(buffer, "/");
      		strcat(buffer, dp->d_name);
      		if(strcmp(buffer,targetpath.ToStdString().c_str())==0)
      		{
      			continue;
      		}
			image = imread( buffer, IMREAD_COLOR );
			//Compute sobel magnitude of images
            sobelX3x3( image, imageX );
			sobelY3x3( image, imageY );
			magnitude( imageX, imageY, imagetexture );
			convertScaleAbs(imagetexture, imagetexture);
			//Calculate Histogram
			for (int i = 0; i < image.rows; i++)
		    {
		        for (int j = 0; j < image.cols; j++)
		        {
		        	targetcolorhistogram.at<float>(target.at<cv::Vec3b>(i, j)[0]/32,target.at<cv::Vec3b>(i, j)[1]/32,target.at<cv::Vec3b>(i, j)[2]/32)++;
		        	targettexturehistogram.at<float>(targettexture.at<cv::Vec3b>(i, j)[0]/32,targettexture.at<cv::Vec3b>(i, j)[1]/32,targettexture.at<cv::Vec3b>(i, j)[2]/32)++;
		    		imagecolorhistogram.at<float>(image.at<cv::Vec3b>(i, j)[0]/32,image.at<cv::Vec3b>(i, j)[1]/32,image.at<cv::Vec3b>(i, j)[2]/32)++;
		    		imagetexturehistogram.at<float>(imagetexture.at<cv::Vec3b>(i, j)[0]/32,imagetexture.at<cv::Vec3b>(i, j)[1]/32,imagetexture.at<cv::Vec3b>(i, j)[2]/32)++;
		    	}
		    }
		    //Perform histogram intersection
		    float totalimagecolor = 0;
		    float totalimagetexture = 0;
		    for (int i = 0; i < 8; i++)
		    {
		        for (int j = 0; j < 8; j++)
		        {
		            for (int k = 0; k < 8; k++)
		            {
		                totalimagecolor += (imagecolorhistogram.at<float>(i, j, k) < targetcolorhistogram.at<float>(i, j, k) ? imagecolorhistogram.at<float>(i, j, k) : targetcolorhistogram.at<float>(i, j, k)) / (image.rows*image.cols);
		                totalimagetexture += (imagetexturehistogram.at<float>(i, j, k) < targettexturehistogram.at<float>(i, j, k) ? imagetexturehistogram.at<float>(i, j, k) : targettexturehistogram.at<float>(i, j, k)) / (image.rows*image.cols);
		            }
		        }
        	}
        	float colordifference =  1 - totalimagecolor;
        	float texturedifference =  1 - totalimagetexture;
        	//Equal weightage
        	float difference =  0.5*colordifference + 0.5*texturedifference;
        	//Store name and distance metric in hashmap
        	hashmaptexturecolor.insert({targetfolder.ToStdString()+"/"+dp->d_name, difference});
        }
    }
    //Sort hashmap
    sortmaphistogram(hashmaptexturecolor, wxAtoi(textnumberofimages->GetValue()));
}

/*
Use - Function to perform matching for red color toy
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::Red_Toy(wxCommandEvent& WXUNUSED(event))
{
    char buffer[256];
    char dirname[256];
    struct dirent *dp;
    DIR *dirp;
    strcpy(dirname,targetfolder.ToStdString().c_str());
    dirp = opendir(dirname);
    Mat target;
    target = imread(targetpath.ToStdString(), IMREAD_COLOR);
    map<std::string, float> hashmapredtoy;
    Mat targetX, targetY, targettexture;
    //Calculate Sobel magnitude for target
    sobelX3x3( target, targetX );
    sobelY3x3( target, targetY );
    magnitude( targetX, targetY, targettexture );
    convertScaleAbs(targettexture, targettexture);
    while( (dp = readdir(dirp)) != NULL )
    {
        wxWindow::Update();
        Mat image;
        int dim[2] = {32,32};
        //Create histograms
		Mat targetcolorhistogram = Mat::zeros(2, dim, CV_32F);
        Mat targettexturehistogram = Mat::zeros(2, dim, CV_32F);
        Mat imagecolorhistogram = Mat::zeros(2, dim, CV_32F);
        Mat imagetexturehistogram = Mat::zeros(2, dim, CV_32F);
        Mat imageX, imageY, imagetexture;
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") )
        {
            strcpy(buffer, targetfolder.ToStdString().c_str());
      		strcat(buffer, "/");
      		strcat(buffer, dp->d_name);
      		if(strcmp(buffer,targetpath.ToStdString().c_str())==0)
      		{
      			continue;
      		}
			image = imread( buffer, IMREAD_COLOR );
			//Calculate sobel for images
            sobelX3x3( image, imageX );
			sobelY3x3( image, imageY );
			magnitude( imageX, imageY, imagetexture );
			convertScaleAbs(imagetexture, imagetexture);
			//Calculate histograms
			for (int i = (int)(image.rows*0.25); i < (int)(image.rows*0.75); i++)
		    {
		        for (int j = (int)(image.cols*0.4); j < (int)(image.cols*0.6); j++)
		        {
		        	targetcolorhistogram.at<float>(target.at<cv::Vec3b>(i, j)[1]/8,target.at<cv::Vec3b>(i, j)[2]/8)++;
		        	targettexturehistogram.at<float>(targettexture.at<cv::Vec3b>(i, j)[1]/8,targettexture.at<cv::Vec3b>(i, j)[2]/8)++;
		    		imagecolorhistogram.at<float>(image.at<cv::Vec3b>(i, j)[1]/8,image.at<cv::Vec3b>(i, j)[2]/8)++;
		    		imagetexturehistogram.at<float>(imagetexture.at<cv::Vec3b>(i, j)[1]/8,imagetexture.at<cv::Vec3b>(i, j)[2]/8)++;
		    	}
		    }
		    //Perform Histogram intersection
		    float totalimagecolor = 0;
		    float totalimagetexture = 0;
		    for (int i = 0; i < 32; i++)
		    {
		        for (int j = 0; j < 32; j++)
		        {
                    totalimagecolor += min(imagecolorhistogram.at<float>(i, j),targetcolorhistogram.at<float>(i, j))/(0.5*image.rows*0.33*image.cols);
                    totalimagetexture += min(imagetexturehistogram.at<float>(i, j),targettexturehistogram.at<float>(i, j))/(0.5*image.rows*0.33*image.cols);
		        }
        	}
        	float colordifference =  1 - totalimagecolor;
        	float texturedifference =  1 - totalimagetexture;
        	//Weights for both distances
        	float difference = 0.3*colordifference + 0.7*texturedifference;
        	//Store name and distance metric in hashmap
        	hashmapredtoy.insert({targetfolder.ToStdString()+"/"+dp->d_name, difference});
        }
    }
    //Sort hashmap
    sortmaphistogram(hashmapredtoy, wxAtoi(textnumberofimages->GetValue()));
}

/*
Use - Function find Green dumpster matchings
Arguments - wxCommandEvent event
Return - None
*/
void MainFrame::Green_Trash(wxCommandEvent& WXUNUSED(event))
{
    char buffer[256];
    char dirname[256];
    struct dirent *dp;
    DIR *dirp;
    strcpy(dirname,targetfolder.ToStdString().c_str());
    dirp = opendir(dirname);
    Mat target;
    target = imread(targetpath.ToStdString(), IMREAD_COLOR);
    map<std::string, float> hashmapgreentrash;
    Mat targetX, targetY, targettexture;
    //Find Sobel magnitude of target
    sobelX3x3( target, targetX );
    sobelY3x3( target, targetY );
    magnitude( targetX, targetY, targettexture );
    convertScaleAbs(targettexture, targettexture);
    while( (dp = readdir(dirp)) != NULL )
    {
        wxWindow::Update();
        Mat image;
        int dim[2] = {32,32};
        //Create Histograms
		Mat targetcolorhistogram = Mat::zeros(2, dim, CV_32F);
        Mat targettexturehistogram = Mat::zeros(2, dim, CV_32F);
        Mat imagecolorhistogram = Mat::zeros(2, dim, CV_32F);
        Mat imagetexturehistogram = Mat::zeros(2, dim, CV_32F);
        Mat imageX, imageY, imagetexture;
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") )
        {
            strcpy(buffer, targetfolder.ToStdString().c_str());
      		strcat(buffer, "/");
      		strcat(buffer, dp->d_name);
      		if(strcmp(buffer,targetpath.ToStdString().c_str())==0)
      		{
      			continue;
      		}
			image = imread( buffer, IMREAD_COLOR );
			//Find Sobel magnitudes of images
            sobelX3x3( image, imageX );
			sobelY3x3( image, imageY );
			magnitude( imageX, imageY, imagetexture );
			convertScaleAbs(imagetexture, imagetexture);
			//Calculate Histograms
			for (int i = (int)(image.rows*0.33); i < image.rows; i++)
		    {
		        for (int j = 0; j < image.cols; j++)
		        {
		        	targetcolorhistogram.at<float>(target.at<cv::Vec3b>(i, j)[1]/8,target.at<cv::Vec3b>(i, j)[2]/8)++;
		        	targettexturehistogram.at<float>(targettexture.at<cv::Vec3b>(i, j)[1]/8,targettexture.at<cv::Vec3b>(i, j)[2]/8)++;
		    		imagecolorhistogram.at<float>(image.at<cv::Vec3b>(i, j)[1]/8,image.at<cv::Vec3b>(i, j)[2]/8)++;
		    		imagetexturehistogram.at<float>(imagetexture.at<cv::Vec3b>(i, j)[1]/8,imagetexture.at<cv::Vec3b>(i, j)[2]/8)++;
		    	}
		    }
		    //Perform Histogram intersection
		    float totalimagecolor = 0;
		    float totalimagetexture = 0;
		    for (int i = 0; i < 32; i++)
		    {
		        for (int j = 0; j < 32; j++)
		        {
                    totalimagecolor += min(imagecolorhistogram.at<float>(i, j),targetcolorhistogram.at<float>(i, j))/(0.5*image.rows*0.33*image.cols);
                    totalimagetexture += min(imagetexturehistogram.at<float>(i, j),targettexturehistogram.at<float>(i, j))/(0.5*image.rows*0.33*image.cols);
		        }
        	}
        	float colordifference =  1 - totalimagecolor;
        	float texturedifference =  1 - totalimagetexture;
        	//Weights for distances
        	float difference = 0.3*colordifference + 0.7*texturedifference;
        	//Store name and distance metric in hashmap
        	hashmapgreentrash.insert({targetfolder.ToStdString()+"/"+dp->d_name, difference});
        }
    }
    //Sort hashmap
    sortmaphistogram(hashmapgreentrash, wxAtoi(textnumberofimages->GetValue()));
}
