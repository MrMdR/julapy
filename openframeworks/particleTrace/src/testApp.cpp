#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofBackground( 0, 0, 0 );
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	bDebug = false;
	
//	pt.loadImage( "perlin_noise.png" );
	pt.loadImage( "old_man.jpg" );
	pt.addRandomParticles( 500 );
	pt.setup();
	
	tileSaver.init( 10, 0, true );
	
	screenGrabber.setup( "screengrab_movie/" );
	screenGrabber.setPause( true );
	
	initGui();
}

void testApp :: initGui ()
{
	gui.setAutoSave( false );
	
	gui.addTitle( "toggles" );
	
	gui.addToggle( "bShowSourceImage  ",	pt.bShowSourceImage );
	gui.addToggle( "bShowTraceImage  ",		pt.bShowTraceImage );
	gui.addToggle( "bShowParticles  ",		pt.bShowParticles );
	gui.addToggle( "bUseImageColour  ",		pt.bUseImageColour );
	gui.addToggle( "bEnableImageForce  ",	pt.bEnableImageForce );
	gui.addToggle( "bEnableTraceForce  ",	pt.bEnableTraceForce );
	gui.addToggle( "bDrawParticleLines  ",	pt.bDrawParticleLines );
	gui.addToggle( "bDrawParticleStrip  ",	pt.bDrawParticleStrip );
	
	gui.addPage( "particle params" );
	gui.addSlider( "lineWidth", pt.lineWidth, 0.0, 1.0 );
	
	if( bDebug )
	{
		gui.show();
	}
	else
	{
		gui.hide();
	}
	
	gui.setPage( 1 );
	
//	gui.loadFromXML();
}

//--------------------------------------------------------------
void testApp::update()
{
	if( tileSaver.bGoTiling )
		return;
	
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
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	pt.keyPressed( key );
	
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

