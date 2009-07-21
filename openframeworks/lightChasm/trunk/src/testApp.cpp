#include "testApp.h"
#include "Poco/Delegate.h"
#include "Poco/Timestamp.h"

void testApp::setup()
{
	ofSetFrameRate( 25 );
	ofBackground( 255, 255, 255 );
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
//	ofSetVerticalSync( true );
	ofSoundStreamSetup( 0, 2, this, 44100, 512, 4 );
	
//	ofAddListener( oscPad.oscPadChangeEvent, this, &testApp :: oscPadChangeEvent );
	
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
	audioInAvgPower.init();
}

void testApp :: initOsc ()
{
	oscReceiver.setup( 12345 );
	
//	oscPad.init( &oscReceiver );
//	oscPad.setOrientation( );
//	oscPad.setOffOnRelease( false );
//	oscPad.setExclusiveMode( true );
	
	oscRcvr.init( &oscReceiver );
	oscRcvr.addInput( "/1/fader0", &opCheckersSize );
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
	opCirlce.setCirlceResolution( opCirlceRes = 10 );
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
	videoIndex	= 0;
	videosTotal	= 3;
	
	videos		= new ofVideoPlayer[ videosTotal ];
//	videos[ 0 ].loadMovie( "squares_640x480.mov" );
//	videos[ 1 ].loadMovie( "koyaanisqatsi_sky_04_white_on_black.mov" );
//	videos[ 2 ].loadMovie( "clouds_white_on_black.mov" );

//	videos[ 0 ].loadMovie( "nionr_01.mov" );
//	videos[ 1 ].loadMovie( "nionr_02.mov" );
//	videos[ 2 ].loadMovie( "nionr_04.mov" );

	videos[ 0 ].loadMovie( "flight_01.mov" );
	videos[ 1 ].loadMovie( "nionr_02.mov" );
	videos[ 2 ].loadMovie( "flight_02.mov" );
	
	videos[ 0 ].play();
	
	videoPlayStates			= new bool[ videosTotal ];
	videoPlayStates[ 0 ]	= true;
	videoPlayStates[ 1 ]	= false;
	videoPlayStates[ 2 ]	= false;
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp::update()
{
	audioIn.update();
	float avgPowerScaled =  audioInAvgPower.getNormalisedValue( audioIn.averagePower );
//	printf( "avgPowerScaled: %4.2f \n", avgPowerScaled );
	
	updateOsc();
	
	updateVideo();
	
	opScope.update( videos[ videoIndex ].getPixels() );
	
	opCirlce.setRotation( opCirlceRot += 0.1 );
	opCirlce.setAudioInValue( avgPowerScaled );
	opCirlce.setRgbScale( slider01, slider02, slider03 );
	opCirlce.setCirlceResolution( opCirlceRes = slider05 * 200 );
	opCirlce.update();

	opBars.setAudioAvgMin( opBarsAudioAvgMin = slider04 * 100 );
	opBars.setAudioInData( audioIn.averages );
	opBars.update();
	
	opCheckers.setSize( opCheckersSize * 100 );
	opCheckers.update();
}

void testApp :: updateOsc ()
{
//	oscPad.update();
//	
//	if( oscPad.getButtonState( 1, 1 ) )
//	{
//		videoPlayStates[ 0 ]	= true;
//		videoPlayStates[ 1 ]	= false;
//		videoPlayStates[ 2 ]	= false;
//	}
//	
//	if( oscPad.getButtonState( 2, 1 ) )
//	{
//		videoPlayStates[ 0 ]	= false;
//		videoPlayStates[ 1 ]	= true;
//		videoPlayStates[ 2 ]	= false;
//	}
//	
//	if( oscPad.getButtonState( 3, 1 ) )
//	{
//		videoPlayStates[ 0 ]	= false;
//		videoPlayStates[ 1 ]	= false;
//		videoPlayStates[ 2 ]	= true;
//	}
//	
//	slider01 = oscPad.getSliderValue( 1 );
//	slider02 = oscPad.getSliderValue( 2 );
//	slider03 = oscPad.getSliderValue( 3 );
//	slider04 = oscPad.getSliderValue( 4 );
//	slider05 = oscPad.getSliderValue( 5 );
//	slider06 = oscPad.getSliderValue( 6 );
	
	oscRcvr.ping();
}

void testApp :: updateVideo ()
{
	for( int i=0; i<videosTotal; i++ )
	{
		if( !videoPlayStates[ i ] && videoIndex == i )
		{
			videos[ i ].stop();
		}
		
		if( videoPlayStates[ i ] && videoIndex != i )
		{
			videoIndex = i;
			videos[ i ].setPosition( 0 );
			videos[ i ].play();
		}
	}
}

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp::draw()
{
	ofSetColor( 0xFFFFFF );

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );

	opScope.draw();
	opCheckers.draw();	
	opBars.draw();
	opCirlce.draw();
	
	glDisable( GL_BLEND );

//	audioIn.draw();
	
	ofSetColor( 0xFF0000 );
	ofDrawBitmapString
	(
		"fps :: " + ofToString(ofGetFrameRate(), 2) + "\n\n",
		20,
		20
	);
}

//--------------------------------------------------------------

void testApp :: oscPadChangeEvent ( int &x )
{
//	printf( "x: %d y: %d\n", x, y );
	printf( "x: %d \n", x );
}

void testApp :: keyPressed(int key)
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
		videoPlayStates[ 0 ]	= true;
		videoPlayStates[ 1 ]	= false;
		videoPlayStates[ 2 ]	= false;
	}

	if( key == '2' )
	{
		videoPlayStates[ 0 ]	= false;
		videoPlayStates[ 1 ]	= true;
		videoPlayStates[ 2 ]	= false;
	}

	if( key == '3' )
	{
		videoPlayStates[ 0 ]	= false;
		videoPlayStates[ 1 ]	= false;
		videoPlayStates[ 2 ]	= true;
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

