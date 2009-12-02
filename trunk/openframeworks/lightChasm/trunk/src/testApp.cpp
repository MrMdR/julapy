#include "testApp.h"

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofBackground( 0, 0, 0 );
//	ofEnableSmoothing();
	ofSetCircleResolution( 30 );
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
	initMaskImage();
	
	screenGrabUtil.setup( "movie/lightchasm", &renderArea );
	
	bDebug			= true;
	bRightMonitor	= false;
	
	fadeBlack		= 0.0;
	
	autoOverride		= true;
	autoMode			= 0;
	autoModesTotal		= 3;
	autoModeStartTime	= 0;
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
//	renderAreaFullScreen.width  = 1440;
//	renderAreaFullScreen.height = 900;
	renderAreaFullScreen.width  = 1280;			// 13" macbook.
	renderAreaFullScreen.height = 800;

	renderAreaRightMonitor.x		= 1280;		// 13" macbook.
//	renderAreaRightMonitor.x		= 1440;
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
	opBarsAudioAvgMin		= 1.0;
	opBarsAudioAvgMinScale	= 0.5f;
	
	int opBarsTotal;
	opBarsTotal = 10;
	
	opBarsAudioData = new float[ opBarsTotal ];
	
	opBars.init( 640, 480 );
	opBars.setNumberOfBars( opBarsTotal );
	opBars.setAudioAvgMin( opBarsAudioAvgMin * ( 1 - opBarsAudioAvgMinScale ) );
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
	videos[ 0 ].video.loadMovie( "video_01.mov" );
	videos[ 1 ].video.loadMovie( "video_02.mov" );
	videos[ 2 ].video.loadMovie( "video_03.mov" );
	
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

void testApp :: initMaskImage()
{
//	maskImage.loadImage( "mask.jpg" );
	maskImage.loadImage( "mask.png" );
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp :: update()
{
	audio->update();
	audio->getFftPeakData( opBarsAudioData, opBars.getNumberOfBars() );
	audioInAvgPower.addValue( audio->fftAveragePower );
	float avgPowerScaled =  audioInAvgPower.getNormalisedValue();
	
	if( autoOverride )
		updateAutoMode();
	else
		updateOsc();
	
	updateVideo();
	
	opCirlce.setRotation( opCirlceRot += opCirlceRotScale * 1.0 + 0.1 );
	opCirlce.setAudioInValue( avgPowerScaled );
	opCirlce.setRgbScale( opCirlceColor, opCirlceColor, opCirlceColor );
	opCirlce.setCirlceResolution( opCirlceRes = (int)( opCirlceResScale * ( ( MAX_CIRCLE_PTS - 1 ) * 0.5 ) ) * 2 );
	opCirlce.update();

	opBars.setAudioAvgMin( opBarsAudioAvgMin * ( 1 - opBarsAudioAvgMinScale ) );
	opBars.setAudioInData( opBarsAudioData );
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

void testApp :: updateAutoMode ()
{
	autoModeElapsedTime = ofGetElapsedTimeMillis() - autoModeStartTime;
	
	if( autoMode == 0 )
	{
		opCirlceResScale		= 0.1;
		opCirlceRotScale		= cos( autoModeElapsedTime * 0.0005            ) * 0.5 + 0.5;
		opCheckersSizeScale		= cos( autoModeElapsedTime * 0.00002           ) * 0.5 + 0.5;
		opBarsAudioAvgMinScale	= 0.0;
		opRainMakeRateScale		= sin( autoModeElapsedTime * 0.0005  - HALF_PI ) * 0.5 + 0.5;
		videos[ 0 ].oscPlaying	= false;
		videos[ 1 ].oscPlaying	= false;
		videos[ 2 ].oscPlaying	= false;
	}

	if( autoMode == 1 )
	{
		opCirlceResScale		= cos( autoModeElapsedTime * 0.00002           ) * 0.5 + 0.5;
		opCirlceRotScale		= sin( autoModeElapsedTime * 0.001   - HALF_PI ) * 0.5 + 0.5;
		opCheckersSizeScale		= sin( autoModeElapsedTime * 0.00002 - HALF_PI ) * 0.5 + 0.5;
		opBarsAudioAvgMinScale	= 0.0;
		opRainMakeRateScale		= sin( autoModeElapsedTime * 0.00002 - HALF_PI ) * 0.5 + 0.5;
		videos[ 0 ].oscPlaying	= false;
		videos[ 1 ].oscPlaying	= false;
		videos[ 2 ].oscPlaying	= false;
	}

	if( autoMode == 2 )
	{
		opCirlceResScale		= 0.0;
		opCirlceRotScale		= 0.0;
		opCheckersSizeScale		= sin( autoModeElapsedTime * 0.00002 - HALF_PI ) * 0.5 + 0.5;
		opBarsAudioAvgMinScale	= 1.0;
		opRainMakeRateScale		= 0.0;
		videos[ 0 ].oscPlaying	= true;
		videos[ 1 ].oscPlaying	= false;
		videos[ 2 ].oscPlaying	= false;
	}

	if( autoMode == 3 )
	{
		opCirlceResScale		= sin( autoModeElapsedTime * 0.00002 - HALF_PI ) * 0.5 + 0.5;
		opCirlceRotScale		= 0.0;
		opCheckersSizeScale		= cos( autoModeElapsedTime * 0.00002           ) * 0.5 + 0.5;
		opBarsAudioAvgMinScale	= 1.0;
		opRainMakeRateScale		= 0.0;
		videos[ 0 ].oscPlaying	= false;
		videos[ 1 ].oscPlaying	= true;
		videos[ 2 ].oscPlaying	= false;
	}
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
	opCheckers.drawToFBO();
	opRain.drawToFBO();
	opCirlce.drawToFBO();

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );
	
	glPushMatrix();
	glTranslatef( renderArea.x, renderArea.y, 0 );
	
	drawVideos();
	opCheckers.drawFBOToScreen();
	opBars.draw();
	opCirlce.drawFBOToScreen();
	opRain.drawFBOToScreen();
	
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );		// alpha blend
	maskImage.draw( 0, 0 );
	
	drawBlack();
	
	glPopMatrix();
	
	glBlendFunc( GL_ONE, GL_ZERO );
	
	if( bDebug )
	{
		gui.draw();
		
		glPushMatrix();
		glTranslatef( 10, renderArea.height - 210, 0 );
		
		audio->draw( 400, 200 );
		
		glPopMatrix();
	}
	
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

void testApp :: drawBlack ()
{
	ofEnableAlphaBlending();
	
	ofSetColor( 0, 0, 0, (int)( 255 * fadeBlack ) );
	ofRect( 0, 0, renderArea.width, renderArea.height );
	
	ofDisableAlphaBlending();
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
	
	if( key == '-' )
		fadeBlack = MIN( 1, MAX( 0, fadeBlack + 0.01 ) );
	
	if( key == '=' )
		fadeBlack = MIN( 1, MAX( 0, fadeBlack - 0.01 ) );
	
	if( key == '1' || key == '2' || key == '3' || key == '4' || key == '5' ||
		key == '6' || key == '7' || key == '8' || key == '9' || key == '0' )
	{
		autoModeStartTime = ofGetElapsedTimeMillis();
		
		switch ( key )
		{
			case '1':
				autoMode = 0;
				break;
				
			case '2':
				autoMode = 1;
				break;
				
			case '3':
				autoMode = 2;
				break;
				
			case '4':
				autoMode = 3;
				break;
				
			case '5':
				return;
				break;
				
			case '6':
				return;
				break;
				
			case '7':
				return;
				break;
				
			case '8':
				return;
				break;
				
			case '9':
				return;
				break;
				
			case '0':
				return;
				break;
		}
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