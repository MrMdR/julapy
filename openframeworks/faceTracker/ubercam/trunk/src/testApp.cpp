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
	
	animationsTotal = 1;
	animations		= new ofxFrameAnimation *[ animationsTotal ];
	animations[ 0 ] = new HeadphoneAnimation();
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

void testApp :: addFace( int faceID, int x, int y, int w, int h )
{
	int animationIndex = (int)ofRandom( 0, animationsTotal );
	
	faces.push_back( Face() );
	faces.back().faceID			= faceID;
	faces.back().x				= x;
	faces.back().y				= y;
	faces.back().w				= w;
	faces.back().h				= h;
	faces.back().currentFrame	= 0;
	faces.back().idleCount		= 0;
	faces.back().idleCountLimit	= 100;
	faces.back().found			= true;
	faces.back().animation		= animations[ animationIndex ];
}

bool testApp :: hasFace( int faceID )
{
	bool found = false;
	
	for( int i=0; i<faces.size(); i++ )
	{
		if( faces.at( i ).faceID == faceID )
		{
			faces.at( i ).found = true;
			found = true;
			break;
		}
	}
	
	return found;
}

void testApp :: updateFace( int faceID, int x, int y, int w, int h )
{
	for( int i=0; i<faces.size(); i++ )
	{
		if( faces.at( i ).faceID == faceID )
		{
			faces.at( i ).x = x;
			faces.at( i ).y = y;
			faces.at( i ).w = w;
			faces.at( i ).h = h;
		}
	}
}

void testApp :: cullFaces()
{
	for( int i=0; i<faces.size(); i++ )
	{
		if( faces.at( i ).found )
		{
			faces.at( i ).found = false;
		}
		else
		{
			faces.erase( faces.begin() + i );
		}
	}
}

void testApp :: renderFaces ()
{
	for( int i=0; i<faces.size(); i++ )
	{
		int currentFrame;
		int totalFrames;
		int faceX, faceY, faceW, faceH;
		
		currentFrame	= faces.at( i ).currentFrame;
		totalFrames		= faces.at( i ).animation->totalFrames();
		faceX			= faces.at( i ).x;
		faceY			= faces.at( i ).y;
		faceW			= faces.at( i ).w;
		faceH			= faces.at( i ).h;
		
		currentFrame += 1;
		if( currentFrame > totalFrames - 1 )
		{
			currentFrame = 0;
		}
		
		faces.at( i ).currentFrame = currentFrame;
		faces.at( i ).animation->gotoFrame( currentFrame );
		faces.at( i ).animation->draw( faceX, faceY, faceW, faceH );
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
		
		if( hasFace( faceID ) )
		{
			updateFace( faceID, x, y, w, h );
		}
		else
		{
			addFace( faceID, x, y, w, h );
		}
	}
	
	cullFaces();
	renderFaces();
	
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
