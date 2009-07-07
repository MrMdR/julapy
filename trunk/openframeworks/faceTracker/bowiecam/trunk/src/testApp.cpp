#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{	 
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
	
	bowiesTotal = 4;
	bowies	= new ofImage[ bowiesTotal ];
	bowies[ 0 ].loadImage( "bowie_01.png" );
	bowies[ 1 ].loadImage( "bowie_02.png" );
	bowies[ 2 ].loadImage( "bowie_03.png" );
	bowies[ 3 ].loadImage( "bowie_04.png" );
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
		sampleImage.draw( WIDTH, 0 );
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
		
//		drawSquare( x, y, w, h );
		
		if( faceMode > 0.75 )
		{
			drawZiggyEyesClosed( x, y, w, h );
		}
		else if( faceMode > 0.50 )
		{
			drawStencilBowie( x, y, w, h );
		}
		else if( faceMode > 0.25 )
		{
			drawDogBowie( x, y, w, h );
		}
		else
		{
			drawFlashBolt( x, y, w, h );
		}
	}
	
	#ifdef DEBUG_MODE
	#else
		glPopMatrix();
	#endif
}

void testApp :: drawSquare ( float x, float y, float w, float h )
{
	ofNoFill();
	ofSetLineWidth( 10 );
	ofRect( x, y, w, h );
}

void testApp :: drawZiggyEyesClosed ( float x, float y, float w, float h )
{
	float dx = 0;
	float dy = -0.32;
	float dw = 1.15;
	float dh = 1.1;
	float ws = h / bowies[ 0 ].height * bowies[ 0 ].width;
	float hs = w / bowies[ 0 ].width * bowies[ 0 ].height;
	
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	bowies[ 0 ].draw
	(
		x - w * ( dw - 1 ) * 0.5 + dx * w,
		y - h * ( dh - 1 ) * 0.5 + dy * h,
		w * dw,
		hs * dh
	);
	ofDisableAlphaBlending();
}

void testApp :: drawStencilBowie ( float x, float y, float w, float h )
{
	float dx = 0;
	float dy = -0.35;
	float dw = 1.15;
	float dh = 1.1;
	float ws = h / bowies[ 1 ].height * bowies[ 1 ].width;
	float hs = w / bowies[ 1 ].width * bowies[ 1 ].height;
	
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	bowies[ 1 ].draw
	(
		x - w * ( dw - 1 ) * 0.5 + dx * w,
		y - h * ( dh - 1 ) * 0.5 + dy * h,
		w * dw,
		hs * dh
	);
	ofDisableAlphaBlending();
}

void testApp :: drawDogBowie ( float x, float y, float w, float h )
{
	float dx = 0;
	float dy = 0;
	float dw = 1.25;
	float dh = 1.25;
	float ws = h / bowies[ 2 ].height * bowies[ 2 ].width;
	float hs = w / bowies[ 2 ].width * bowies[ 2 ].height;
	
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	bowies[ 2 ].draw
	(
		x - w * ( dw - 1 ) * 0.5 + dx * w,
		y - h * ( dh - 1 ) * 0.5 + dy * h,
		w * dw,
		hs * dh
	);
	ofDisableAlphaBlending();
}

void testApp :: drawFlashBolt ( float x, float y, float w, float h )
{
	float dx = 0;
	float dy = -0.5;
	float dw = 1.15;
	float dh = 1.1;
	float ws = h / bowies[ 3 ].height * bowies[ 3 ].width;
	float hs = w / bowies[ 3 ].width * bowies[ 3 ].height;
	
	ofNoFill();
	ofSetColor( 0xFFFFFF );
	ofEnableAlphaBlending();
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	bowies[ 3 ].draw
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
