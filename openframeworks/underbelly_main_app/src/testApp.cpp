#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );

	bDebug				= true;
	bBoids				= true;
	bFullScreen			= false;
	bRightMonitor		= false;
	bUpdateCameraPos	= false;
	
	mode				= MODE_BOIDS;
	
	modeTrackOffsetX	= 260;
	modeTrackOffsetY	= 115;
	
	initRenderArea();
	initBoids();
	initVideo();
	initVideoGrabber();
	initVideoPlayer();
	initGui();
	
	cameraRadius = 200;
	updateCameraPos( videoSmlRect.width * 0.5, videoSmlRect.height * 0.5 );
	
	loadRockData();
}

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
	renderAreaRightMonitor.width	= 1024;
	renderAreaRightMonitor.height	= 768;
	
	renderArea.x		= renderAreaWindow.x;
	renderArea.y		= renderAreaWindow.y;
	renderArea.width	= renderAreaWindow.width;
	renderArea.height	= renderAreaWindow.height;
}

void testApp :: initBoids ()
{
	flock.init();
	flock.addBlobs( &blobs );
	
//	for( int i=0; i<10; i++ )
//	{
//		flock.addObstacle( ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ) );
//	}
}

void testApp :: initVideo()
{
	videoLrgRect.width  = 640;
	videoLrgRect.height = 480;
	
	videoSmlRect.width  = 320;
	videoSmlRect.height = 240;

	videoLrgImage.allocate( videoLrgRect.width, videoLrgRect.height );
	videoSmlImage.allocate( videoSmlRect.width, videoSmlRect.height );
	
	cvImageWarp.allocate( videoSmlRect.width, videoSmlRect.height );
	cvImageBg.allocate( videoSmlRect.width, videoSmlRect.height );
	cvImageDiff.allocate( videoSmlRect.width, videoSmlRect.height );
	cvImageThsh.allocate( videoSmlRect.width, videoSmlRect.height );
	
	cvWarper.setup( &videoSmlImage, &cvImageWarp );
	cvWarper.setPosition( modeTrackOffsetX, modeTrackOffsetY );
	cvWarper.load( "video_warper_data.txt" );
	
	threshold_1	= 30;
	threshold_2	= 30;
	blur		= 4;
	
	isVideoNewFrame = false;
}

void testApp :: initVideoGrabber ()
{
#ifndef USE_VIDEO_INPUT
#ifdef	USE_POINT_GREY_CAMERA	
	bool result = videoGrabber.initGrabber( videoLrgRect.width, videoLrgRect.height, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 25, true, new Libdc1394Grabber() );
	
	if( result )
	{
		ofLog(OF_LOG_NOTICE,"Camera succesfully initialized.");
	} 
	else
	{
		ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}
#else
	videoGrabber.initGrabber( videoLrgRect.width, videoLrgRect.height );
#endif
#endif
}

void testApp :: initVideoPlayer ()
{
#ifdef USE_VIDEO_INPUT
	videoPlayer.loadMovie( "underbelly_test_03.mov" );
	videoPlayer.setPaused( false );
	videoPlayer.setPosition( 0.0 );
	videoPlayer.setLoopState( true );
	videoPlayer.play();
#endif
}

void testApp :: initGui ()
{
	gui.addTitle( "boids" );
	gui.addSlider( "boidsNum ",				flock.boidsNumRevised,		0, 500   );
	gui.addSlider( "boidSeperationWeight ", flock.boidSeperationWeight,	0, 10.0  );
	gui.addSlider( "boidAlignmentWeight ",	flock.boidAlignmentWeight,	0, 10.0  );
	gui.addSlider( "boidCohesionWeight ",	flock.boidCohesionWeight,	0, 10.0  );
	gui.addSlider( "boidSeparationDist ",	flock.boidSeparationDist,	0, 500.0 );
	gui.addSlider( "boidPerception ",		flock.boidPerception,		0, 500.0 );
	gui.addSlider( "boidMaxSpeed ",			flock.boidMaxSpeed,			0, 100.0 );
	gui.addSlider( "boidMaxForce ",			flock.boidMaxForce,			0, 10.0  );
	gui.addSlider( "mouseReach ",			flock.mouseReach,			0, 200.0 );
	gui.addSlider( "mouseForce ",			flock.mouseForce,			-20.0, 0 );
	
	gui.addPage();
	gui.addTitle( "openCV" );
	gui.addSlider( "threshold_1 ",		threshold_1,	0, 255 );
	gui.addSlider( "blur ",				blur,			0, 40  );
	gui.addSlider( "threshold_2 ",		threshold_2,	0, 255 );
	gui.addToggle( "bUpdateCameraPos",	bUpdateCameraPos );
	gui.addSlider( "cameraRadius",		cameraRadius,	0, 360 );
	
//	gui.loadFromXML();
	
	gui.show();
	gui.setPage( 1 );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp :: update()
{
	updateVideoGrabber();
	updateVideoPlayer();
	updateTracking();
	
	int blobsFound;
	blobsFound = updateContours();
	
	if( blobsFound > 0 )
	{
		contourUtil.downSampleBlobs( contourFinder.blobs, peeps, 40 );
		contourUtil.scaleBlobs( peeps, renderArea.width / (float)videoSmlRect.width, renderArea.height / (float)videoSmlRect.height );
//		contourUtil.translateBlobs( peeps, renderArea.x, renderArea.y );
	}
	else
	{
		peeps.clear();
	}
	
	if( mode == MODE_BOIDS )
	{
		updateBlobs();
		updateBoids();
	}
}

void testApp :: updateVideoGrabber ()
{
#ifndef USE_VIDEO_INPUT
#ifdef	USE_POINT_GREY_CAMERA
	videoGrabber.update();
#else
	videoGrabber.grabFrame();
#endif
	
	isVideoNewFrame = videoGrabber.isFrameNew();
	
	if( isVideoNewFrame )
	{
		
#ifdef USE_POINT_GREY_CAMERA
		
		videoLrgImage.setFromPixels( videoGrabber.getPixels(), videoLrgRect.width, videoLrgRect.height );
		
#else
		ofxCvColorImage videoClrImage;
		videoClrImage.allocate( videoLrgRect.width, videoLrgRect.height );
		videoClrImage.setFromPixels( videoGrabber.getPixels(), videoLrgRect.width, videoLrgRect.height );
		
		videoLrgImage.setFromColorImage( videoClrImage );
		
		videoClrImage.clear();
#endif
		
		videoSmlImage.scaleIntoMe( videoLrgImage );
	}
#endif
}

void testApp :: updateVideoPlayer ()
{
#ifdef USE_VIDEO_INPUT
	
	videoPlayer.update();
	
	isVideoNewFrame = videoPlayer.isFrameNew();
	
	if( isVideoNewFrame )
	{
		ofxCvColorImage videoClrImage;
		videoClrImage.allocate( videoLrgRect.width, videoLrgRect.height );
		videoClrImage.setFromPixels( videoPlayer.getPixels(), videoLrgRect.width, videoLrgRect.height );
		
		videoLrgImage.setFromColorImage( videoClrImage );
		
		videoClrImage.clear();
		
		videoSmlImage.scaleIntoMe( videoLrgImage );
	}
	
#endif
}

void testApp :: updateTracking ()
{
	cvWarper.warp();
	
	cvImageDiff.absDiff( cvImageBg, cvImageWarp );
	cvImageDiff.threshold( threshold_1, false );
	cvImageDiff = cvImageDiff;
	cvImageDiff.blur( blur );
	
	cvImageThsh = cvImageDiff;
	cvImageThsh.threshold( threshold_2, false );
}

void testApp :: updateBackground ()
{
	cvImageBg = cvImageWarp;
}

int testApp :: updateContours ()
{
	int maxArea;
	maxArea = videoSmlRect.width * videoSmlRect.height;
	
	float cfMinArea;
	cfMinArea = 0.001;
	
	int maxContoursToFind;
	maxContoursToFind = 20;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		cvImageThsh,					// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		false,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void testApp :: updateCameraPos	( int x, int y )
{
	cameraPosition.x = x;
	cameraPosition.y = y;
	
	float dist = 0;
	int w = videoSmlRect.width;
	int h = videoSmlRect.height;
	
//	cameraRadius = 0;
//	
//	dist = ofDist( x, y, 0, 0 );
//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
//	
//	dist = ofDist( x, y, w, 0 );
//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
//	
//	dist = ofDist( x, y, w, h );
//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
//	
//	dist = ofDist( x, y, 0, h );
//	cameraRadius = ( dist > cameraRadius ) ? dist : cameraRadius;
}

void testApp :: updateBlobs ()
{
	blobs.clear();
	
	for( int i=0; i<rocks.size(); i++ )
	{
		blobs.push_back( rocks[ i ] );
	}
	
	for( int i=0; i<peeps.size(); i++ )
	{
		blobs.push_back( peeps[ i ] );
	}
}	

void testApp :: updateBoids ()
{
	ofRectangle boidRect;
	boidRect.width	= renderArea.width;
	boidRect.height	= renderArea.height;
	
	flock.setContainer( boidRect );
	flock.update();
}

void testApp :: updateRenderArea()
{
	if( ofGetWindowMode() == OF_WINDOW )
	{
		bFullScreen = false;
		
		renderArea.x		= renderAreaWindow.x;
		renderArea.y		= renderAreaWindow.y;
		renderArea.width	= renderAreaWindow.width;
		renderArea.height	= renderAreaWindow.height;
	}
	
	if( ofGetWindowMode() == OF_FULLSCREEN )
	{
		bFullScreen = true;
		
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
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	ofBackground( 0, 0, 0 );

	if( mode == MODE_TRACK )
	{
		drawTracking();
	}

	if( mode == MODE_BOIDS )
	{
		glPushMatrix();
		glTranslatef( renderArea.x, renderArea.y, 0 );
		
		drawBoids();
		drawRocks();
		drawPeeps();
		
		glPopMatrix();
	}
	
	gui.draw();
}

void testApp :: drawTracking ()
{
	ofFill();
	ofSetColor( 0x222222 );
	if( bFullScreen )
	{
		ofRect( 0, 0, renderAreaFullScreen.width, renderAreaFullScreen.height );
	}
	else 
	{
		ofRect( 0, 0, renderAreaWindow.width, renderAreaWindow.height );
	}
	
	int pad = 10;
	
	glPushMatrix();
	glTranslatef( modeTrackOffsetX, modeTrackOffsetY, 0 );
	
	ofSetColor( 0xFFFFFF );
	videoSmlImage.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "1) original", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlImage.width + pad, 0, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageWarp.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "2) warped", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlRect.width + pad, 0, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageBg.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "3) background", 10, 20 );
	
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( 0, videoSmlRect.height + pad, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageDiff.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "4) diff + blur", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlRect.width + pad, 0, 0 );
	
	ofSetColor( 0xFFFFFF );
	cvImageThsh.draw( 0, 0 );
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "5) threshold", 10, 20 );
	
	glPushMatrix();
	glTranslatef( videoSmlRect.width + pad, 0, 0 );
	
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, videoSmlRect.width, videoSmlRect.height );
	contourFinder.draw();
	ofSetColor( 0x00FF00 );
	ofDrawBitmapString( "6) contours", 10, 20 );
	
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	ofNoFill();											//-- camera position.
	ofSetColor( 255, 0, 0, 127 );
	ofEnableAlphaBlending();
	ofCircle( cameraPosition.x, cameraPosition.y, cameraRadius );
	ofFill();
	ofCircle( cameraPosition.x, cameraPosition.y, 3 );
	ofDisableAlphaBlending();
	
	glPopMatrix();
	
	cvWarper.draw();
}

void testApp :: drawBoids ()
{
	flock.draw();
}

void testApp :: drawRocks ()
{
	for( int i=0; i<rocks.size(); i++ )
	{
		ofxCvBlob &blob = rocks[ i ];
		
		int t = blob.pts.size();
		
		for( int j=0; j<t; j++ )
		{
			ofPoint &p1 = blob.pts[ j ];
			ofPoint &p2 = blob.pts[ ( j + 1 ) % t ];
			
			ofNoFill();
			ofSetColor( 0xFF00FF );
			ofLine( p1.x, p1.y, p2.x, p2.y );
			ofCircle( p1.x, p1.y, 4 );
		}
	}
}

void testApp :: drawPeeps ()
{
	for( int i=0; i<peeps.size(); i++ )
	{
		ofxCvBlob &blob = peeps[ i ];
		
		int t = blob.pts.size();
		
		for( int j=0; j<t; j++ )
		{
			ofPoint &p1 = blob.pts[ j ];
			ofPoint &p2 = blob.pts[ ( j + 1 ) % t ];
			
			ofNoFill();
			ofSetColor( 0x00FFFF );
			ofLine( p1.x, p1.y, p2.x, p2.y );
			ofCircle( p1.x, p1.y, 4 );
		}
	}
}

///////////////////////////////////////////////////
//	DATA.
///////////////////////////////////////////////////

void testApp :: loadRockData ( string fileName )
{
	rocks.clear();
	
	ifstream myFile;
	string line;
	myFile.open( ofToDataPath( fileName ).c_str() );
	if( myFile.is_open() )
	{
		bool bNewBlob = true;
		
		while( !myFile.eof() )
		{
			getline( myFile, line );
			
			if( bNewBlob )
			{
				if( line == "" )		// if second blank line then finished.
				{
					break;
				}
				
				bNewBlob = false;
				
				rocks.push_back( ofxCvBlob() );
			}
			
			if( line == "" )
			{
				bNewBlob = true;
				
				continue;
			}
			
			vector<string> values = ofSplitString( line, " " );
			
			ofxCvBlob &blob = rocks.back();
			
			blob.pts.push_back( ofPoint() );
			ofPoint &p = blob.pts.back();
			p.x = atof( values[ 0 ].c_str() );
			p.y = atof( values[ 1 ].c_str() );
		}
	}
	myFile.close();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == '1' )
	{
		mode = MODE_BOIDS;
		
		gui.setPage( 1 );
	}

	if( key == '2' )
	{
		mode = MODE_TRACK;
		
		gui.setPage( 2 );
	}
	
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
	
	if( key == 'b' )
	{
		updateBackground();
	}
	
	if( key == 'c' )
	{
		bUpdateCameraPos = !bUpdateCameraPos;
	}
	
	if( key == 'w' )
	{
		cvWarper.reset();
	}
	
	if( key == 's' )
	{
		cvWarper.save( "video_warper_data.txt" );
	}
	
	if( key == 'l' )
	{
		cvWarper.load( "video_warper_data.txt" );
	}
	
	if( key == 'o' )
	{
		if( bBoids )
		{
			flock.addObstacle( ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ) );
		}
	}
	
	if( key == 'f' )
	{
		ofToggleFullscreen();
		updateRenderArea();
	}
	
	if( key == 'r' )
	{
		bRightMonitor = !bRightMonitor;
		
		updateRenderArea();
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{
	if( bBoids )
	{
		flock.addMouse( x, y );
	}
}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{
	if( mode == MODE_BOIDS && bBoids )
	{
		flock.addFood( x, y );
	}
	
	if( mode == MODE_TRACK && bUpdateCameraPos )
	{
		updateCameraPos( x - modeTrackOffsetX, y - modeTrackOffsetY );
	}
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

