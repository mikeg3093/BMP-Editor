// Mike Grayson
// 
// BitMapClass.cpp file

#include "BitMapClass.h"
#include <string>
#include <vector>
#include <map>
#define C_PI 3.141592653589793238462643383279502884197169399375

//----------------------------------------------------------------------------
// BitMapClass: Constructs a BitMapClass Class
// @Param: void
// @Returns: void
//----------------------------------------------------------------------------
BitMapClass::BitMapClass(void)              // BitMapClass Constructor
{
	PictureName = NULL;                     //.bmp file name
	PixelArray = NULL;                      //.bmp picture data
	width = 0;								//width of picture in pixels.
	height = 0;								//height of picture in pixels.
	offset = 0;								//number of bytes before picture data (typically 40).
	bit_depth = 0;							//number of pixels that determine a color (1, 4, 8, or 24).
	bytes_per_color = 0;					//bit_depth / 8 
	padding = 0;							//number of bytes appended to each picture row to ensure the row is divisible by four.
	row_size = 0;	
}
//----------------------------------------------------------------------------
// getNbyteInteger: Reads N bytes from fp
// @Param: FILE* fp - file to read from
// @Param: int N - number of bytes to read
// @Returns: int - N byte value read from file
//----------------------------------------------------------------------------
int BitMapClass::getNbyteInteger(FILE* &fp,int N)
{
   int byte=0;
   int value=0;
   int shift=0;

   for(int i=0;i<N;i++)
   {
	   byte = fgetc(fp);
	   value |= byte << shift;
	   shift = shift + 8;
   }
   return value;
}
//-----------------------------------------------------------------------------
// GetPixel: Reads a single rgb value from a file (3 bytes at a time, 1 for each coloe).
// @Param: FILE* fp - InputFile
// @Returns: rgb - a pixel (with 3 colors)
//-----------------------------------------------------------------------------
BitMapClass::rgb BitMapClass::GetPixel(FILE* &fp)
{   
	rgb Pixel;
	Pixel.blue = fgetc(fp);
	Pixel.green = fgetc(fp);
	Pixel.red = fgetc(fp);
	return Pixel;
}
//----------------------------------------------------------------------------
// InvertPixel: Inverts the colors of an rgb type.
// @Param: rgb P - Pixel to invert
// @Returns: rgb - Inverted pixel
//----------------------------------------------------------------------------
BitMapClass::rgb BitMapClass::InvertPixel(rgb P)
{
	P.blue = 255 - P.blue;
	P.red = 255 - P.red;
	P.green = 255 - P.green;
	return P;
}
//----------------------------------------------------------------------------
// PutPixel: Writes a single rgb (pixel) to a file.
// @Param: FILE* fp - output file
// @Param: rgb P - pixel to write to file
// @Returns: VOID
//----------------------------------------------------------------------------
void BitMapClass::PutPixel(FILE* &fp,rgb P)
{
	fputc(P.blue,fp);
	fputc(P.green,fp);
	fputc(P.red,fp);
}
//----------------------------------------------------------------------------
// GrayScalePixel: Grayscale a pixel.
// @Param: rgb P - Pixel to grayscale
// @Returns: rgb - grayed pixel
//----------------------------------------------------------------------------
//gray = (0.299*r + 0.587*g + 0.114*b);
BitMapClass::rgb BitMapClass::GrayScalePixel(rgb P)//, bool rem)
{
	//int Gray = P.red*0.299+P.green*0.587+P.blue*0.114;
	//if(P.blue < 40 && P.red < 40 && P.green < 70)
	if(P.green < 32)// && rem == true)
	{
		P.blue = 0;
		P.red = 0;
		P.green = 0;
	}
	else 
	{
		P.blue = 255;
		P.red = 255;
		P.green = 255;
	}
	return P;
}

BitMapClass::rgb BitMapClass::GrayScaleBasePixel(rgb P, int rem)
{
	//int Gray = P.red*0.299+P.green*0.587+P.blue*0.114;
	//if(P.blue < 40 && P.red < 40 && P.green < 70)
	if(P.green < 32)
	{
		P.blue = 0;
		P.red = 0;
		P.green = 0;
		baseremoval.insert(baseremoval.end(), rem);
	}
	else 
	{
		P.blue = 255;
		P.red = 255;
		P.green = 255;
	}
	return P;
}

//-----------------------------------------------------------------------------
// Create: Creates a bitmap from nothing.
// @Param: char *name - name of the file.
// @Param: int width - width of image
// @Param: int height - height of image
// @Param: int bit_depth - number of bits to represent color (24 bit default)
// @Returns: void
//-----------------------------------------------------------------------------
bool BitMapClass::Create(char *name,int width,int height,int bit_depth)
{
	fp = fopen(name, "wb");	//Open file for reading (r) and writing (+) and read it in binary format (b)
	BMPHeader H;
	char BMP1 = 0x42;       
	char BMP2 = 0x4D;		
	int ictr = 0;
	int bytes_per_color = bit_depth/8;
	int file_size=0;
	int i=0;
	int row_size = ((width * bytes_per_color) + 3) & (~3);	//number of bytes for each row padding included
	int padding = row_size - (width * bytes_per_color);		//what is each row padded with. Needed for reading and writing the file.
	char *row_data = new char[row_size];
	for(i=0;i<row_size;i++)
	{
		row_data[i]=rand()%255;
	}
	//Update BMP Header Values
	H.bfSize = row_size*height+sizeof(BMPHeader)+2;
	H.bfReserved1=0;
	H.bfReserved2=0;
	H.bfOffBits=0x36;
	H.biSize=0x28;
	H.biWidth=width;
	H.biHeight=height;
	H.biPlanes=1;
	H.biBitCount=bit_depth;
	H.biCompression=0;
	H.biSizeImage=width*height*bytes_per_color;
	H.biXPelsPerMeter=0;
	H.biYPelsPerMeter=0;
	H.biClrUsed=0;
	H.biClrImportant=0;
	//Write BMP Header Values
	fwrite(&BMP1,1,1,fp);
	fwrite(&BMP2,1,1,fp);
	fwrite(&H,sizeof(H),1,fp);
	//Write Pixel Data by Rows
	for(int i=0;i<height;i++)
	{
		fwrite(row_data,row_size,1,fp);
	}
	fclose(fp);
	return true;
}
//---------------------------------------------------------------------------
// Open: Opens a BMP file for reading and writing
// @Param: char* PictureName - Name of the file to be opened
// @Returns: true or false
//---------------------------------------------------------------------------
bool BitMapClass::Open(char* PictureName)
{
	fp = fopen(PictureName, "r+b");	  //Open file for reading (r) and writing (+) and read it in binary format (b)
	
	fseek(fp,0xA,SEEK_SET);					//Put the file pointer at the byte holding the offset
	offset = getNbyteInteger(fp,4);			//Read in the offset

	fseek(fp, 0x12, SEEK_SET);				//Put the file pointer at the byte holding the width
	width = getNbyteInteger(fp,4);			//Read in the width
	height = getNbyteInteger(fp,4);			//Read in the height

	fseek(fp,0x1C,SEEK_SET);				//Put the file pointer at the byte holding the bit_depth
	bit_depth = getNbyteInteger(fp,4);		//Read in the bit_depth

	bytes_per_color = bit_depth/8;			//calculate number of bytes needed to store a single color for a pixel (8 bits per byte)

	row_size = ((width * bytes_per_color) + 3) & (~3);	//number of bytes for each row padding included
	padding = row_size - (width * bytes_per_color);		//what is each row padded with. Needed for reading and writing the file. 

	PixelArray = new rgb[width*height];		//Dynamically allocate an array just big enough to hold the current picture

	LoadPixelArray(fp, PixelArray, height, width, offset, padding);
	return true;
}
//----------------------------------------------------------------------------
// Load a 1D array of pixels with all the color data from file fp.
// @Param: FILE* fp -  input file pointer
// @Param: rgb* PixelArray - 1D array of bmp colors
// @Param: int height - height of picture
// @Param: int width - width of picture
// @Param: int offset - Number of bytes in file until actual picture data
// @Param: int padding - number of bytes at end of each row of data to make row total divisible by four
// @Returns: VOID
//----------------------------------------------------------------------------
void BitMapClass::LoadPixelArray(FILE* &fp,rgb *&PixelArray,int height,int width,int offset,int padding)
{
	int i;

 	fseek(fp,offset,SEEK_SET);			//Place file pointer at beginning of picture data
	for(i=0;i<width*height;i++){		//Loop width*height times
		if(i>0 && i%width==0){			//If we are at the end of the row, skip the padding
			fseek(fp,padding,SEEK_CUR);
		}
		PixelArray[i] = GetPixel(fp);	//Do actual reading of color pixel from file
	}
}
//----------------------------------------------------------------------------
// InvertColors: Inverts the color of each pixel of a bmp.
// @Returns: VOID
//----------------------------------------------------------------------------
bool BitMapClass::InvertColors(){
	for(int i=0;i<width*height;i++)
	{                
		// Invert each idividual pixel in PixelArray
		PixelArray[i] = InvertPixel(PixelArray[i]);
	}
	Save(fp, PixelArray, height, width, offset, padding);
	cout << "****Your image's colors are now inverted!****\n\n\n";
	return true;
}
//----------------------------------------------------------------------------
// GrayScale: Converts image to grayscale (irreversible).
// @Returns: VOID
//----------------------------------------------------------------------------
bool BitMapClass::GrayScale(){//vector<int> basevector){

	int n = 0;
	for(int i=0;i<width*height;i++)
	{

		//if(basevector.at(n) == i)
		//{
			//n++;
			PixelArray[i] = GrayScalePixel(PixelArray[i]);//, false);
		//}
		//else
			//PixelArray[i] = GrayScalePixel(PixelArray[i], true);
		//Grayscale each individual pixel in PixelArray
	}
	Save(fp, PixelArray, height, width, offset, padding);
	cout << "****Your image has been grayscaled!****\n\n\n";
	return true;
}

vector<int> BitMapClass::GrayScaleBase(){

	for(int i=0;i<width*height;i++)
	{
		//Grayscale each individual pixel in PixelArray
		PixelArray[i] = GrayScaleBasePixel(PixelArray[i], i);
	}
	Save(fp, PixelArray, height, width, offset, padding);
	cout << "****Your image has been grayscaled!****\n\n\n";
	return baseremoval;
}
//----------------------------------------------------------------------------
// ****The following code is a bitmap twist function from supercomputingblog.com.
// Some variable names have been changed for integration purposes. 
//
// Twist: Twists a BMPs pixels to the left (irreversible)
// @Param: rgb* PixelArray - BMP picture data to be twisted
// @Param: int height - height of picture
// @Param: int width - width of picture
// @Returns: true or false
//----------------------------------------------------------------------------
bool BitMapClass::Twist(int tnumber)
{
	for(int i = 0; i < tnumber; i++)
	{
	double cX = (double)width/2.0f;
	double cY = (double)height/2.0f;
	TwistPixelArray = new rgb[width*height];
	tfactor=.01;
	for(int i=0;i<width*height;i++)
	{
		TwistPixelArray[i] = PixelArray[i];
	}
	
	#pragma omp parallel for
	for (int i=0; i < height; i++)
	{
		double relY = cY-i;
		for (int j=0; j < width; j++)
		{
			double relX = j-cX;
			// relX and relY are points in our UV space
			// Calculate the angle our points are relative to UV origin. Everything is in radians.
			double originalAngle;
			if (relX != 0)
			{
				originalAngle = atan(abs(relY)/abs(relX));
				if ( relX > 0 && relY < 0) originalAngle = 2.0f*C_PI - originalAngle;
				else if (relX <= 0 && relY >=0) originalAngle = C_PI-originalAngle;
				else if (relX <=0 && relY <0) originalAngle += C_PI;
			}
			else
			{
				// Take care of rare special case
				if (relY >= 0) originalAngle = 0.5f * C_PI;

				else originalAngle = 1.5f * C_PI;
			}		
			// Calculate the distance from the center of the UV using pythagorean distance
			double radius = sqrt(relX*relX + relY*relY);
			// Use any equation we want to determine how much to rotate image by
			//double newAngle = originalAngle + factor*radius;	// a progressive twist
			double newAngle = originalAngle + 1/(tfactor*radius+(4.0f/C_PI));
			// Transform source UV coordinates back into bitmap coordinates
			int srcX = (int)(floor(radius * cos(newAngle)+0.5f));
			int srcY = (int)(floor(radius * sin(newAngle)+0.5f));
			srcX += cX;
			srcY += cY;
			srcY = height - srcY;
			// Clamp the source to legal image pixel
			if (srcX < 0) srcX = 0;
			else if (srcX >= width) srcX = width-1;
			if (srcY < 0) srcY = 0;
			else if (srcY >= height) srcY = height-1;
			// Set the pixel color
			PixelArray[i*width + j] = TwistPixelArray[srcY*width + srcX];
		}
	}
	//Save twisted pixel data to .bmp file
	Save(fp, PixelArray, height, width, offset, padding);
	}
	cout << "\n****Your image is now twisted!****\n\n\n";
	return true;
}
//----------------------------------------------------------------------------
// Overlay: Takes a BMP and places another BMP on top of it at a user defined 
//          location (irreversible). WARNING: Will fail and terminate program 
//          if the second image is too big or the overlay causes the second 
//          image to run off of the edge of the first image. 
//          EDIT: Program is error-checked in main.cpp to prevent WARNING case
// @Param: rgb* OverlayPixelArray - Overlay BMP Picture Data
// @Param: int Oheight - height of overlay picture
// @Param: int Owidth - width of overlay picture
// @Param: int OverlayX - user defined overlay X point
// @Param: int OverlayY - user defined overlay Y point
// @Returns: true or false
//-----------------------------------------------------------------------------
bool BitMapClass::Overlay(rgb* OverlayPixelArray, int Oheight, int Owidth, int OverlayX, int OverlayY)
{
	double startpt = width*OverlayY;      // Determine a start point to begin converting picture data from OverlayPixelArray to PixelArray
	startpt = startpt + OverlayX;					 
	int OPActr=0;					      //OverlayPixelArray counter

	for(int i=startpt;i<startpt+width*Oheight;i+=width)          //Converting OverlayPixelArray picture data to PixelArray picture data 
	{                                                            //at the user-defined point
		for(int j = i;j<i+Owidth;j++)
		{
			PixelArray[j] = OverlayPixelArray[OPActr];
			OPActr++;
		}
	}
	Save(fp, PixelArray, height, width, offset, padding);        //Saving new PixelArray data w/ overlayed image 
	cout << "****Your image now has another image on top of it!****\n\n\n";
	return true;
}
//----------------------------------------------------------------------------
// Crop: Crops a BMP at a user defined location for a user defined height and
//       width (irreversible). Does so by creating a new BMP of size crop area 
//       and writing the cropped BMP over the original. WARNING: Will fail and terminate 
//       program if the cropping area falls outside of the BMPs picture data.
//       EDIT: Program is error-checked in main.cpp to prevent WARNING
// @Param: char* PictureName - Name of BMP file to be cropped
// @Param: rgb* PixelArray - BMP Picture Data of file to be cropped
// @Param: int height - height of picture
// @Param: int width - width of picture
// @Param: int Cheight - height of crop area
// @Param: int Cwidth - width of crop area
// @Param: int OverlayX - user defined crop X point
// @Param: int OverlayY - user defined crop Y point
// @Returns: true or false
//-----------------------------------------------------------------------------
bool BitMapClass::Crop(char* PictureName, rgb* PixelArray, int height,int width, int Cheight, int Cwidth, int CropX, int CropY)
{
	int startpt = width*CropY;                          // Determine a start point to begin copying picture data 
	startpt+=CropX;                                     // into a new rgb array
	int CPActr=0;										// CropPixelArray counter
    CropPixelArray = new rgb[Cheight*Cwidth];           // New array to hold picture data of cropped area

	for(int i = startpt;i<startpt+width*Cheight;i+=width)  //Copying cropped area into CropPixelArray
	{
		for(int j = i;j<i+Cwidth;j++)
		{
			CropPixelArray[CPActr] = PixelArray[j];
			CPActr++;
		}
	}
	Create(PictureName,Cwidth,Cheight,24);                 //Creating a new (empty) BMP of size crop area that overwrites 
														   //the original uncropped BMP
	Open(PictureName);	                                   //Opening that new BMP
	Save(fp, CropPixelArray, Cheight, Cwidth, offset, padding);  //Saving CropPixelArray picture data into the empty BMP
															     //that we just created for it!
	cout << "****Your image is now cropped!****\n\n\n";
	return true;
}
//----------------------------------------------------------------------------
// UniqueColors: Counts and prints the number of unique colors in PixelArray, 
//               which is holding Bitmap pixel data. 
//               ****Can only count up to 50,000 unique colors****
// 
// @Returns: true or false
//-----------------------------------------------------------------------------
bool BitMapClass:: UniqueColors()
{
	int colornum = 0;                        //Unique color counter
	rgb UniqueColorArray[50000];             //Array for holding colors that have already been counted

	for(int i = 0; i < height*width; i++)
	{
		for(int j = 0; j < colornum+1; j++)  
		{   
			//Compares a pixel color to colors that have already been counted 
			if(PixelArray[i].blue == UniqueColorArray[j].blue && 
			   PixelArray[i].green == UniqueColorArray[j].green && 
			   PixelArray[i].red == UniqueColorArray[j].red)
			{   
				//If a color has been counted, exit compare loop. colornum does not increment
				j = colornum+1; 
			}
			//If color could not be found...
			if(j == colornum)
			{
				//Long compute time message, many colors instance
				if(colornum == 10000)
				{
					cout << "Geez this has alot of colors....\n";
				}
				//Long compute time message, many colors instance
				if(colornum == 20000)
				{
					cout << "This is going to take awhile....\n";
				}
				//Add color to UniqueColorArray...
				UniqueColorArray[colornum] = PixelArray[i];
				//and increment colornum
				colornum++;
				j++;
			}
		}
		//If the array is about to overflow....
		if(colornum >= 50000)
		{
			//UniqueColorArray overflow message 
			cout << "\n****I can't count that high!****\n\n\n";
			i = height*width; //loop fail/exit
		}
	}
	//Succesful count message
	if(colornum<50000)
	{
		cout << "\n**** Your image has " << colornum << " colors!****\n\n\n";
	}
	return true;
}

//----------------------------------------------------------------------------
// UniqueColors: Counts and prints the number of unique colors in PixelArray, 
//               which is holding Bitmap pixel data. 
//
// @Returns: true or false
//-----------------------------------------------------------------------------

bool BitMapClass::UniqueColorsVector()
{
	int pixelnumber;									// Integer color number of an rgb pixel
	int j;												// for loop int; loops UniqueColorVector.size times
	vector<int> UniqueColorVector;						// Vector holding pixelnumber(s)
	
	for(int i = 0; i < height*width; i++)				// Loops through individual pixels in PixelArray
	{
		pixelnumber = ConvertPixel(PixelArray[i]);		// Converts rgb pixel to int value
			
		for(j = 0; j < UniqueColorVector.size(); j++)	// Loops vector size times to...
		{
			if(pixelnumber == UniqueColorVector[j])		// ... compare pixelnumber to other pixelnumbers in vector...
			{
				j = UniqueColorVector.size();			// exits if color was found in vector
			}
		}

		if(j == UniqueColorVector.size())				// If pixelnumber was not found in vector...
		{
			UniqueColorVector.insert(UniqueColorVector.end(), pixelnumber);  // ...insert pixelnumber into vector
		}
	}

	cout << "\n**** Your image has " << UniqueColorVector.size() << " colors! ****\n\n\n";  // output vector size (number of unique colors)
	return true;
}

//----------------------------------------------------------------------------
// UniqueColorsMap: Counts and prints the number of unique colors and how
//					many times each color appearsin PixelArray, 
//					which is holding Bitmap pixel data. 
// 
// @Returns: true or false
//-----------------------------------------------------------------------------
bool BitMapClass::UniqueColorsMap()
{
	string pixelname;									//Name of color "r:g:b:"
	map<string, int> UniqueColorMap;					//Map<pixelname, how may times pixelname appears>
	map<string, int>::iterator it;						//Iterator for printing 
	
	for(int i = 0; i < height*width; i++)				// Loops through individual pixels in PixelArray
	{
		pixelname = RgbToString(PixelArray[i]);			//Convert rgb to string
		
		 if (UniqueColorMap.count(pixelname)>0)			//If pixelname is already in map...
		 {
			 UniqueColorMap[pixelname]++;				//...increment number of times it has appeared...
		 }
		 else
		 {
			 UniqueColorMap.insert(pair<string,int>(pixelname, 1)); //...otherwise add pixelname to the map
		 }
	}

	for (it=UniqueColorMap.begin() ; it != UniqueColorMap.end(); it++) //Print pixelnames and number of times they appear
	{
     cout << it->first << " => " << it->second << endl;
	}

	cout << "\n**** Your image has " << UniqueColorMap.size() << " colors! ****\n\n\n";  //Output map size (number of unique colors)
	return true;
}
string BitMapClass::RgbToString(rgb P) 
{ 
	string temp="";
	string returnvalue="";
	int Colors[] = {P.red,P.green,P.blue};
	
	for(int i=2;i>=0;i--)
	{	
		for(int j=0; j<3; j++)
			{
				temp+=Colors[i]%10+48;
				Colors[i]/=10;
			}
	}
	
	for (int i=1;i<10;i++)
	{	
		if(i==4 || i==7)
		returnvalue+=":";
		returnvalue+=temp[9-i];
	}
	
	return returnvalue;
}

int BitMapClass::ConvertPixel(rgb P)
{
	int pixelnum;
	P.red = P.red*65536;
	P.blue = P.blue*256;
	pixelnum = P.red + P.blue + P.green;
	return pixelnum;
}


//----------------------------------------------------------------------------
// Takes the rgb data from the pixel array, and writes it back to the original file 
// saving all the changes made while the bmp was loaded. If this function isn't called
// any changes will be lost when the program closes.
// @Param: FILE* fp -  input file pointer
// @Param: rgb* PixelArray - 1D array of bmp colors
// @Param: int height - height of picture
// @Param: int width - width of picture
// @Param: int offset - Number of bytes in file until actual picture data
// @Param: int padding - number of bytes at end of each row of data to make row total divisible by four
// @Returns: true or false
//----------------------------------------------------------------------------
bool BitMapClass::Save(FILE* &fp,rgb *&PixelArray,int height,int width,int offset,int padding){
	int i;

 	fseek(fp,offset,SEEK_SET);			//Place file pointer at beginning of picture data
	for(i=0;i<width*height;i++){
		if(i>0 && i%width==0){			//If were at the end of a row, skip the padding
			fseek(fp,padding,SEEK_CUR);
		}
		PutPixel(fp,PixelArray[i]);		//Write pixel to file
	}
	return true;
}
//Destuctor
BitMapClass::~BitMapClass(void)
{
}
