/*
 *  Tracker.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 13/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tracker.h"

Tracker :: Tracker ()
{
	screenPos.x	= 260;
	screenPos.y	= 115;
}

Tracker :: ~Tracker ()
{

}

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void Tracker :: setup()
{
	initVideo();
	initVideoGrabber();
	initVideoPlayer();
	
	cameraRadius = 200;
	updateCameraPos( videoSmlRect.width * 0.5, videoSmlRect.height * 0.5 );
	
	bUpdateCameraPos	= false;
}

void Tracker :: initVideo()
{
	videoLrgRect.width  = 640;
	videoLrgRect.height = 480;
	
	videoSmlRect.width  = 320;
	videoSmlRect.height = 240;
	
	videoLrgImage.allocate( videoLrgRect.width, videoLrgRect.height );
	videoSmlImage.allocate( videoSmlRect.width, videoSmlRect.height );
	
	cvImageWarp.allocate( videoSmlRect.width, videoSmlRect.height );
	cvImageBg.allocate( videoSmlRect.width, videoSmlRect.height );
	cvImageDiff.allocate( videoSmlRect.width, videoSmlRect.height );
	cvImageThsh.allocate( videoSmlRect.width, videoSmlRect.height );
	
	cvWarper.setup( &videoSmlImage, &cvImageWarp );
	cvWarper.setPosition( screenPos.x, screenPos.y );
	cvWarper.load( "video_warper_data.txt" );
	
	threshold_1	= 30;
	threshold_2	= 30;
	blur		= 4;
	
	isVideoNewFrame = false;
}

void Tracker :: initVideoGrabber ()
{
#ifndef USE_VIDEO_INPUT
#ifdef	USE_POINT_GREY_CAMERA	
	bool result = videoGrabber.initGrabber( videoLrgRect.width, videoLrgRect.height, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 25, true, new Libdc1394Grabber() );
	
	if( result )
	{
		ofLog(OF_LOG_NOTICE,"Camera succesfully initialized.");
	} 
	else
	{
		ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}
#else
	videoGrabber.initGrabber( videoLrgRect.width, videoLrgRect.height );
#endif
#endif
}

void Tracker :: initVideoPlayer ()
{
#ifdef USE_VIDEO_INPUT
	videoPlayer.loadMovie( "underbelly_test_03.mov" );
	videoPlayer.setPaused( false );
	videoPlayer.setPosition( 0.0 );
	videoPlayer.setLoopState( true );
	videoPlayer.play();
#endif
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void Tracker :: update ()
{
	if( ofGetFrameNum() == 2 )		// automatically set background after a delay.
	{
		updateBackground();
	}
	
	updateVideoGrabber();
	updateVideoPlayer();
	updateTracking();
	
	int blobsFound;
	blobsFound = updateContours();
	
	if( blobsFound > 0 )
	{
		blobUtil.downSampleBlobs( contourFinder.blobs, blobs, 40 );
		blobUtil.scaleBlobs( blobs, renderArea.width / (float)videoSmlRect.width, renderArea.height / (float)videoSmlRect.height );
//		blobUtil.translateBlobs( peeps, renderArea.x, renderArea.y );
	}
	else
	{
		blobs.clear();
	}
}

void Tracker :: updateVideoGrabber ()
{
#ifndef USE_VIDEO_INPUT
#ifdef	USE_POINT_GREY_CAMERA
	videoGrabber.update();
#else
	videoGrabber.grabFrame();
#endif
	
	isVideoNewFrame = videoGrabber.isFrameNew();
	
	if( isVideoNewFrame )
	{
		
#ifdef USE_POINT_GREY_CAMERA
		
		videoLrgImage.setFromPixels( videoGrabber.getPixels(), videoLrgRect.width, videoLrgRect.height );
		
#else
		ofxCvColorImage videoClrImage;
		videoClrImage.allocate( videoLrgRect.width, videoLrgRect.height );
		videoClrImage.setFromPixels( videoGrabber.getPixels(), videoLrgRect.width, videoLrgRect.height );
		
		videoLrgImage.setFromColorImage( videoClrImage );
		
		videoClrImage.clear();
#endif
		
		videoSmlImage.scaleIntoMe( videoLrgImage );
	}
#endif
}

void Tracker :: updateVideoPlayer ()
{
#ifdef USE_VIDEO_INPUT
	
	videoPlayer.update();
	
	isVideoNewFrame = videoPlayer.isFrameNew();
	
	if( isVideoNewFrame )
	{
		ofxCvColorImage videoClrImage;
		videoClrImage.allocate( videoLrgRect.width, videoLrgRect.height );
		videoClrImage.setFromPixels( videoPlayer.getPixels(), videoLrgRect.width, videoLrgRect.height );
		
		videoLrgImage.setFromColorImage( videoClrImage );
		
		videoClrImage.clear();
		
		videoSmlImage.scaleIntoMe( videoLrgImage );
	}
	
#endif
}

void Tracker :: updateTracking ()
{
	cvWarper.warp();
	
	cvImageDiff.absDiff( cvImageBg, cvImageWarp );
	cvImageDiff.threshold( threshold_1, false );
	cvImageDiff.blur( blur );
	
	cvImageThsh = cvImageDiff;
	cvImageThsh.threshold( threshold_2, false );
}

void Tracker :: updateBackground ()
{
	cvImageBg = cvImageWarp;
}

int Tracker :: updateContours ()
{
	int maxArea;
	maxArea = videoSmlRect.width * videoSmlRect.height;
	
	float cfMinArea;
	cfMinArea = 0.001;
	
	int maxContoursToFind;
	maxContoursToFind = 20;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		cvImageThsh,					// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		false,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void Tracker :: updateCameraPos	( int x, int y )
{
	cameraPosition.x = x - screenPos.x;
	cameraPosition.y = y - screenPos.y;
	
	float dist = 0;
	int w = videoSmlRect.width;
	int h = videoSmlRect.height;
	
	//	cameraRadius = 0;
	//	
	//	dist = ofDist( x, y, 0, 0 );
	//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
	//	
	//	dist = ofDist( x, y, w, 0 );
	//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
	//	
	//	dist = ofDist( x, y, w, h );
	//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
	//	
	//	dist = ofDist( x, y, 0, h );
	//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
}

void Tracker :: updateRenderArea( ofRectangle &area )
{
	renderArea.x		= area.x;
	renderArea.y		= area.y;
	renderArea.width	= area.width;
	renderArea.height	= area.height;
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void Tracker :: draw ()
{
	int pad = 10;
	
	glPushMatrix();
	glTranslatef( screenPos.x, screenPos.y, 0 );
	
	ofSetColor( 0xFFFFFF );
	videoSmlImage.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "1) original", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlImage.width + pad, 0, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageWarp.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "2) warped", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlRect.width + pad, 0, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageBg.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "3) background", 10, 20 );
	
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( 0, videoSmlRect.height + pad, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageDiff.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "4) diff + blur", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlRect.width + pad, 0, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageThsh.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "5) threshold", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlRect.width + pad, 0, 0 );
	
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, videoSmlRect.width, videoSmlRect.height );
	contourFinder.draw();
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "6) contours", 10, 20 );
	
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	ofNoFill();											//-- camera position.
	ofSetColor( 255, 0, 0, 127 );
	ofEnableAlphaBlending();
	ofCircle( cameraPosition.x, cameraPosition.y, cameraRadius );
	ofFill();
	ofCircle( cameraPosition.x, cameraPosition.y, 3 );
	ofDisableAlphaBlending();
	
	glPopMatrix();
	
	cvWarper.draw();
}