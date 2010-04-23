/*
 *  ofxTesseract.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 21/04/10.
 *  
 *	Code based on a iphone example by Nolan Brown.
 *	http://github.com/nolanbrown/Tesseract-iPhone-Demo
 *	
 *	Also this,
 *	http://robertcarlsen.net/dev/pocket-ocr
 *
 *	KNOWN ISSUES :
 *		- setting datapath in SimpleInit does not work.
 *		- tessdata needs to be copied to /usr/local/share/tessdata/
 *
 *
 */

#include "ofxTesseract.h"

ofxTesseract :: ofxTesseract()
{
	string dataPath;
	dataPath = ofToDataPath( "tessdata", false );
	
	cout << "ofxTesseract :: loading tessdata from - " << dataPath << endl;
	
	char *dataPathChar;
	dataPathChar = new char[ dataPath.length() + 1 ];
	
	strcpy( dataPathChar, dataPath.c_str() );
	
	tess = new TessBaseAPI();
	
	tess->SimpleInit			// LK NOTE : assigning datapath not working, defaulting to /usr/local/share/tessdata/ - had to copy the files into that dir and it works.
	(
		dataPathChar,			// datapath		- Path to tessdata-no ending.
		"eng",					// language		- ISO 639-3 string or NULL.
		false					// numeric_mode	- 
	);
	
	bUseRoi = false;
}

void ofxTesseract :: setROI( const ofRectangle &rect )
{
	roi.x		= rect.x;
	roi.y		= rect.y;
	roi.width	= rect.width;
	roi.height	= rect.height;
	
	bUseRoi		= true;
}

void ofxTesseract :: resetROI ()
{
	bUseRoi		= false;
}

string ofxTesseract :: findTextInImage( ofImage &image )
{
	const unsigned char* imageData;
	int imageBytesPerPixel;
	int imageBytesPerLine;
	
	imageData			= image.getPixels();
	imageBytesPerPixel	= image.bpp / 8;
	imageBytesPerLine	= imageBytesPerPixel * image.width;
	
	char* text = tess->TesseractRect
	(
		imageData,
		imageBytesPerPixel,
		imageBytesPerLine, 
		( bUseRoi ) ? roi.x			: 0,
		( bUseRoi ) ? roi.y			: 0,
		( bUseRoi ) ? roi.width		: image.width,
		( bUseRoi ) ? roi.height	: image.height
	);
	
	string str;
	str.assign( text );
	
	return str;
}

string ofxTesseract :: findTextInImage( const unsigned char* pixels, int width, int height, int type )
{
	const unsigned char* imageData;
	int imageBytesPerPixel;
	int imageBytesPerLine;
	
	int bpp = 0;
	
	if( type == OF_IMAGE_GRAYSCALE )
	{
		bpp = 8;
	}
	else if( type == OF_IMAGE_COLOR )
	{
		bpp = 24;
	}
	else if( type == OF_IMAGE_COLOR_ALPHA )
	{
		bpp = 32;
	}
	
	if( bpp == 0 )
	{
		cout << "findTextInImage() did not specify a valid image type" << endl;
		
		return "";
	}
	
	imageData			= pixels;
	imageBytesPerPixel	= bpp / 8;
	imageBytesPerLine	= imageBytesPerPixel * width;
	
	char* text = tess->TesseractRect
	(
		imageData,
		imageBytesPerPixel,
		imageBytesPerLine, 
		( bUseRoi ) ? roi.x			: 0,
		( bUseRoi ) ? roi.y			: 0,
		( bUseRoi ) ? roi.width		: width,
		( bUseRoi ) ? roi.height	: height
	);
	
	string str;
	str.assign( text );
	
	return str;
}