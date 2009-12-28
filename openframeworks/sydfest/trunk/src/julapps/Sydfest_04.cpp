/*
 *  Sydfest_04.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 28/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sydfest_04.h"

Sydfest_04 :: Sydfest_04()
{

}

Sydfest_04 :: ~Sydfest_04()
{

}

void Sydfest_04 :: setup ()
{
	bDrawDebug	= false;
	threshold	= 30;
	
	initVideoGrabber();
	
	initImage();
	updateContours();
	
	initBox2d();
	initGui();
}

void Sydfest_04 :: update ()
{
	updateVideoGrabber();
//	updateContours();
	updateBox2d();
}

void Sydfest_04 :: draw ()
{
	drawImage();
//	drawContours();
	drawBox2d();
}

void Sydfest_04 :: drawDebug ()
{
	if( !bDrawDebug )
		return;
	
	gui.draw();
	
	glPushMatrix();
	glTranslatef( 270, 74, 0 );
	
	drawVideoGrabber();
	
	glPopMatrix();
	
	drawVideoGrabberWarper();
}

//////////////////////////////////////////////////
//	INIT.
//////////////////////////////////////////////////

void Sydfest_04 :: initVideoGrabber ()
{
	videoGrabberLrgRect.width  = 640;
	videoGrabberLrgRect.height = 480;
	
	videoGrabberSmlRect.width  = 320;
	videoGrabberSmlRect.height = 240;
	
	videoGrabberSrcRect.width  = 320;
	videoGrabberSrcRect.height = 240;
	
	videoGrabberDstRect.width  = 240;
	videoGrabberDstRect.height = 240;
	
	videoGrabber.initGrabber( videoGrabberLrgRect.width, videoGrabberLrgRect.height );
	
	videoGrabberLrgImage.allocate( videoGrabberLrgRect.width, videoGrabberLrgRect.height );
	videoGrabberSmlImage.allocate( videoGrabberSmlRect.width, videoGrabberSmlRect.height );
	videoGrabberSrcImage.allocate( videoGrabberSrcRect.width, videoGrabberSrcRect.height );
	videoGrabberDstImage.allocate( videoGrabberDstRect.width, videoGrabberDstRect.height );
	
	videoGrabberWarper.setup( &videoGrabberSrcImage, &videoGrabberDstImage );
	videoGrabberWarper.setPosition( 270, 74 );
	videoGrabberWarper.load( "video_warper_data.txt" );
	
	isVideoGrabberNewFrame = false;
}

void Sydfest_04 :: initImage ()
{
//	image.loadImage( "barracks1.jpg" );
//	image.loadImage( "barracks2.jpg" );
	image.loadImage( "barracks3.jpg" );
	
	imageRect.width		= image.width;
	imageRect.height	= image.height;
	
	imageCol.allocate( imageRect.width, imageRect.height );
	imageGry.allocate( imageRect.width, imageRect.height );
	
	imageCol.setFromPixels( image.getPixels(), imageRect.width, imageRect.height );
	imageGry = imageCol;
}

void Sydfest_04 :: initContours ()
{
	contourAnalysis.setSize( renderArea.width, renderArea.height );
}

void Sydfest_04 :: initBox2d ()
{
	box2d.init();
	box2d.setGravity( 0, 20 );
	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	
	vector<ofxCvBlobToo> blobs;
	blobs = contourFinder.getBlobs();
	
	for( int i=0; i<blobs.size(); i++ )
	{
		box2dLineStrips.push_back( ofxBox2dLine() );
		
		ofxBox2dLine *lineStrip;
		lineStrip = &box2dLineStrips.back();
		
		ofPoint p;
		lineStrip->setWorld( box2d.getWorld() );
		
		bool bReversePoints;
		bReversePoints = false;
			
		vector<ofPoint> pts;
		pts = blobs[ i ].getPoints();
		
		vector<ofPoint> simpleContour;
		
		int simpleAmount;
		simpleAmount = 20;
		
		contourAnalysis.simplify( pts, simpleContour, simpleAmount );
		
		if( bReversePoints )
		{
			for( int j=simpleContour.size()-1; j>0; j-- )
			{
				p.x = simpleContour[ j ].x;
				p.y = simpleContour[ j ].y;
				lineStrip->addPoint( p.x, p.y );
			}
		}
		else
		{
			for( int j=0; j<simpleContour.size(); j++ )
			{
				p.x = simpleContour[ j ].x;
				p.y = simpleContour[ j ].y;
				lineStrip->addPoint( p.x, p.y );
			}
		}
		
		lineStrip->createShape();
	}
}

void Sydfest_04 :: initGui ()
{
	gui.addSlider( "threshold", threshold, 0, 255 );
	
//	gui.loadFromXML( "ofxSimpleGuiToo.xml" );
}

//////////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////////

void Sydfest_04 :: updateVideoGrabber ()
{
	videoGrabber.grabFrame();
	
	isVideoGrabberNewFrame = videoGrabber.isFrameNew();
	
	if( isVideoGrabberNewFrame )
	{
		videoGrabberLrgImage.setFromPixels( videoGrabber.getPixels(), videoGrabberLrgRect.width, videoGrabberLrgRect.height );
		
		videoGrabberSmlImage.scaleIntoMe( videoGrabberLrgImage );
		videoGrabberSrcImage.setFromColorImage( videoGrabberSmlImage );
		
		videoGrabberWarper.warp();
	}
}

void Sydfest_04 :: updateContours ()
{
	int area		= imageRect.width * imageRect.height;
	int minArea		= area * 0.001;
	int maxArea		= area * 0.9;
	int maxBlobs	= 20;
	
//	contourFinder.findContours( imageGry, minArea, maxArea, maxBlobs );
	contourFinder.findContoursAndDefects( imageGry, minArea, maxArea, maxBlobs );
}

void Sydfest_04 :: updateBox2d ()
{
	box2d.update();
}

//////////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////////

void Sydfest_04 :: drawVideoGrabber ()
{
	ofFill();
	ofSetColor( 150, 150, 150 );
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 0, 0, 0 );
		ofRect( -1, -1, videoGrabberSrcImage.width + 2, videoGrabberSrcImage.height + 2 );
		videoGrabberSrcImage.draw( 0, 0 );
	glPopMatrix();
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( videoGrabberSrcRect.width + 10, 0, 0 );
		ofRect( -1, -1, videoGrabberDstImage.width + 2, videoGrabberDstImage.height + 2 );
		videoGrabberDstImage.draw( 0, 0 );
	glPopMatrix();
}

void Sydfest_04 :: drawVideoGrabberWarper ()
{
	videoGrabberWarper.draw();
}

void Sydfest_04 :: drawImage ()
{
	ofSetColor( 0xFFFFFF );
	
	imageGry.draw( 0, 0 );
}

void Sydfest_04 :: drawContours ()
{
	contourFinder.draw( 0, 0 );
}

void Sydfest_04 :: drawBox2d ()
{
	box2d.draw();
	
	for( int i=0; i<box2dCircles.size(); i++ )
	{
		ofSetColor( 0xFF0000 );
		
		ofxBox2dCircle *circle;
		circle = &box2dCircles[ i ];
		
		ofPoint p = circle->getPosition();
		float	r = circle->getRadius();
		
		ofFill();
		ofCircle( p.x, p.y, r );
		ofNoFill();
		ofCircle( p.x, p.y, r );
	}

	for( int i=0; i<box2dLineStrips.size(); i++ )
	{
		box2dLineStrips[ i ].draw();
	}
}

//////////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////////

void Sydfest_04 :: keyReleased ( int key )
{
	if( key == 'd' )
		bDrawDebug = !bDrawDebug;
}

void Sydfest_04 :: mousePressed ( int x, int y, int button )
{
	int circlesPerClick;
	circlesPerClick = 10;
	
	for( int i=0; i<circlesPerClick; i++ )
	{
		box2dCircles.push_back( ofxBox2dCircle() );
		
		ofxBox2dCircle *circle;
		circle = &box2dCircles.back();
		
		circle->setPhysics
		(
			0.0001,		// mass.
			0.75,		// bounce.
			0.01		// friction.
		);
		
		float rnd;
		rnd = ofRandom( -2, 2 );
		
		circle->setup
		(
			box2d.getWorld(),
			x + rnd,
			y + rnd,
			ofRandom( 5, 10 )
		);
	}
}

