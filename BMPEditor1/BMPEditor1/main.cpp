//---------------------------------------------------------------------
//
// Name:    Mike Grayson
//
// Course:  CS 1053, Section 202, Spring 2012
//
// Purpose: This program is used to create new bitmap images and 
//			edit existing bitmap images. Multiple creations and edits
//          are permitted until the program is exited. New bitmaps are 
//          created as blank (white) .bmp files with a user-specified 
//          height and width. Editing capabilities of existing images 
//			are as follows:
//          Inverting Colors
//          Grayscaling Colors
//          Twisting Images
//          Overlaying Two Images
//          Cropping an Image
//          Counting Unique Colors within an Image
//
// Input:   User-based menu prompted input. Input types include:
//          .bmp File Names
//          Menu Choices/Selections
//          Editing/Creation Specifications     
//
// Output:  A(n) (possibly) altered or new bmp file
//---------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "BitMapClass.h"
//#include "Form1.h"

using namespace std;

#define BF_TYPE 0x4D42 

void main(void)
{   
	//VARIABLE DECLARATIONS
	char FirstMenuChoice=0;         //User Input for 'Create/Load/Exit Menu'
	char BmpEditChoice=0;           //User Input for 'Edit BMP Menu'
	int  NewBmpWidth=0;             //User Input for New BMP Width
	int  NewBmpHeight=0;            //User Input for New BMP Height
	int  Xoverlay=0, Yoverlay=0;    //User Input for Overlay Command X and Y Coordinates                
	int  Cropheight=0, Cropwidth=0; //User Input for Crop Command Height and Width
	int  CropX=0, CropY=0;          //User Input for Crop Command X and Y Coordinates 
	int  twistnumber=0;             //User Input for Twist Number
	
	;            //User Input for a New BMP File Name
	char EditBmpName[50];           //User Input for a BMP File Name that they would like to edit
	char OverlayBmpName[50];        //User Input for a BMP File Name to Overlay 
	char NewBmpName[50];
	BitMapClass BMP;                //Class used for Opening/Creating/Altering User's BMP
	BitMapClass OverlayBMP;         //Class for Overlay Command to handle two BMPs at once
	vector<int> basevector;

	//WELCOME DISPLAY
	cout << "-----------------------------------------------\n";
	cout << "     Welcome to Mike's Bitmap Editor V1.0!     \n";
	cout << "-----------------------------------------------\n\n";

	//If FirstMenuChoice Becomes '3', Exit Program.
	while(FirstMenuChoice!='3'){    

	//CREATE/LOAD/EXIT MENU
	cout << "What would you like to do?\n";
	cout << "1. Create a new bitmap\n";
	cout << "2. Load and edit an existing bitmap\n";
	cout << "3. Exit the program\n";
	cin >> FirstMenuChoice;         //INPUT FirstMenuChoice
	cout << endl;

	//ERROR CHECK (Validates 'Create/Load/Exit Menu' Input)
	while(FirstMenuChoice !='1' && FirstMenuChoice !='2' && FirstMenuChoice !='3')
	{   
		//New Input Prompt
		cout << "Please select 1, 2, or 3.\n";
		cout << "1. Create a new bitmap\n";
		cout << "2. Load and edit an existing bitmap\n";
		cout << "3. Exit the program\n";
		cin >> FirstMenuChoice;      //RE-INPUT FirstMenuChoice
		cout << endl;
	}

	//'CREATE A NEW BITMAP' MENU
	if(FirstMenuChoice == '1')
	{   
		cout << "What will the name of your new bitmap be?\n";
		cout << "(Please inlcude .bmp)\n";
		scanf("%s", NewBmpName);            //INPUT NEW BITMAP NAME
		cout << endl << endl << "What will the width of your new bitmap (in pixels) be?\n";
		cin >> NewBmpWidth;                 //INPUT NEW BITMAP WIDTH
		cout << endl << endl << "What will the height of your new bitmap (in pixels) be?\n";
		cin >> NewBmpHeight;                //INPUT NEW BITMAP HEIGHT

		//Create a new bmp with user-defined parameters (BitMapClass Function Call)
		BMP.Create(NewBmpName, NewBmpWidth, NewBmpHeight, 24);
		cout << endl << endl << "****You're new bitmap has been created!****\n\n";
		//LOOP TO CREATE/LOAD/EXIT MENU
	}

	//'EDIT AN EXISTING BITMAP' MENUS
	else if(FirstMenuChoice == '2')
	{   
		//EDIT BMP NAME INPUT MENU
		cout << "What is the name of the bitmap you would like to edit?\n";
		cout << "(Please include the .bmp)\n";
		scanf("%s", EditBmpName);           //INPUT EDIT BITMAP NAME
		//ERROR CHECK (Validates EditBmpName)
		while(!fopen(EditBmpName, "r+b"))
		{   
			//New input prompt
			cout << endl;
			cout << "FILE NOT FOUND\n\n";
			cout << "Please re-enter the file name.\n";
			cout << "(Please include .bmp)\n";
			scanf("%s", EditBmpName);       //RE-INPUT EDIT BITMAP NAME
		}
		//Open User Input Bitmap for Editing (BitMapClass Function Call)
		BMP.Open(EditBmpName);
		cout << endl << endl;

		//EDIT BITMAP MENU
		cout << "What would you like to do to this image?" << endl;
		cout << "1. Invert the colors in the image" << endl;
		cout << "2. Grayscale the image" << endl;
		cout << "3. Twist the image" << endl;
		cout << "4. Overlay another image on top of this one" << endl;
		cout << "5. Crop the image" << endl;
		cout << "6. Count the number of unique colors within the image" << endl;
		cout << "7. Grayscalebase" << endl;
		cin >> BmpEditChoice;               //INPUT BmpEditChoice
		cout << endl << endl;
		
		//ERROR CHECK (Validates BmpEditChoice)
		while(BmpEditChoice < '1' || BmpEditChoice > '7')
		{   
			//New Input Prompt
			cout << "Please choose 1 - 6\n";
			cin >> BmpEditChoice;           //RE-INPUT BmpEditChoice
			cout << endl << endl;
		}

		//SWITCH FOR DETERMINING WHICH EDIT TO CARRYOUT
		switch(BmpEditChoice)
		{
		case '1':
			//Invert colors (BitMapClass Function Call)
			BMP.InvertColors();
			break;
		case '2':
			//Grayscale (BitMapClass Function Call)
			BMP.GrayScale();//basevector);
			break;
		case '3':
			//Twist
			//Prompt for Twistnumber
			cout << "Enter a Twist Factor (1-10)\n";
			cout << "Caution: A larger twist factor will take longer\n";
			cin >> twistnumber;
			//ERROR CHECK (Validate twistnumber)
			while(twistnumber > 10 || twistnumber < 0)
			{
				cout << "\nNice try\n";
				cout << "Enter a Twist Factor (1-10)\n";
				cout << "Caution: A larger twist factor will take longer\n";
				cin >> twistnumber;          //RE-INPUT twistnumber
			}
			//Twist (BitMapClass Function Call)
			BMP.Twist(twistnumber);
			break;
		case '4':
			//Overlay
			//Prompt for OverlayBmpName
			cout << "What is the name of the picture that you would like to overlay?" << endl;
			cout << "(Please include .bmp)" << endl;
			scanf("%s", OverlayBmpName);        //INPUT OverlayBmpName
			//ERROR CHECK (Validates OverlayBmpName)
			while(!fopen(OverlayBmpName, "r+b"))
			{   //New input prompt
				cout << endl;
				cout << "FILE NOT FOUND" << endl << endl;
				cout << "Please re-enter the file name." << endl;
				cout << "(Please include .bmp)" << endl;
				scanf("%s", OverlayBmpName);       //RE-INPUT OverlayBmpName
			}
			OverlayBMP.Open(OverlayBmpName);   //Open OverlayBmpName
			cout << endl << endl << "At what X coordinate would you like to overlay the image at?" << endl;
			cin >> Xoverlay;                   //INPUT Xoverlay
			cout << endl << endl << "At what Y coordinate would you like to overlay the image at?" << endl;
			cin >> Yoverlay;                   //INPUT Yoverlay
			cout << endl;
            //ERROR CHECK (Validates Successful Overlay)
			//if the overlay is possible....
			if(Xoverlay+OverlayBMP.width<BMP.width && Yoverlay+OverlayBMP.height<BMP.height)
			{
				//....Overlay w/ user defined parameters (BitMapClass Function Call)
				BMP.Overlay(OverlayBMP.PixelArray, OverlayBMP.height, OverlayBMP.width, Xoverlay, Yoverlay);
			}
			else//Invalid input case
			{   
				cout << "****ERROR: YOUR OVERLAY DID NOT FIT****" << endl << endl;
			}
			break;
		case '5':
			//Crop
			//Prompt for Crop Area and Location
			cout << "At what X coordinate would you like to begin the crop?" << endl;
			cin >> CropX;                      //INPUT CropX
			cout << "At what Y coordinate would you like to begin the crop?" << endl;
			cin >> CropY;                      //INPUT CropY
			cout << "How many pixels wide would you like the crop to be?" << endl;
			cin >> Cropwidth;                  //INPUT Cropwidth
			cout << "How many pixels wide would you like the crop to be?" << endl;
			cin >> Cropheight;                 //INPUT Cropheight
			cout << endl;
			//ERROR CHECK (Validates Successful crop)
			//if crop is possible....
			if(CropX+Cropwidth<BMP.width && CropY+Cropheight<BMP.height)
			{ 
				//....Crop w/ user defined parameters (BitMapClass Function Call)
				BMP.Crop(EditBmpName, BMP.PixelArray, BMP.height, BMP.width, Cropheight, Cropwidth, CropX, CropY);
			}
			else//Invalid input case
			{
				cout << "****ERROR: YOUR CROP FAILED****" << endl << endl;
			}
			break;
		case '6':
			//Count Unique Colors
			//BMP.UniqueColors(BMP.PixelArray, BMP.height, BMP.width);
			//BMP.UniqueColorsVector(BMP.PixelArray, BMP.height, BMP.width);
			BMP.UniqueColorsMap();
			break;
		case '7':
			//Grayscale (BitMapClass Function Call)
			basevector = BMP.GrayScaleBase();
			break;
		default:
			break;
		}
	}
	//LOOP TO CREATE/LOAD/EXIT MENU
	}
}
