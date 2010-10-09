/*
 *  BomAnimation.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 9/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxOpenCv.h"
#include "ofxDirList.h"

class BomAnimation
{
	
public:
	
	 BomAnimation () {};
	~BomAnimation () {};
	
	//--------------------------------------
	
	ofxDirList				DIR;
	ofxCvGrayscaleImage*	images;
	int						imageTotal;
	int						imageIndex;
	
	
	//--------------------------------------
	
	void loadAnimationImages ( string dirLocation = "" )
	{
		ofImage image;
		
		imageTotal	= DIR.listDir( dirLocation );
		images		= new ofxCvGrayscaleImage[ imageTotal ];
		
		for( int i=0; i<imageTotal; i++ )
		{
			string fileName;
			fileName = DIR.getName( i );
			
			image.loadImage( dirLocation + "/" +fileName );
			image.setImageType( OF_IMAGE_GRAYSCALE );
			
			int w, h;
			w = image.getWidth();
			h = image.getHeight();
			
			images[ i ].allocate( w, h );
			images[ i ].setFromPixels( image.getPixels(), w, h );
			
			image.clear();
		}
		
		imageIndex = 0;
	}
	
	ofxCvGrayscaleImage* getBomImage ()
	{
		return &images[ imageIndex ];
	}
	
	void setProgress ( float p )
	{
		imageIndex = ( imageTotal - 1 ) * p;
	}
	
	void nextFrame ()
	{
		if( ++imageIndex > imageTotal - 1 )
		{
			imageIndex = 0;
		}
	}
	
	void prevFrame ()
	{
		if( --imageIndex < 0 )
		{
			imageIndex = imageTotal - 1;
		}
	}
	
	void draw ( int x = 0, int y = 0 )
	{
		images[ imageIndex ].draw( x, y );
	}
};