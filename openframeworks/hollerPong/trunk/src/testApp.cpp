#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofEnableSmoothing();
	
	haarFinderImage.allocate( 160, 120 );
	haarFaceImage.allocate( 80, 80 );
	haarFinder.setup( "haarXML/haarcascade_frontalface_default.xml" );
	bHaarFaceFound	= false;
	
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
	
	if( ct.video.isFrameNew() )
	{
		updateHaarFinder( ct.colImg );
	}
	
	const ColorTrackerData* data;
	data = ct.getTrackerData();

	updatePongPaddles( data[ 0 ].cy, data[ 1 ].cy );
	updatePongFlip( data[ 0 ].cx, data[ 1 ].cx );
	updatePongPause( data[ 2 ].active );
	updatePongReset( data[ 3 ].active );
		
	pong.update();
}

void testApp :: updateHaarFinder ( ofxCvColorImage& colImg )
{
	ofxCvGrayscaleImage grayImg;
	grayImg.allocate( colImg.width, colImg.height );
	grayImg = colImg;
	
	float haarScale;
	haarScale = ct.videoRect.width / (float)haarFinderImage.width;
	
	haarFinderImage.scaleIntoMe( grayImg );
	haarFinder.findHaarObjects( haarFinderImage );

	int noHaarBlobs;
	noHaarBlobs = haarFinder.blobs.size();
	
	bHaarFaceFound = ( noHaarBlobs > 0 );
	
	if( bHaarFaceFound )
	{
		int i;
		i= (int)( ofRandom( 0, noHaarBlobs ) );
		
		haarRect.x		= haarFinder.blobs[ i ].boundingRect.x;
		haarRect.y		= haarFinder.blobs[ i ].boundingRect.y;
		haarRect.width	= haarFinder.blobs[ i ].boundingRect.width;
		haarRect.height = haarFinder.blobs[ i ].boundingRect.height;
		
		ofRectangle haarRectScaled;
		haarRectScaled.x		= haarRect.x * haarScale;
		haarRectScaled.y		= haarRect.y * haarScale;
		haarRectScaled.width	= haarRect.width * haarScale;
		haarRectScaled.height	= haarRect.height * haarScale;
		
		ofxCvColorImage imageCopy;							// have to copy image due to bug with roi.
		imageCopy.allocate( colImg.width, colImg.height );
		imageCopy = colImg;
		imageCopy.setROI( haarRectScaled );
		
		ofxCvColorImage imageFace;
		imageFace.allocate( haarRectScaled.width, haarRectScaled.height );
		imageFace.setFromPixels( imageCopy.getRoiPixels(), haarRectScaled.width, haarRectScaled.height );
		
		haarFaceImage.scaleIntoMe( imageFace );
	}
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
		
		drawHaar();
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
		
		ofPoint p;
		p = pong.getBallPosition();
		haarFaceImage.draw
		(
			p.x - haarFaceImage.width  * 0.5,
			p.y - haarFaceImage.height * 0.5
		);
		
		pong.drawScore();
		pong.drawPaused();
		pong.drawReset();
		
		ofSetColor( 0xFFFFFF );
		ofDrawBitmapString( "press 'd' for debug screen", 20, ofGetHeight() - 10 );
		ofDrawBitmapString( "press 'f' for full screen", ofGetWidth() - 220, ofGetHeight() - 10 );
	}
}

void testApp :: drawHaar ()
{
	if( !bHaarFaceFound )
		return;
	
	float haarScale;
	haarScale = ct.videoRect.width / (float)haarFinderImage.width;
	
	ofRectangle haarRectScaled;
	haarRectScaled.x		= haarRect.x * haarScale;
	haarRectScaled.y		= haarRect.y * haarScale;
	haarRectScaled.width	= haarRect.width * haarScale;
	haarRectScaled.height	= haarRect.height * haarScale;
	
	haarRectScaled.x += ct.videoRect.x;
	haarRectScaled.y += ct.videoRect.y;
	
	ofNoFill();
	ofSetLineWidth( 3 );
	ofSetColor( 255, 0, 255 );
	ofRect
	(
		haarRectScaled.x,
		haarRectScaled.y,
		haarRectScaled.width,
		haarRectScaled.height
	);
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

