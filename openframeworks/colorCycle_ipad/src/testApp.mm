#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	
	iPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
	
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	
	cc.setScreenSize( ofGetWidth(), ofGetHeight() );
	cc.setup();
	
	footer = NULL;
	footer = new FooterBar();
	if( footer != NULL )
		footer->setup();
}

//--------------------------------------------------------------
void testApp::update()
{
	if( footer->isShuffleSelected() )
		cc.shuffle();
	
	if( footer->isColorSelected() )
		cc.down( 0, 0, 0 );
	
	if( footer->isAddSelected() )
		cc.addCircle();
	
	if( footer->isRemoveSelected() )
		cc.removeCircle();
	
	float gx;
	gx = ofxAccelerometer.getForce().y;
	gx *= 2;									// increase the reaction to tilt.
	gx = MIN( 1.0, MAX( -1.0, gx ) );			// between -1 and 1.
	gx *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;
	
	float gy;
	gy = ofxAccelerometer.getForce().x;
	gy *= 2;									// increase the reaction to tilt.
	gy = MIN( 1.0, MAX( -1.0, gy ) );			// between -1 and 1.
	gy *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;
	
	cc.setGravity( gx, gy );
	
	cc.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	cc.draw();
	
	ofSetColor( 0, 0, 0 );
	ofDrawBitmapString( ofToString( ofGetFrameRate(),  0 ), ofGetScreenWidth() - 30, 20 );
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch)
{
	cc.down( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch)
{
	cc.drag( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch)
{
	cc.up( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch)
{
	if( footer != NULL )
		footer->toggleShow();
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

