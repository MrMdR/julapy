#include "clockApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void clockApp :: setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	bDebug = true;
	
	font.loadFont( "fonts/cooperBlack.ttf", 40 );
	
	screenGrabber.setup( "movie/" );
	screenGrabber.setPause( true );
	
	ofRectangle rect;
	rect.width	= ofGetWidth();
	rect.height	= ofGetHeight();
	
	videoSaver.setup( rect, OF_IMAGE_COLOR );
	videoSaver.setPause( true );
	
	initClock();
	initGui();
}
	
void clockApp :: initClock ()
{
	box2d.init();
	box2d.createBounds();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.registerGrabbing();
	box2d.getWorld()->SetContactListener( &contactListener );
	
	contactListener.addReceiver( &clock );
	
	clock.setBox2d( &box2d );
	clock.setSize( ofGetWidth(), ofGetHeight() );
	clock.setTimeFont( &font );
	clock.setup();
}

void clockApp :: initGui ()
{
	gui.addTitle( "clock" );
	gui.addSlider( "forceCenterPull",	clock.forceCenterPull,	0, 100 );
	gui.addSlider( "forceCenterPush",	clock.forceCenterPush,	0, 100 );
	gui.addSlider( "rayBlobPad",		clock.rayBlobPad,		0, 100 );
	gui.addSlider( "rayBlobEase",		clock.rayBlobEase,		0, 1 );
	
	gui.setPage( 1 );
	
	if( bDebug )
		gui.show();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void clockApp::update()
{
	int hrs;
	int min;
	int sec;
	
	if( screenGrabber.isRecording() )
	{
		int frameNum = ofGetFrameNum();
		
		int s = 00 + frameNum / frameRate;		// start time is add at front.
		int m = 48 + s / 60;					// start time is add at front.
		int h = 12 + m / 60;					// start time is add at front.
		
		hrs = h % 24;
		min = m % 60;
		sec = s % 60;
	}
	else
	{
		hrs = ofGetHours();
		min = ofGetMinutes();
		sec = ofGetSeconds();
	}
	
	clock.update( hrs, min, sec );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void clockApp::draw()
{
	int bg = 0;
	ofBackground( bg, bg, bg );
	
	clock.draw();
	
	ofSetColor( 0xFFFFFF );
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 15, ofGetHeight() - 15 );

	gui.draw();
	
	screenGrabber.save();
	
	screenImage.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
	videoSaver.addFrame( screenImage.getPixels() );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void clockApp :: keyPressed(int key)
{
	if( key == 's' )
		screenGrabber.togglePause();
	
	if( key == 'v' )
	{
		videoSaver.togglePause();
		
		if( videoSaver.isPaused() )
		{
			videoSaver.saveToDisk( "movies/" );
		}
	}
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
		
		if( bDebug )
		{
			gui.show();
		}
		else
		{
			gui.hide();
		}
	}
	
	if( key == 'm' )
	{
		clock.toggleClockMode();
	}
}

void clockApp :: keyReleased(int key)
{

}

void clockApp::mouseMoved(int x, int y )
{
	float p;
	p = x / (float)ofGetWidth();
	p = MAX( MIN( p , 1 ), 0 );
	
	float g;
	g = ( p - 0.5 ) * 2;
	
	clock.setGravitySlant( g );
}

void clockApp::mouseDragged(int x, int y, int button)
{

}

void clockApp::mousePressed(int x, int y, int button)
{

}

void clockApp::mouseReleased(int x, int y, int button)
{

}

void clockApp::windowResized(int w, int h)
{

}

