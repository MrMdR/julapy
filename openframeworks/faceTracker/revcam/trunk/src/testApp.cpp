#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{	 
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );
	
	vidGrabber.setVerbose( true );
	vidGrabber.initGrabber( WIDTH, HEIGHT );
        
	colorLargeImage.allocate( WIDTH, HEIGHT );
	colorSmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
    grayLargeImage.allocate( WIDTH, HEIGHT );
	graySmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
	
	sourceToSampleScale = WIDTH / SAMPLE_WIDTH;
	sampleToSourceScale	= SAMPLE_WIDTH / (float)WIDTH;

	haarFinder.setup( "haarXML/haarcascade_frontalface_default.xml" );
	haarTracker.setup( &haarFinder );
	
//	loadCarlaWin();
//	loadDaftPunkWin();
	loadMarianneWin();
}

void testApp :: loadCarlaWin()
{
	for( int i=0; i<72; i++ )
	{
		string imagePath = "animations/carla/carla_win" + ofToString( i ) + ".png";
		carla_win.addImage( imagePath );
	}
}

void testApp :: loadDaftPunkWin()
{
	for( int i=0; i<94; i++ )
	{
		string imagePath = "animations/daftpunk/daftpunk_win" + ofToString( i ) + ".png";
		daftpunk_win.addImage( imagePath );
	}
}

void testApp :: loadMarianneWin()
{
	for( int i=0; i<102; i++ )
	{
		string imagePath = "animations/marianne/marianne_win" + ofToString( i ) + ".png";
		marianne_win.addImage( imagePath );
	}
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground( 10, 10, 10 );
    
    bool bNewFrame = false;
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if( bNewFrame )
	{
		colorLargeImage.setFromPixels( vidGrabber.getPixels(), WIDTH, HEIGHT );

		colorLargeImage.mirror( false, true );
		colorSmallImage.scaleIntoMe( colorLargeImage );
		
		grayLargeImage	= colorLargeImage;
		graySmallImage	= colorSmallImage;
		
		haarTracker.findHaarObjects( graySmallImage );
	}
	
	carla_win.nextFrame();
	daftpunk_win.nextFrame();
	marianne_win.nextFrame();
}

//--------------------------------------------------------------
void testApp::draw()
{
	int i;
	float x, y, w, h;
	float wRatio, hRatio, scale;
	int faceID;
	float faceMode;
	
	#ifdef DEBUG_MODE
	#else
	
		wRatio = ofGetWidth() / (float)WIDTH;
		hRatio = ofGetHeight() / (float)HEIGHT;
	
		scale = MIN( wRatio, hRatio );
		
		glPushMatrix();
		glTranslatef
		( 
			(int)( ( ofGetWidth() - ( WIDTH * scale ) ) * 0.5f ),
			(int)( ( ofGetHeight() - ( HEIGHT * scale ) ) * 0.5f ),
			0
		);
		glScalef( scale, scale, 0 );
	
	#endif
	
	ofSetColor( 0xffffff );
	grayLargeImage.draw( 0, 0 );
	
	#ifdef DEBUG_MODE
		graySmallImage.draw( WIDTH, 0 );
	#endif

	while( haarTracker.hasNextHaarItem() )
	{
		faceID		= haarTracker.getHaarItemID();
		faceMode	= ( faceID % 10 ) / 10.0f;
		
		haarTracker.getHaarItemPropertiesEased( &x, &y, &w, &h );
		
		x	*= sourceToSampleScale;
		y	*= sourceToSampleScale;
		w	*= sourceToSampleScale;
		h	*= sourceToSampleScale;

		if( faceMode > 0.66 )
		{
			//
		}
		else if( faceMode > 0.33 )
		{
			//
		}
		else
		{
			//
		}
		
		drawMarianneWin( x, y, w, h );
	}
	
	#ifdef DEBUG_MODE
	#else
		glPopMatrix();
	#endif
}

void testApp :: drawCarlaWin ( float x, float y, float w, float h )
{
	float dx = 0.03;
	float dy = -0.25;
	float dw = 2.15;
	float dh = 2.15;
	float ws = h / carla_win.height() * carla_win.width();
	float hs = w / carla_win.width() * carla_win.height();
	
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	carla_win.draw
	(
		x - w * ( dw - 1 ) * 0.5 + dx * w,
		y - h * ( dh - 1 ) * 0.5 + dy * h,
		w * dw,
		hs * dh
	);
	ofDisableAlphaBlending();
}

void testApp :: drawDaftPunkWin( float x, float y, float w, float h )
{
	float dx = 0;
	float dy = -0.4;
	float dw = 1.9;
	float dh = 2.0;
	float ws = h / daftpunk_win.height() * daftpunk_win.width();
	float hs = w / daftpunk_win.width() * daftpunk_win.height();
	
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	daftpunk_win.draw
	(
		x - w * ( dw - 1 ) * 0.5 + dx * w,
		y - h * ( dh - 1 ) * 0.5 + dy * h,
		w * dw,
		hs * dh
	);
	ofDisableAlphaBlending();
}

void testApp :: drawMarianneWin( float x, float y, float w, float h )
{
	float dx = -1.28;
	float dy = 0.7;
	float dw = 4.8;
	float dh = 4.8;
	float ws = h / marianne_win.height() * marianne_win.width();
	float hs = w / marianne_win.width() * marianne_win.height();
	
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	marianne_win.draw
	(
		x - w * ( dw - 1 ) * 0.5 + dx * w,
		y - h * ( dh - 1 ) * 0.5 + dy * h,
		w * dw,
		hs * dh
	);
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
		case ' ':
			vidGrabber.videoSettings();
		break;
	}
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
void testApp::mouseReleased()
{
	//
}
