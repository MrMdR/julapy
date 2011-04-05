#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofBackground( 0, 0, 0 );
//	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	bDebug		= true;
	bStepMode	= false;
	bStepOne	= false;
	
//	pt.loadImage( "perlin_noise.png" );
//	pt.loadImage( "old_man.jpg" );
	pt.loadImage( "bec_500x614.png" );
//	pt.loadImage( "old_postcard.png" );
//	pt.loadImage( "lossy_07.jpg" );
//	pt.addRandomParticles( 300 );
	
	pt.setup();
	
	tileSaver.init( 10, 0, true );
	
	screenGrabber.setup( "screengrab_movie/" );
	screenGrabber.setPause( true );
	
	initGui();
}

void testApp :: initGui ()
{
	gui.setAutoSave( false );
	
	gui.addTitle( "render toggles" );
	gui.addToggle( "bShowSourceImage  ",	pt.bShowSourceImage );
	gui.addToggle( "bShowTraceImage  ",		pt.bShowTraceImage );
	gui.addToggle( "bShowParticles  ",		pt.bShowParticles );
	gui.addToggle( "bShowParticleLines  ",	pt.bShowParticleLines );
	gui.addToggle( "bShowParticleStrip  ",	pt.bShowParticleStrip );
	gui.addToggle( "bShowParticleHead  ",	pt.bShowParticleHead );
	
	gui.addPage( "render params" );
	gui.addSlider( "lineWidth",				pt.lineWidth,	0.0, 3.0 );
	gui.addSlider( "lineAlpha",				pt.lineAlpha,	0.0, 1.0 );
	gui.addSlider( "traceAlpha",			pt.traceAlpha,	0.0, 1.0 );
	gui.addSlider( "traceBlur",				pt.traceBlur,	0, 10 );
	gui.addSlider( "minPosDist",			pt.minPosDist,	0, 40 );
	
	gui.addPage( "force toggles" );
	gui.addToggle( "bUseImageColour  ",		pt.bUseImageColour );
	gui.addToggle( "bUseImageForce  ",		pt.bUseImageForce );
	gui.addToggle( "bUseTraceForce  ",		pt.bUseTraceForce );
	gui.addToggle( "bUseWanderForce  ",		pt.bUseWanderForce );
	
	gui.addPage( "force params" );
	gui.addSlider( "velLimit",				pt.velLimit,		0.0, 10.0 );
	gui.addSlider( "velEase",				pt.velEase,			0.0, 1.0 );
	gui.addSlider( "imageVecScale",			pt.imageVecScale,	0.0, 500.0 );
	gui.addSlider( "traceVecScale",			pt.traceVecScale,	0.0, 500.0 );
	gui.addSlider( "wanderVecScale",		pt.wanderVecScale,	0.0, 1.0 );
	
	bDebug ? gui.show() : gui.hide();
	
	gui.setPage( 1 );
}

//--------------------------------------------------------------
void testApp::update()
{
	if( tileSaver.bGoTiling )
		return;
	
	if( bStepMode )
	{
		if( !bStepOne )
			return;
		
		bStepOne = false;
	}
	
	pt.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	tileSaver.begin();
	
	pt.draw( tileSaver.bGoTiling );
	
	tileSaver.end();
	
	if( tileSaver.bGoTiling )
		return;
	
	if( screenGrabber.isRecording() )
		screenGrabber.save();
	
	//---

	ofSetColor( 0xFF0000 );
	
	string msg = "";
	
	msg = "debug mode : ";
	msg += bDebug ? "ON" : "OFF";
	ofDrawBitmapString( msg, 20, ofGetHeight() - 100 );
	
	msg = "press 'd' to toggle debug mode.";
	ofDrawBitmapString( msg, 20, ofGetHeight() - 80 );
	
	msg = "step mode : ";
	msg += bStepMode ? "ON" : "OFF";
	ofDrawBitmapString( msg, 20, ofGetHeight() - 40 );
	
	msg = bStepMode ? "press space to step" : "press shift + space to toggle step mode";
	ofDrawBitmapString( msg, 20, ofGetHeight() - 20 );
	
	//---
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
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
	
	if( key == 's' )
	{
		char str[255];
		sprintf( str, "screengrab/image_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
		
		ofImage img;
		img.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
		img.saveImage( str );
		img.clear();
	}
	
	if( key == 't' )
	{
		char str[255];
		sprintf( str, "screengrab_lrg/image_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
		
		tileSaver.finish( str, false );
	}
	
	if( key == 'm' )
	{
		screenGrabber.togglePause();
	}
	
	if( key == 'k' )
	{
		pt.killAllParticles();
	}
	
	if( key >= '0' && key <= '9' )
	{
		gui.setPage( key - '0' );
		gui.show();
	}
	else
	{
		switch(key)
		{
			case '[': gui.prevPage(); break;
			case ']': gui.nextPage(); break;
		}
	}
	
	if( key == ' ' )
	{
		if( glutGetModifiers() == GLUT_ACTIVE_SHIFT )
		{
			bStepMode = !bStepMode;
		}
		else
		{
			bStepOne = true;
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	pt.keyReleased( key );
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
	pt.mouseMoved( x, y );
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	pt.mouseDragged( x, y, button );
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	pt.addParticle( x, y, true );
	pt.mousePressed( x, y, button );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	pt.mouseReleased( x, y, button );
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
	pt.windowResized( w, h );
}

