#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync( true );
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber( WIDTH, HEIGHT );
        
	colorLargeImage.allocate( WIDTH, HEIGHT );
	colorSmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
    grayLargeImage.allocate( WIDTH, HEIGHT );
	graySmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
	
	sourceToSampleScale = WIDTH / SAMPLE_WIDTH;
	sampleToSourceScale	= SAMPLE_WIDTH / (float)WIDTH;

	haarFinder.setup( "haarXML/haarcascade_frontalface_default.xml" );
	haarTracker.setup( &haarFinder );
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
			ofSetColor( 0xFF0000 );
		}
		else if( faceMode > 0.33 )
		{
			ofSetColor( 0x00FF00 );
		}
		else
		{
			ofSetColor( 0x0000FF );
		}
		
		ofNoFill();
		ofSetLineWidth( 10 );
		ofRect( x, y, w, h );
	}
	
	#ifdef DEBUG_MODE
	#else
		glPopMatrix();
	#endif
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
