#include "testApp.h"

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofBackground( 255, 255, 255 );
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofSetVerticalSync( true );
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
	audioInAvgPower.init();
}

void testApp :: initOsc ()
{
	oscReceiver.setup( 12345 );
	
	oscRcvr.init( &oscReceiver );
	oscRcvr.addInput( "/1/fader0", &opCheckersSizeScale );
//	oscRcvr.addInput( "/1/fader1", & );
	oscRcvr.addInput( "/1/fader2", &opBarsAudioAvgMinScale );
	oscRcvr.addInput( "/1/fader3", &opCirlceResScale );
	oscRcvr.addInput( "/1/fader4", &opCirlceRotScale );
	oscRcvr.addInput( "/1/fader5", &opCirlceColor );
}

void testApp :: initOpCheckers ()
{
	opCheckersSize		= 150;
	opCheckersSizeScale	= 0.5f;
	
	opCheckers.init( 640, 480 );
	opCheckers.setSize( opCheckersSize * opCheckersSizeScale );
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
	
	opCirlceRot			= 0.0f;
	opCirlceRotScale	= 0.0f;
	opCirlceRes			= MIN_CIRCLE_PTS;
	opCirlceResScale	= 0.5f;
	opCirlceColor		= 0.0f;
	
	opCirlce.x		= rx;
	opCirlce.y		= ry;
	opCirlce.radius	= sqrt( rx * rx + ry * ry ) * 1.5f;
	opCirlce.setCirlceResolution( opCirlceRes = (int)( opCirlceResScale * MAX_CIRCLE_PTS ) );
	opCirlce.setRotation( opCirlceRot );
}

void testApp :: initOpBars ()
{
	opBarsAudioAvgMin		= 60;
	opBarsAudioAvgMinScale	= 0.5f;
	
	opBars.init( 640, 480 );
	opBars.setNumberOfBars( audioIn.averagesTotal );
	opBars.setAudioAvgMin( opBarsAudioAvgMin * opBarsAudioAvgMinScale );
}

void testApp :: initVideos ()
{
	videoIndex	= 0;
	videosTotal	= 3;
	
	videos = new ofVideoPlayer[ videosTotal ];
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
	
	opCirlce.setRotation( opCirlceRot += opCirlceRotScale * 1.0 + 0.1 );
	opCirlce.setAudioInValue( avgPowerScaled );
	opCirlce.setRgbScale( opCirlceColor, opCirlceColor, opCirlceColor );
	opCirlce.setCirlceResolution( opCirlceRes = (int)( opCirlceResScale * ( ( MAX_CIRCLE_PTS - 1 ) * 0.5 ) ) * 2 );
	opCirlce.update();

	opBars.setAudioAvgMin( opBarsAudioAvgMin * opBarsAudioAvgMinScale );
	opBars.setAudioInData( audioIn.averages );
	opBars.update();
	
	opCheckers.setSize( MAX( 3, opCheckersSize * opCheckersSizeScale ) );
	opCheckers.update();
}

void testApp :: updateOsc ()
{
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

void testApp :: keyPressed(int key)
{
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

