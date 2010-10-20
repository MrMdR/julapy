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
	
	BomAnimation () 
	{
		images		= NULL;
		progress	= 0;
		bBlend		= true;
	};
	
	~BomAnimation () {};
	
	//--------------------------------------
	
	ofxDirList				DIR;
	ofRectangle				rect;
	ofxCvGrayscaleImage*	images;
	ofxCvGrayscaleImage		image0;
	ofxCvGrayscaleImage		image1;
	ofxCvGrayscaleImage		image2;
	int						imageTotal;
	int						imageIndex;
	float					progress;
	bool					bBlend;
	
	//--------------------------------------
	
	void loadAnimationImages ( string dirLocation = "" )
	{
		ofImage image;
		
		imageTotal	= DIR.listDir( dirLocation );
		imageIndex	= 0;
		images		= new ofxCvGrayscaleImage[ imageTotal ];
		
		for( int i=0; i<imageTotal; i++ )
		{
			string fileName;
			fileName = DIR.getName( i );
			
			image.loadImage( dirLocation + "/" +fileName );
			image.setImageType( OF_IMAGE_GRAYSCALE );
			
			if( i == 0 )
			{
				rect.width	= image.getWidth();
				rect.height	= image.getHeight();
			}
			
			images[ i ].allocate( rect.width, rect.height );
			images[ i ].setFromPixels( image.getPixels(), rect.width, rect.height );
			
			image.clear();
		}
		
		image0.allocate( rect.width, rect.height );
		image1.allocate( rect.width, rect.height );
		image2.allocate( rect.width, rect.height );
	}
	
	ofxCvGrayscaleImage* getBomImage ()
	{
		if( bBlend )
		{
			return &image2;
		}
		else
		{
			return &images[ imageIndex ];
		}
	}
	
	void setProgress ( float p )
	{
		progress	= p;
		imageIndex	= ( imageTotal - 1 ) * p;
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
	
	void update ()
	{
		int i, j, t;
		t = imageTotal - 1;
		i = imageIndex;
		j = ofClamp( i + 1, 0, t );
		
		float p1, p2, p3;
		p1 = i / (float)( imageTotal - 1 );
		p2 = j / (float)( imageTotal - 1 );
		p3 = ( progress - p1 ) / ( p2 - p1 );
		
		image0 = images[ i ];
		image1 = images[ j ];
		
		int r0 = ( 1 - p3 ) * 255;
		int r1 = p3 * 255;
		
		image0.convertToRange( 0, r0 );
		image1.convertToRange( 0, r1 );
		
		image2  = image0;
		image2 += image1;
		
		cout << p3 << endl;
	}
	
	void draw ( int x = 0, int y = 0 )
	{
		images[ imageIndex ].draw( x, y );
	}
};