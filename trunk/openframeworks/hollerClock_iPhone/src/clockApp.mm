#include "clockApp.h"

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void clockApp :: setup()
{	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT );
	
	//-------------------
	
	deviceType = DEVICE_IPAD;
//	deviceType = DEVICE_IPHONE;

	ofBackground( 0, 0, 0 );
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	font.loadFont( "fonts/cooperBlack.ttf", 40 );
	
	initClock();
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
	if( deviceType == DEVICE_IPAD )		clock.setSize( 1024, 768 );
	if( deviceType == DEVICE_IPHONE )	clock.setSize( 480,  320 );
//	clock.setTimeFont( &font );
	clock.setup();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void clockApp :: update()
{
	int hrs;
	int min;
	int sec;
	
	if( true )
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
	
	//-- orientation.
	
	float ax = ofxAccelerometer.getForce().x;
	float ot = 0.6;								// orientation threshold.
	
	if( ax > ot )
	{
		if( ofxiPhoneGetOrientation() != OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT )
		{
			ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
		}
	}
	else if( ax < -ot )
	{
		if( ofxiPhoneGetOrientation() != OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT )
		{
			ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT );
		}
	}
	
	//-- gravity slant.
	
	float g;
	g = ofxAccelerometer.getForce().y;
	g *= 2;									// increase the reaction to tilt.
	g = MIN( 1.0, MAX( -1.0, g ) );			// between -1 and 1.
	g *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;
	
	clock.setGravitySlant( g );
	
	//-- update.
	
	clock.update( hrs, min, sec );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void clockApp :: draw()
{
	int bg = 0;
	ofBackground( bg, bg, bg );
	
	clock.draw();
	
	ofSetColor( 0xFFFFFF );
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 15, ofGetHeight() - 15 );
}

void clockApp :: exit()
{

}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void clockApp::touchDown(ofTouchEventArgs &touch){

}

void clockApp::touchMoved(ofTouchEventArgs &touch){

}

void clockApp::touchUp(ofTouchEventArgs &touch){

}

void clockApp::touchDoubleTap( ofTouchEventArgs &touch )
{
	clock.toggleClockMode();
}

void clockApp::lostFocus(){

}

void clockApp::gotFocus(){

}

void clockApp::gotMemoryWarning(){

}

void clockApp::deviceOrientationChanged(int newOrientation)
{
	//
}

