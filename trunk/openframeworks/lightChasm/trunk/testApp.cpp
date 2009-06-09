#include "testApp.h"

void testApp::setup()
{
	ofBackground( 255, 255, 255 );
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofSetFrameRate( 25 );
	
	oscPad.init();
	oscPad.setOffOnRelease( false );
	
	opCirlceWidth	= 640;
	opCirlceHeight	= 480;
	
	float rx = opCirlceWidth / 2.0f;
	float ry = opCirlceHeight / 2.0f;
	
	opCirlce.x		= rx;
	opCirlce.y		= ry;
	opCirlce.radius	= sqrt( rx * rx + ry * ry ) * 1.5f;
	opCirlce.setCirlceResolution( opCirlceRes = 6 );
	opCirlce.setRotation( opCirlceRot = 0 );
	
	testVideo.loadMovie( "squares_640x480.mov" );
	testVideo.play();
}

void testApp::update()
{
	oscPad.update();
	opCirlce.update();
}

void testApp::draw()
{
	if( oscPad.getButtonState( 1, 1 ) )
	{
		testVideo.draw( 0, 0, testVideo.width, testVideo.height );
	}

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );
	opCirlce.draw();
	
	glDisable( GL_BLEND );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if( key == ',' )
	{
		opCirlce.setCirlceResolution( opCirlceRes -= 2 );
	}
	
	if( key == '.' )
	{
		opCirlce.setCirlceResolution( opCirlceRes += 2 );
	}
	
	if( key == 'l' )
	{
		opCirlce.setRotation( opCirlceRot += 0.5 );
	}
	
	if( key == ';' )
	{
		opCirlce.setRotation( opCirlceRot -= 0.5 );
	}
	
	if( key == 'i' )
	{
		opCirlce.setInverse();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

