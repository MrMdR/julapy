#include "testApp.h"

void testApp::setup()
{
	ofSetFrameRate( 25 );
	ofBackground( 255, 255, 255 );
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
//	ofSetVerticalSync( true );
	ofSoundStreamSetup( 0, 2, this, 44100, 512, 4 );
	
	initAudioIn();
	initOsc();
	initOpScope();
	initOpCheckers();
	initOpCircle();
	initOpBars();
	initVideos();
}

///////////////////////////////////////////////////
//	INIT.
///////////////////////////////////////////////////

void testApp :: initAudioIn ()
{
	audioIn.init();
}

void testApp :: initOsc ()
{
	oscPad.init();
	oscPad.setOrientation( );
	oscPad.setOffOnRelease( false );
}

void testApp :: initOpCheckers ()
{
	opCheckers.init( 640, 480 );
	opCheckers.setSize( opCheckersSize = 30 );
}

void testApp :: initOpScope ()
{
	opScope.init( 640, 480 );
}

void testApp :: initOpCircle ()
{
	opCirlceWidth	= 640;
	opCirlceHeight	= 480;
	
	float rx = opCirlceWidth / 2.0f;
	float ry = opCirlceHeight / 2.0f;
	
	opCirlce.x		= rx;
	opCirlce.y		= ry;
	opCirlce.radius	= sqrt( rx * rx + ry * ry ) * 1.5f;
	opCirlce.setCirlceResolution( opCirlceRes = 50 );
	opCirlce.setRotation( opCirlceRot = 0 );
}

void testApp :: initOpBars ()
{
	opBarsAudioAvgMin = 15;
	
	opBars.init( 640, 480 );
	opBars.setNumberOfBars( audioIn.averagesTotal );
	opBars.setAudioAvgMin( opBarsAudioAvgMin );
}

void testApp :: initVideos ()
{
	isVideo01Playing = true;
	isVideo02Playing = false;
	isVideo03Playing = false;
	
	//	video01.loadMovie( "koyaanisqatsi_water_02_white_on_black.mov" );
	video01.loadMovie( "squares_640x480.mov" );
	video01.play();
	video01.setPaused( true );
	
	video02.loadMovie( "koyaanisqatsi_sky_04_white_on_black.mov" );
	video02.play();
	video02.setPaused( true );
	
	video03.loadMovie( "clouds_white_on_black.mov" );
	video03.play();
	video03.setPaused( true );
	
	videoPlayIndex = 0;
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp::update()
{
	audioIn.update();
	oscPad.update();
	
//	isVideo01Playing = oscPad.getButtonState( 1, 1 );
//	isVideo02Playing = oscPad.getButtonState( 2, 1 );
//	isVideo03Playing = oscPad.getButtonState( 3, 1 );
	
	if( isVideo01Playing )
	{
		if( videoPlayIndex != 0 )
		{
			videoPlayIndex = 0;
			video01.setPaused( false );
			video02.setPaused( true );
			video03.setPaused( true );
		}
	}
	
	if( isVideo02Playing )
	{
		if( videoPlayIndex != 1 )
		{
			videoPlayIndex = 1;
			video01.setPaused( true );
			video02.setPaused( false );
			video03.setPaused( true );
		}
	}
	
	if( isVideo03Playing )
	{
		if( videoPlayIndex != 2 )
		{
			videoPlayIndex = 2;
			video01.setPaused( true );
			video02.setPaused( true );
			video03.setPaused( false );
		}
	}
	
	if( isVideo01Playing )
	{
		opScope.update( video01.getPixels() );
	}
	
	if( isVideo02Playing )
	{
		opScope.update( video02.getPixels() );
	}

	if( isVideo03Playing )
	{
		opScope.update( video03.getPixels() );
	}
	
	opCirlce.setRotation( opCirlceRot += 0.1 );
	opCirlce.update();
	
	opBars.setAudioInData( audioIn.averages );
	opBars.update();
	
	opCheckers.update();
}

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp::draw()
{
	ofSetColor( 0xFFFFFF );
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );

//	if( isVideo01Playing )
//	{
//		video01.draw( 0, 0, video01.width, video01.height );
//	}
//	
//	if( isVideo02Playing )
//	{
//		video02.draw( 0, 0, video02.width, video02.height );
//	}
//
//	if( isVideo03Playing )
//	{
//		video03.draw( 0, 0, video03.width, video03.height );
//	}

	opScope.draw();
	opCheckers.draw();
	opBars.draw();
	opCirlce.draw();
	
	glDisable( GL_BLEND );

//	audioIn.draw();
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
		opCirlce.setRotation( opCirlceRot += 0.1 );
	}
	
	if( key == ';' )
	{
		opCirlce.setRotation( opCirlceRot -= 0.1 );
	}
	
	if( key == 'n' )
	{
		opBars.setAudioAvgMin( opBarsAudioAvgMin -= 1 );
	}

	if( key == 'm' )
	{
		opBars.setAudioAvgMin( opBarsAudioAvgMin += 1 );
	}
	
	if( key == 'j' )
	{
		opCheckers.setSize( opCheckersSize -= 1 );
	}

	if( key == 'k' )
	{
		opCheckers.setSize( opCheckersSize += 1 );
	}
	
	if( key == 'i' )
	{
		opCirlce.setInverse();
	}
	
	if( key == '1' )
	{
		isVideo01Playing = true;
		isVideo02Playing = false;
		isVideo03Playing = false;
	}

	if( key == '2' )
	{
		isVideo01Playing = false;
		isVideo02Playing = true;
		isVideo03Playing = false;
	}

	if( key == '3' )
	{
		isVideo01Playing = false;
		isVideo02Playing = false;
		isVideo03Playing = true;
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

