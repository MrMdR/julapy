#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofEnableSmoothing();
	
	bShowDebug		= false;
	bUseMouse		= false;
	
	bPongPaused0	= false;
	bPongPaused1	= false;
	
	ct.init( 4 );
	ct.loadFromFile();
	
	pong.init();
	
	p1 = 0;
	p2 = 0;
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	ct.update();
	
	const ColorTrackerData* data;
	data = ct.getTrackerData();

	updatePongPaddles( data[ 0 ].cy, data[ 1 ].cy );
	updatePongFlip( data[ 0 ].cx, data[ 1 ].cx );
	updatePongPause( data[ 2 ].active );
	updatePongReset( data[ 3 ].active );
		
	pong.update();
}

void testApp :: updatePongPaddles ( float p1y, float p2y )
{
	if( bUseMouse )
		return;
	
	float ease;
	ease = 0.5;
	
	float stretch;		// stretch y position, so the paddles handle a better when hitting top and bottom of screen.
	stretch = 0.4;
	
	p1y = ( p1y * ( 1 + stretch ) ) - stretch * 0.5;
	p2y = ( p2y * ( 1 + stretch ) ) - stretch * 0.5;
	
	p1 += ( p1y - p1 ) * ease;
	p2 += ( p2y - p2 ) * ease;
	
	pong.setPlayerOnePosition( p1 );
	pong.setPlayerTwoPosition( p2 );
}

void testApp :: updatePongFlip ( float p1x, float p2x )
{
	if( bUseMouse )
		return;

	bool flipSides;
	flipSides = ( p1x > p2x );
	
	pong.flipSides( flipSides );
}

void testApp :: updatePongPause ( bool bPongPaused )
{
	if( bUseMouse )
		return;
	
	if( bPongPaused0 != bPongPaused )
	{
		bPongPaused0  = bPongPaused;
		
		if( bPongPaused0 )
		{
			bPongPaused1 = !bPongPaused1;
			
			pong.setPaused( bPongPaused1 );
		}
	}
}

void testApp :: updatePongReset ( bool bPongReset )
{
	if( bPongReset )
	{
		pong.reset();
	}
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	if( bShowDebug )
	{
		ct.draw();
	}
	else 
	{
		ofFill();
		ofSetColor( 0x000000 );
		ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
		
		pong.drawBackdropStars();
		
		drawVideo();
		
		pong.drawBackdropDivider();
		pong.drawPaddles();
		pong.drawBall();
		pong.drawScore();
		pong.drawPaused();
		pong.drawReset();
		
		ofSetColor( 0xFFFFFF );
		ofDrawBitmapString( "press 'd' for debug screen", 20, ofGetHeight() - 10 );
		ofDrawBitmapString( "press 'f' for full screen", ofGetWidth() - 220, ofGetHeight() - 10 );
	}
}

void testApp :: drawVideo ()
{
	int x, y, w, h, b;

	b = 8;
	w = ct.videoRect.width;
	h = ct.videoRect.height;
	x = (int)( ( ofGetWidth() - w ) * 0.5 );
	y = ofGetHeight() - h - b;

	ofEnableAlphaBlending();
	
	ofFill();
	ofSetColor( 255, 255, 255, 60 );
	
	ofRect( x - b, y - b, w + b * 2, b );
	ofRect( x - b, ofGetHeight() - b, w + b * 2, b );
	ofRect( x - b, y, b, h );
	ofRect( x + w, y, b, h );
	
	ofSetColor( 255, 255, 255, 120 );
	
	ct.colImg.draw( x, y );
	
	ofDisableAlphaBlending();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	ct.keyPressed( key );
	
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
	
	if( key == 'd' )
	{
		bShowDebug = !bShowDebug;
		
		if( bShowDebug )
		{
			pong.setPaused( true );
		}
		else
		{
			pong.setPaused( false );
		}
	}
	
	if( key == 'm' )
	{
		bUseMouse = !bUseMouse;
	}
	
	if( key == 'p' )
	{
		pong.togglePause();
	}
}

void testApp::keyReleased(int key)
{
	//
}

void testApp::mouseMoved(int x, int y )
{
	ct.mouseMoved( x, y );
	
	float p;
	p = y / (float)ofGetHeight();
	
	if( bUseMouse )
	{
		pong.setPlayerOnePosition( p );
		pong.setPlayerTwoPosition( p );
	}
}

void testApp::mouseDragged(int x, int y, int button)
{
	ct.mouseDragged( x, y, button );
}

void testApp::mousePressed(int x, int y, int button)
{
	ct.mousePressed( x, y, button );
}

void testApp::mouseReleased(int x, int y, int button)
{
	ct.mouseReleased();
}

void testApp::windowResized(int w, int h)
{

}

