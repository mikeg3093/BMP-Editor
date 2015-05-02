// Mike Grayson
//
// BitMapClass.h file

#pragma once

#include <iostream>
#include <vector>
using namespace std;

class BitMapClass
{
//-----------------------------------------------------------------------------
//rgb: struct to hold an rgb value (red,green,blue)
//used as a base datatype for pixelarray and such
//-----------------------------------------------------------------------------
	struct rgb
	{
		int red;
		int green;
		int blue;
	};
//-----------------------------------------------------------------------------
// BMPHeader: Header of size 38 bytes that holds all the header information
// for a bitmap. Should hold 2 extra bytes (BFTYPE) but it seems that two unsigned
// shorts pads with two extra bytes.
//-----------------------------------------------------------------------------
	struct BMPHeader
	{
		unsigned int   bfSize;           /* Size of file */
		unsigned short bfReserved1;      /* Reserved */
		unsigned short bfReserved2;      /* ... */
		unsigned int   bfOffBits;        /* Offset to bitmap data */
		unsigned int   biSize;           /* Size of info header */
		int            biWidth;          /* Width of image */
		int            biHeight;         /* Height of image */
		unsigned short biPlanes;         /* Number of color planes */
		unsigned short biBitCount;       /* Number of bits per pixel */
		unsigned int   biCompression;    /* Type of compression to use */
		unsigned int   biSizeImage;      /* Size of image data */
		int            biXPelsPerMeter;  /* X pixels per meter */
		int            biYPelsPerMeter;  /* Y pixels per meter */
		unsigned int   biClrUsed;        /* Number of colors used */
		unsigned int   biClrImportant;   /* Number of important colors */
	};
	
private:
	
	//Private functions of BitMapClass (are defined in BitMapClass.cpp)
	int getNbyteInteger(FILE* &fp,int N);
	rgb InvertPixel(rgb P);
	rgb GrayScalePixel(rgb P);//, bool rem);
	rgb GrayScaleBasePixel(rgb P, int rem);
	string RgbToString(rgb P); 
	int ConvertPixel(rgb P);
	void PutPixel(FILE* &fp,rgb P);
	void LoadPixelArray(FILE* &fp,rgb *&PixelArray,int height,int width,int offset,int padding);
	rgb GetPixel(FILE* &fp);
	vector<int> baseremoval;

	
public:
	
	FILE* fp;
	char* PictureName;				//File to read.
	rgb* PixelArray;				//Array of pixels holding actual picture data.
	rgb* OverlayPixelArray;         //Array of pixels holding picture data to be overlayed on an existing PixelArray
	rgb* TwistPixelArray;           //Array of pixels holding picture data that has been twisted from PixelArray
	rgb* CropPixelArray;            //Array of pixels holding picture data that has ben cropped from PixelArray
	int width;						//width of picture in pixels.
	int height;						//height of picture in pixels.
	double tfactor;                 //Twist factor
	int Oheight;                    //OverlayPixelArray height
	int Owidth;                     //OverlayPixelArray width
	int OverlayX;                   //Overlay X Coordinate
	int OverlayY;                   //Overlay Y Coordinate
	int offset;						//number of bytes before picture data (typically 40).
	int bit_depth;					//number of pixels that determine a color (1, 4, 8, or 24).
	int bytes_per_color;			//bit_depth / 8 
	int padding;					//number of bytes appended to each picture row to ensure the row is divisible by four.
	int row_size;                   //Size of a bitmap row in individual rgb units plus padding (width*3+padding)
	
	//Public Functions for BitMapClass (are defined in BitMapClass.cpp)
	BitMapClass(void);
	~BitMapClass(void);
	bool Create(char *name,int width,int height,int bit_depth);
	bool Open(char* PictureName);
	bool InvertColors();
	bool GrayScale();//vector<int>);
	vector<int> GrayScaleBase();
	bool Twist(int tnumber);
	bool Overlay(rgb* OverlayPixelArray, int Oheight, int Owidth, int OverlayX, int OverlayY);
	bool UniqueColors();
	bool UniqueColorsVector();
	bool UniqueColorsMap();
	bool Crop(char* PictureName, rgb* PixelArray,int height,int width, int Cheight, int Cwidth, int CropX, int CropY);
	bool Save(FILE* &fp,rgb *&PixelArray,int height,int width,int offset,int padding);
	
};

