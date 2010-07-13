/*
 *  Rocks.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 13/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Rocks.h"

Rocks :: Rocks ()
{
	//
}

Rocks :: ~Rocks()
{
	//
}

void Rocks :: setup ()
{
	rocksImage.loadImage( "underbelly_rocks.jpg" );
	
	rocksColorImage.allocate( rocksImage.width, rocksImage.height );
	rocksGrayImage.allocate( rocksImage.width, rocksImage.height );
	
	threshold = 127;
}

void Rocks :: update()
{
	rocksColorImage.setFromPixels( rocksImage.getPixels(), rocksImage.width, rocksImage.height );
	rocksGrayImage.setFromColorImage( rocksColorImage );
	rocksGrayImage.threshold( threshold, true );

	int blobsFound;
	blobsFound = findBlobs();
	
	if( blobsFound > 0 )
	{
		blobUtil.downSampleBlobs( contourFinder.blobs, blobs, 10 );
	}
}

int  Rocks :: findBlobs ()
{
	int maxArea;
	maxArea = rocksGrayImage.width * rocksGrayImage.height;
	
	float cfMinArea;
	cfMinArea = 0.001;
	
	int maxContoursToFind;
	maxContoursToFind = 20;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		rocksGrayImage,					// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		false,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void Rocks :: draw ()
{
	drawImage();
	drawBlobs();
	drawDownsampledBlobs();
}

void Rocks :: drawImage ()
{
	ofFill();
	ofSetColor( 0xFFFFFF );
	rocksGrayImage.draw( 0, 0 );
}

void Rocks :: drawBlobs ()
{
	contourFinder.draw();
}

void Rocks :: drawDownsampledBlobs ()
{
	for( int i=0; i<blobs.size(); i++ )
	{
		ofxCvBlob &blob = blobs[ i ];
		
		int t = blob.pts.size();
		
		for( int j=0; j<t; j++ )
		{
			ofPoint &p1 = blob.pts[ j ];
			ofPoint &p2 = blob.pts[ ( j + 1 ) % t ];
			
			ofNoFill();
			ofSetColor( 0xFF00FF );
			ofLine( p1.x, p1.y, p2.x, p2.y );
			ofCircle( p1.x, p1.y, 4 );
		}
	}
}

///////////////////////////////////////////////////
//	SAVE / LOAD
///////////////////////////////////////////////////

void Rocks :: save( string fileName )
{
	ofstream myFile;
	myFile.open( ofToDataPath( fileName ).c_str() );
	if( myFile.is_open() )
	{
		for( int i=0; i<blobs.size(); i++ )
		{
			ofxCvBlob &blob = blobs[ i ];
			
			for( int j=0; j<blob.pts.size(); j++ )
			{
				ofPoint &p = blob.pts[ j ];
				
				myFile << ofToString( p.x, 4 ) << " " << ofToString( p.y, 4 ) << endl;
			}
			
			myFile << "" << endl;
		}
	}
	myFile.close();
}

void Rocks :: load( string fileName )
{
	blobs.clear();
	
	ifstream myFile;
	string line;
	myFile.open( ofToDataPath( fileName ).c_str() );
	if( myFile.is_open() )
	{
		bool bNewBlob = true;
		
		while( !myFile.eof() )
		{
			getline( myFile, line );
			
			if( bNewBlob )
			{
				if( line == "" )		// if second blank line then finished.
				{
					break;
				}
				
				bNewBlob = false;
				
				blobs.push_back( ofxCvBlob() );
			}
			
			if( line == "" )
			{
				bNewBlob = true;
				
				continue;
			}
			
			vector<string> values = ofSplitString( line, " " );
			
			ofxCvBlob &blob = blobs.back();
			
			blob.pts.push_back( ofPoint() );
			ofPoint &p = blob.pts.back();
			p.x = atof( values[ 0 ].c_str() );
			p.y = atof( values[ 1 ].c_str() );
		}
	}
	myFile.close();
}
