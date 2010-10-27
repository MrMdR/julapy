/*
 *  Contour.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 24/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Contour.h"

Contour :: Contour()
{
	bDebug				= false;
	bProgressChanged	= true;
	bTabChanged			= true;
	
	setup();
}

Contour :: ~Contour()
{

}

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void Contour :: setup ()
{
	model = Model :: getInstance();
	
	ofAddListener( model->progressChangeEvent,	this, &Contour :: progressChangeEventHandler );
	ofAddListener( model->tabTypeChangeEvent,	this, &Contour :: tabTypeChangeEventHandler );
	
	initSize();
	initAnim();
	initOpenCv();
	initContours();
	initGui();
}

void Contour :: initSize ()
{
	screenRect.width	= ofGetWidth();
	screenRect.height	= ofGetHeight();
	
	sourceRect.width	= 886;		// hardcoded size of BOM image.
	sourceRect.height	= 691;
	
	float smallRectScale;			// scale down for performance.
	smallRectScale = 0.25;
	
	smallRect.width		= sourceRect.width  * smallRectScale;
	smallRect.height	= sourceRect.height * smallRectScale;
	
	float largeRectExtra;
	largeRectExtra		= 0.04;			// the extra is to cover the whole screen. set to 0 to see what happens without it.
	largeRectExtra		= 0;
	
//	largeRect			= ofxResizeUtil :: cropToSize( smallRect, screenRect );
//	largeRect			= ofxResizeUtil :: fitToSize( smallRect, screenRect );
//	largeRect.x			-= largeRect.width  * largeRectExtra * 0.5;
//	largeRect.y			-= largeRect.height * largeRectExtra * 0.5;
//	largeRect.width		*= 1 + largeRectExtra;
//	largeRect.height	*= 1 + largeRectExtra;
	
	largeRect			= model->getPixelRect();
	
	debugRect.height	= 150;			// debug height.
	debugRect.width		= (int)( ( debugRect.height / sourceRect.height ) * sourceRect.width );	
}

void Contour :: initAnim ()
{
	bomRainAnim.loadAnimationImages( "bom/rain/image" );
	bomTempAnim.loadAnimationImages( "bom/temp/image" );
	
	bomImage.allocate( smallRect.width, smallRect.height );
}

void Contour :: initOpenCv ()
{
	bandsTotal	= 8;
	bandIndex	= 0;
	
	bands		= new ofxCvGrayscaleImage[ bandsTotal ];
	bandCutoffs	= new float[ bandsTotal ];
	
	for( int i=0; i<bandsTotal; i++ )
	{
		ofxCvGrayscaleImage& band = bands[ i ];
		band.allocate( smallRect.width, smallRect.height );
		
		float& bandCutoff = bandCutoffs[ i ];
		bandCutoff = 0.1 + ( i / (float)( bandsTotal - 1 ) ) * 0.8;
	}
	
	bandSum.allocate( smallRect.width, smallRect.height );
	
	//-- colors;
	
	bandRainColors = new ofColor[ bandsTotal ];
	bandTempColors = new ofColor[ bandsTotal ];
	
	int t;
	ofColor c;
	vector<KeyDataItem>* keyData;
	
	keyData = &model->getKeyRainData();					// rain colors.
	t		= MIN( keyData->size(), bandsTotal );
	
	for( int i=0; i<t; i++ )
	{
		c = keyData->at( i ).color;
		bandRainColors[ i ] = c;
	}
	
	keyData = &model->getKeyTempData();					// temp colors.
	t		= MIN( keyData->size(), bandsTotal );
	
	for( int i=0; i<t; i++ )
	{
		c = keyData->at( i ).color;
		bandTempColors[ i ] = c;
	}
	
	bandBaseColor = &bandRainColors[ 0 ];
}

void Contour :: initContours ()
{
	contourSmoothScale			= 0.5;
}

void Contour :: initGui ()
{
	gui.setAutoSave( false );
	
	gui.addTitle( "general" );
	
//	gui.addToggle( "bDebug  ",			bDebug );
//	gui.addToggle( "bSmoothing  ",		bSmoothing );
//	gui.addToggle( "bDrawPoints  ",		bDrawPoints );
//	gui.addToggle( "bDrawLines  ",		bDrawLines );
//	gui.addToggle( "bDrawCurves  ",		bDrawCurves );
//	gui.addToggle( "bDrawSimplified  ",	bDrawSimplified );
//	gui.addToggle( "bDrawColor  ",		bDrawColor );
	
	gui.addPage( "bands" );
	gui.addTitle( "bands index", 22 );
	gui.addSlider( "bandIndex  ",	bandIndex, 0, bandsTotal - 1 );
	for( int i=0; i<bandsTotal; i++ )
	{
		gui.addTitle( "band " + ofToString( i, 0 ), 22 );
		gui.addSlider( "band " + ofToString( i, 0 ) + " cutoff  ",	bandCutoffs[ i ],	0, 1.0 );
	}
	
	gui.addPage( "contour" );
	gui.addSlider( "contourSmoothScale ",		contourSmoothScale,			0, 1.0 );
	
	gui.show();
	gui.setPage( 4 );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void Contour :: update()
{
	bool bAnimChanged;
	bAnimChanged = updateAnim();
	
	if( bAnimChanged )
	{
		updateOpenCv();
		updateBlobs();
	}
}

bool Contour :: updateAnim ()
{
	bool bChanged;
	bChanged = bTabChanged || bProgressChanged;
	
	if( bTabChanged )
	{
		if( model->getTabType() == TIMELINE_TAB_RAIN )
		{
			bomAnim = &bomRainAnim;
		}
		else if( model->getTabType() == TIMELINE_TAB_TEMP )
		{
			bomAnim = &bomTempAnim;
		}
		
		bTabChanged = false;
	}
	
	if( bChanged )
	{
		bomAnim->setProgress( model->getProgress() );
		bomAnim->update();	
		
		ofxCvGrayscaleImage* image;
		image = bomAnim->getImage();
		
		bomImage.scaleIntoMe( *image );
		
		bProgressChanged = false;
	}
	
	return bChanged;
}

void Contour :: updateOpenCv ()
{
	bandSum.set( 0 );
	
	for( int i=0; i<bandsTotal; i++ )
	{
		int bandCutoff;
		bandCutoff	= bandCutoffs[ i ] * 255;
		
		ofxCvGrayscaleImage& band = bands[ i ];
		band		= bomImage;
		band.threshold( bandCutoff, false );
		
		if( !bDebug )
			continue;
		
		int t = smallRect.width * smallRect.height;
		unsigned char* pixels0 = band.getPixels();
		unsigned char* pixels1 = bandSum.getPixels();
		
		for( int j=0; j<t; j++ )
		{
			if( pixels0[ j ] == 255 )
				pixels1[ j ] = bandCutoff;
		}
	}
}

void Contour :: updateBlobs ()
{
	//-- remove old blobs.
	
	contourBlobs.clear();
	contourBlobsScaled.clear();
	
	//-- colors.
	
	if( model->getTabType() == TIMELINE_TAB_RAIN )
	{
		bandBaseColor = &bandRainColors[ bandsTotal - 1 ];
	}
	else if( model->getTabType() == TIMELINE_TAB_TEMP )
	{
		bandBaseColor = &bandTempColors[ bandsTotal - 1 ];
	}
	
	//-- copy blobs and scale.
	
	float scale;
	scale = largeRect.width / (float)smallRect.width;
	
	float xoff;
	xoff = largeRect.x;
	
	float yoff;
	yoff = largeRect.y;
	
	for( int i=0; i<bandsTotal; i++ )
	{
		ofxCvGrayscaleImage& image = bands[ i ];
		
		int numOfBlobs;
		numOfBlobs = updateContours( image );
		
		if( numOfBlobs == 0 )
			continue;
		
		ofColor c;
		if( model->getTabType() == TIMELINE_TAB_RAIN )
		{
			c = bandRainColors[ bandsTotal - 1 - i ];
		}
		else if( model->getTabType() == TIMELINE_TAB_TEMP )
		{
			c = bandTempColors[ bandsTotal - 1 - i ];
		}
		
		for( int j=0; j<numOfBlobs; j++ )
		{
			ofxCvBlob& blob = contourFinder.blobs[ j ];
			
			contourBlobs.push_back( Blob() );
			contourBlobsScaled.push_back( Blob() );
			
			copyBlob( blob, contourBlobs.back() );
			copyBlob( blob, contourBlobsScaled.back(), xoff, yoff, scale );
			
			contourBlobs.back().color			= c;
			contourBlobsScaled.back().color		= c;
			
			contourBlobs.back().layer			= i;
			contourBlobsScaled.back().layer		= i;
			
			if( contourSmoothScale > 0 )
			{
				contourUtil.smooth( contourBlobsScaled.back().pts, contourSmoothScale, 1.0 );
			}
		}
	}
}

int Contour :: updateContours ( ofxCvGrayscaleImage& image )
{
	int maxArea;
	maxArea = smallRect.width * smallRect.height;
	
	float cfMinArea;
	cfMinArea = 0.001;
	
	int maxContoursToFind;
	maxContoursToFind = 50;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		image,							// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		true,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void Contour :: copyBlob ( ofxCvBlob& blob, ofxCvBlob& blobCopy, float xoff, float yoff, float scale )
{
	for( int k=0; k<blob.pts.size(); k++ )
	{
		ofPoint& p1 = blob.pts[ k ];
		
		blobCopy.pts.push_back( ofPoint() );
		ofPoint& p2 = blobCopy.pts.back();
		
		p2.x = p1.x * scale + xoff;
		p2.y = p1.y * scale + yoff;
	}
	
	blobCopy.area					= blob.area;
	blobCopy.boundingRect.x			= blob.boundingRect.x * scale + xoff;
	blobCopy.boundingRect.y			= blob.boundingRect.y * scale + yoff;
	blobCopy.boundingRect.width		= blob.boundingRect.width  * scale + xoff;
	blobCopy.boundingRect.height	= blob.boundingRect.height * scale + yoff;
	blobCopy.centroid.x				= blob.centroid.x * scale + xoff;
	blobCopy.centroid.y				= blob.centroid.y * scale + yoff;
	blobCopy.hole					= blob.hole;
	blobCopy.length					= blob.length;
	blobCopy.nPts					= blob.nPts;
	
	blobCopy.nPts = blobCopy.pts.size();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void Contour :: progressChangeEventHandler ( float& p )
{
	bProgressChanged = true;
}

void Contour :: tabTypeChangeEventHandler ( int& t )
{
	bTabChanged = true;
}
