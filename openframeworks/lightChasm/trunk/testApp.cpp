#include "testApp.h"

void testApp::setup()
{
	ofBackground( 255, 255, 255 );
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofSetFrameRate( 25 );
	ofSoundStreamSetup( 0, 2, this, 44100, 512, 4 );
	
	audioIn.init();
	
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
	
	opBars.init( 640, 480 );
	opBars.setNumberOfBars( audioIn.averagesTotal );
	
	testVideo.loadMovie( "squares_640x480.mov" );
	testVideo.play();
}

void testApp::update()
{
	audioIn.update();
	oscPad.update();
	opCirlce.update();
	
	opBars.setAudioInData( audioIn.averages );
	opBars.update();
}

void testApp::draw()
{
	ofSetColor( 0xFFFFFF );
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );

	if( oscPad.getButtonState( 1, 1 ) )
	{
		testVideo.draw( 0, 0, testVideo.width, testVideo.height );
	}
	
	opBars.draw();
	opCirlce.draw();
	
	glDisable( GL_BLEND );

	audioIn.draw();
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


void testApp :: audioReceived( float *input, int bufferSize, int nChannels )
{
	for( int i=0; i<bufferSize; i++)
	{
		audioIn.left[ i ]	= input[ i * 2 ];
		audioIn.right[ i ]	= input[ i * 2 + 1 ];
	}
}

