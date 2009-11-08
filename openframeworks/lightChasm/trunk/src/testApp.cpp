#include "testApp.h"

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofBackground( 255, 255, 255 );
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofSetVerticalSync( true );
	
	initRenderArea();
	initAudioIn();
	initOpScope();
	initOpCheckers();
	initOpCircle();
	initOpBars();
	initOpRain();
	initVideos();
	initOsc();
	initGui();
	
	screenGrabUtil.setup( "movie/lightchasm", &renderArea );
	
	bDebug			= true;
	bRightMonitor	= false;
}

///////////////////////////////////////////////////
//	INIT.
///////////////////////////////////////////////////

void testApp :: initRenderArea()
{
	renderAreaWindow.x      = 0;
	renderAreaWindow.y      = 0;
	renderAreaWindow.width  = 1280;
	renderAreaWindow.height = 720;
	
	renderAreaFullScreen.x      = 0;
	renderAreaFullScreen.y      = 0;
	renderAreaFullScreen.width  = 1440;
	renderAreaFullScreen.height = 900;
	
	renderAreaRightMonitor.x		= 1440;
	renderAreaRightMonitor.y		= 0;
	renderAreaRightMonitor.width	= 640;
	renderAreaRightMonitor.height	= 480;
	
	renderArea.x		= renderAreaWindow.x;
	renderArea.y		= renderAreaWindow.y;
	renderArea.width	= renderAreaWindow.width;
	renderArea.height	= renderAreaWindow.height;
}

void testApp :: initAudioIn ()
{
	audio = new AudioLiveSpectrum();
	audio->init( "" );
	
	audioInAvgPower.init();
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
	opCirlceResScale	= 0.1f;
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
	opBars.setNumberOfBars( audio->fftAveragePower );
	opBars.setAudioAvgMin( opBarsAudioAvgMin * opBarsAudioAvgMinScale );
}

void testApp :: initOpRain()
{
	opRainMakeRate		= 0.2;
	opRainMakeRateScale	= 1.0;
	
	opRain.init( 640, 480 );
}

void testApp :: initVideos ()
{
	videosTotal				= 3;
	videoPosition			= 0;
	videoPositionOsc		= 0;
	videoPositionOverride	= false;
	
	videos = new VideoObj[ videosTotal ];
	videos[ 0 ].video.loadMovie( "flight_01_inv.mov" );
	videos[ 1 ].video.loadMovie( "nionr_02.mov" );
	videos[ 2 ].video.loadMovie( "flight_02.mov" );
	
	for( int i=0; i<videosTotal; i++ )
	{
		videos[ i ].video.setPaused( true );
		videos[ i ].duration	= videos[ i ].video.getDuration();
		videos[ i ].frames		= videos[ i ].video.getTotalNumFrames();
		videos[ i ].startTime	= 0;
		videos[ i ].startFrame	= 0;
		videos[ i ].playing		= false;
		videos[ i ].oscPlaying	= false;
	}
}

void testApp :: initOsc ()
{
	oscReceiver.setup( 12345 );
	
	oscRcvr.init( &oscReceiver );
	oscRcvr.addInput( "/1/fader0", &opCirlceResScale );
	oscRcvr.addInput( "/1/fader1", &opCirlceRotScale );
	oscRcvr.addInput( "/1/fader2", &opCheckersSizeScale );
	oscRcvr.addInput( "/1/fader3", &opBarsAudioAvgMinScale );
	oscRcvr.addInput( "/1/fader4", &opRainMakeRateScale );
	oscRcvr.addInput( "/1/fader5", &videoPositionOsc );
	
	oscRcvr.addInput( "/1/toggle1", &videos[ 0 ].oscPlaying );
	oscRcvr.addInput( "/1/toggle2", &videos[ 1 ].oscPlaying );
	oscRcvr.addInput( "/1/toggle3", &videos[ 2 ].oscPlaying );
	oscRcvr.addInput( "/1/toggle4", &videoPositionOverride );
}

void testApp :: initGui ()
{
	gui.addToggle( "bRightMonitor",	&bRightMonitor );
	
	gui.loadFromXML( "ofxSimpleGuiToo.xml" );
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp::update()
{
	audio->update();
	audioInAvgPower.addValue( audio->fftAveragePower );
	float avgPowerScaled =  audioInAvgPower.getNormalisedValue();
//	printf( "avgPowerScaled: %4.2f \n", avgPowerScaled );
	
	updateOsc();
	
	updateVideo();
	
	opCirlce.setRotation( opCirlceRot += opCirlceRotScale * 1.0 + 0.1 );
	opCirlce.setAudioInValue( avgPowerScaled );
	opCirlce.setRgbScale( opCirlceColor, opCirlceColor, opCirlceColor );
	opCirlce.setCirlceResolution( opCirlceRes = (int)( opCirlceResScale * ( ( MAX_CIRCLE_PTS - 1 ) * 0.5 ) ) * 2 );
	opCirlce.update();

	opBars.setAudioAvgMin( opBarsAudioAvgMin * opBarsAudioAvgMinScale );
	opBars.setAudioInData( audio->fftData );
	opBars.update();
	
	opCheckers.setSize( MAX( 3, opCheckersSize * opCheckersSizeScale ) );
	opCheckers.update();

	opRainMakeRate = avgPowerScaled;
	opRain.setMakeRate( opRainMakeRate * opRainMakeRateScale );
	opRain.update();
}

void testApp :: updateOsc ()
{
	oscRcvr.ping();
}

void testApp :: updateVideo ()
{
	videoPosition += ( videoPositionOsc - videoPosition ) * 0.02;
	
	for( int i=0; i<videosTotal; i++ )
	{
		if( videos[ i ].playing != videos[ i ].oscPlaying )	// osc toggle button has changed.
		{
			videos[ i ].playing = videos[ i ].oscPlaying;
			
			if( videos[ i ].playing )
			{
				videos[ i ].startTime  = ofGetElapsedTimef();
				videos[ i ].startFrame = ofGetFrameNum();
			}
		}
		
		if( videos[ i ].playing )
		{
			float t = ( ofGetFrameNum() - videos[ i ].startFrame ) / videos[ i ].frames;
			if( t > 1 )
			{
				t = 0;
				videos[ i ].startTime  = ofGetElapsedTimef();
				videos[ i ].startFrame = ofGetFrameNum();
			}
			
			if( videoPositionOverride )
			{
				videos[ i ].video.setPosition( videoPosition );
			}
			else
			{
				videos[ i ].video.setPosition( t );
			}
		}
	}
}

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp::draw()
{
	opRain.drawToFBO();
	opCirlce.drawToFBO();

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );
	
	glPushMatrix();
	glTranslatef( renderArea.x, renderArea.y, 0 );
	
	drawVideos();
	opCheckers.draw();
	opBars.draw();
	opCirlce.drawFBOToScreen();
	opRain.drawFBOToScreen();
	
	glPopMatrix();
	
	if( bDebug )
	{
		gui.draw();
		
		glPushMatrix();
		glTranslatef( 10, renderArea.height - 210, 0 );
		
		audio->draw( 400, 200 );
		
		glPopMatrix();
	}
	
	glDisable( GL_BLEND );
	
	if( screenGrabUtil.isRecording() )
		screenGrabUtil.save();
	
	drawDebug();
}

void testApp :: drawVideos()
{
	for( int i=0; i<videosTotal; i++ )
	{
		if( videos[ i ].playing )
		{
			opScope.update( videos[ i ].video.getPixels() );
			opScope.draw();
		}
	}
}

void testApp :: drawDebug()
{
	ofSetColor( 0xFF0000 );
	ofDrawBitmapString
	(
		"",
		20,
		20
	);
}

////////////////////////////////////////////////////////
// TOGGLE FULL SCREEN.
////////////////////////////////////////////////////////

void testApp :: updateRenderArea()
{
	if( ofGetWindowMode() == OF_WINDOW )
	{
		renderArea.x		= renderAreaWindow.x;
		renderArea.y		= renderAreaWindow.y;
		renderArea.width	= renderAreaWindow.width;
		renderArea.height	= renderAreaWindow.height;
	}
	
	if( ofGetWindowMode() == OF_FULLSCREEN )
	{
		if( bRightMonitor )
		{
			renderArea.x		= renderAreaRightMonitor.x;
			renderArea.y		= renderAreaRightMonitor.y;
			renderArea.width	= renderAreaRightMonitor.width;
			renderArea.height	= renderAreaRightMonitor.height;
		}
		else
		{
			renderArea.x		= renderAreaFullScreen.x;
			renderArea.y		= renderAreaFullScreen.y;
			renderArea.width	= renderAreaFullScreen.width;
			renderArea.height	= renderAreaFullScreen.height;
		}
	}
	
	screenGrabUtil.setArea( &renderArea );
}

//--------------------------------------------------------------

void testApp :: keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
		updateRenderArea();
	}
	
	if( key == 'm' )
	{
		if( screenGrabUtil.isRecording() )
			screenGrabUtil.stop();
		else
			screenGrabUtil.start();
	}
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
	
	if( key == 'r' )
	{
		bRightMonitor = !bRightMonitor;
		
		updateRenderArea();
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