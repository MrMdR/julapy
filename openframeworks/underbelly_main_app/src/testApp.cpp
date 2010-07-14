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
	bFullScreen			= false;
	bRightMonitor		= false;
	bUseRockBlobs		= true;
	bUseTrackerBlobs	= false;
	bAddFoodForBoids	= false;
	
	mode				= MODE_BOIDS;
	
	initRenderArea();
	
	initTracker();
	initBoids();
	initRocks();
	initOsc();
	initBlendModes();
	initGui();
	
	updateRocks();
	updateRenderArea();
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

void testApp :: initTracker ()
{
	tracker.setup();
}

void testApp :: initBoids ()
{
	int t = 160;
	int w = 512;
	int h = 512;

	for( int i=0; i<t; i++ )
	{
		string imgPath = "";
		char imgPathChar[ 255 ];
		sprintf( imgPathChar, "boid_anim/boid%04d.png", i );
		imgPath.insert( imgPath.size(), imgPathChar );

		boidFrames.push_back( ofImage() );
		ofImage &img = boidFrames.back();
		img.loadImage( imgPath );
	}
	
	flock.addBlobs( &blobs );
	flock.setBoidFrames( &boidFrames );
	flock.init();
}

void testApp :: initRocks ()
{
	rocks.setup();
}

void testApp :: initOsc ()
{
	oscSenderHosts.push_back( "169.254.209.151" );
	oscSenderPorts.push_back( 12345 );
	
//	oscSenderHosts.push_back( "localhost" );
//	oscSenderPorts.push_back( 12345 );
	
	for( int i=0; i<oscSenderHosts.size(); i++ )
	{
		oscSenders.push_back( ofxOscSender() );
		
		string oscSenderHost	= oscSenderHosts[ i ];
		int oscSenderPort		= oscSenderPorts[ i ];
		
		ofxOscSender &oscSender = oscSenders.back();
		oscSender.setup( oscSenderHost, oscSenderPort );
	}
	
	oscTreeVal = 0;
}

void testApp :: initBlendModes()
{
	addBlendMode( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	// alpha blending.
	addBlendMode( GL_SRC_ALPHA, GL_ONE );					// additive blending.
	addBlendMode( GL_ONE,		GL_ONE_MINUS_DST_ALPHA );	// ?
	addBlendMode( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	// ?
	
	blendModeIndex = 0;
}

void testApp :: addBlendMode( GLuint srcBlend, GLuint dstBlend )
{
	blendModes.push_back( srcBlend );
	blendModes.push_back( dstBlend );
}

void testApp :: initGui ()
{
	gui.addTitle( "general" );
	gui.addToggle( "bUseRockBlobs",		bUseRockBlobs );
	gui.addToggle( "bUseTrackerBlobs",	bUseTrackerBlobs );
	gui.addToggle( "bAddFoodForBoids",	bAddFoodForBoids );
	
	gui.addPage();
	gui.addTitle( "boids_1 :: movement" );
	gui.addSlider( "boidsNum ",			flock.boidsNumRevised,		0, 500   );
	gui.addSlider( "seperation ",		flock.boidSeperationWeight,	0, 10.0  );
	gui.addSlider( "alignment ",		flock.boidAlignmentWeight,	0, 10.0  );
	gui.addSlider( "cohesion ",			flock.boidCohesionWeight,	0, 10.0  );
	gui.addSlider( "randomWeight ",		flock.boidRandomWeight,		0, 10.0  );
	gui.addSlider( "separationDist ",	flock.boidSeparationDist,	0, 500.0 );
	gui.addSlider( "perception ",		flock.boidPerception,		0, 500.0 );
	gui.addSlider( "maxSpeed ",			flock.boidMaxSpeed,			0, 100.0 );
	gui.addSlider( "maxForce ",			flock.boidMaxForce,			0, 10.0  );
	gui.addSlider( "mouseReach ",		flock.mouseReach,			0, 200.0 );
	gui.addSlider( "mouseForce ",		flock.mouseForce,			-20.0, 0 );

	gui.addPage();
	gui.addTitle( "boids_1 :: presentation" );
	gui.addSlider( "blendMode ",		blendModeIndex,				0, (int)( ( blendModes.size() - 1 ) * 0.5 ) );

	gui.addPage();
	gui.addTitle( "boids_2" );
	gui.addSlider( "homeX ",			flock.home.x,				-0.5, 1.5 );
	gui.addSlider( "homeY ",			flock.home.y,				-0.5, 1.5 );
	
	gui.addPage();
	gui.addTitle( "tracking" );
	gui.addSlider( "threshold_1 ",		tracker.threshold_1,	0, 255 );
	gui.addSlider( "blur ",				tracker.blur,			0, 40  );
	gui.addSlider( "threshold_2 ",		tracker.threshold_2,	0, 255 );
	gui.addToggle( "bUpdateCameraPos",	tracker.bUpdateCameraPos );
	gui.addSlider( "cameraRadius",		tracker.cameraRadius,	0, 360 );
	
//	gui.show();
	
	if( mode == MODE_BOIDS )
		gui.setPage( 2 );
	
	if( mode == MODE_TRACK )
		gui.setPage( 3 );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp :: update()
{
	tracker.update();
	
	if( mode == MODE_BOIDS )
	{
		updateBlobs();
		updateBoids();
	}
	
	updateOsc();
}

void testApp :: updateBlobs ()
{
	blobs.clear();
	
	if( bUseRockBlobs )
	{
		for( int i=0; i<rocks.blobs.size(); i++ )
		{
			blobs.push_back( rocks.blobs[ i ] );
		}
	}

	if( bUseTrackerBlobs )
	{
		for( int i=0; i<tracker.blobs.size(); i++ )
		{
			blobs.push_back( tracker.blobs[ i ] );
		}
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

void testApp :: updateRocks ()
{
	rocks.update();
}

void testApp :: updateOsc ()
{
	float treeVal;
	treeVal = flock.boidsInTree * 2;
	treeVal = MIN( treeVal, 1.0 );
	
	oscTreeVal += ( treeVal - oscTreeVal ) * 0.2;
	
	ofxOscMessage m;
	m.setAddress( "/boids/tree" );
	m.addFloatArg( oscTreeVal );
	
	for( int i=0; i<oscSenders.size(); i++ )
	{
		ofxOscSender &oscSender = oscSenders.back();
		oscSender.sendMessage( m );
	}
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
	
	tracker.updateRenderArea( renderArea );
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

		glEnable(GL_BLEND);
		glBlendFunc( blendModes[ blendModeIndex * 2 + 0 ], blendModes[ blendModeIndex * 2 + 1 ] );
		
		drawBoids();
		
		glDisable(GL_BLEND);
		
		drawRocks();
		drawPeeps();
//		drawBoidTexture();
		
		glPopMatrix();
	}
	
	if( mode == MODE_ROCKS )
	{
		glPushMatrix();
		glTranslatef( renderArea.x, renderArea.y, 0 );
		
		drawRocksDebug();
		
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
	
	tracker.draw();
}

void testApp :: drawBoids ()
{
	flock.draw();
}

void testApp :: drawRocks ()
{
	if( !bUseRockBlobs )
		return;
	
	rocks.drawDownsampledBlobs();
}

void testApp :: drawRocksDebug ()
{
	rocks.drawImage();
	rocks.drawDownsampledBlobs();
}

void testApp :: drawPeeps ()
{
	if( !bUseTrackerBlobs )
		return;
	
	for( int i=0; i<tracker.blobs.size(); i++ )
	{
		ofxCvBlob &blob = tracker.blobs[ i ];
		
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

void testApp :: drawBoidTexture ()
{
	ofEnableAlphaBlending();
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	
	int frameIndex = ofGetFrameNum() % boidFrames.size();
	cout << frameIndex << endl;
	ofImage &img = boidFrames[ frameIndex ];
	img.draw( 0, 0 );
	
	ofDisableAlphaBlending();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == '[' ) gui.prevPage();
	if( key == ']' ) gui.nextPage();
	
	if( key == '1' )
	{
		mode = MODE_BOIDS;
		
		gui.setPage( 2 );
	}

	if( key == '2' )
	{
		mode = MODE_TRACK;
		
		gui.setPage( 3 );
	}

	if( key == '3' )
	{
		mode = MODE_ROCKS;
		
//		gui.setPage( 2 );
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
	
	if( mode == MODE_TRACK )
	{
		if( key == 'b' )
		{
			tracker.updateBackground();
		}
		
		if( key == 'c' )
		{
			tracker.bUpdateCameraPos = !tracker.bUpdateCameraPos;
		}
		
		if( key == 'w' )
		{
			tracker.cvWarper.reset();
		}
		
		if( key == 's' )
		{
			tracker.cvWarper.save( "video_warper_data.txt" );
		}
		
		if( key == 'l' )
		{
			tracker.cvWarper.load( "video_warper_data.txt" );
		}
	}
	
	if( mode == MODE_BOIDS )
	{
		if( key == 'o' )
		{
			flock.addObstacle( ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ) );
		}
	}
}

void testApp::keyReleased(int key)
{
	//
}

void testApp::mouseMoved(int x, int y )
{
	if( mode == MODE_BOIDS )
	{
		flock.addMouse( x, y );
	}
}

void testApp::mouseDragged(int x, int y, int button)
{
	if( mode == MODE_TRACK && tracker.bUpdateCameraPos )
	{
		tracker.updateCameraPos( x, y );
	}
}

void testApp::mousePressed(int x, int y, int button)
{
	if( mode == MODE_BOIDS && bAddFoodForBoids )
	{
		flock.addFood( x, y );
	}
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

