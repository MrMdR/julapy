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
	
	mode				= MODE_TRACK;
	
	initRenderArea();
	
	initTracker();
	initBoids();
	initRocks();
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
	flock.init();
	flock.addBlobs( &blobs );
	
//	for( int i=0; i<10; i++ )
//	{
//		flock.addObstacle( ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ) );
//	}
}

void testApp :: initRocks ()
{
	rocks.setup();
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
	gui.addSlider( "threshold_1 ",		tracker.threshold_1,	0, 255 );
	gui.addSlider( "blur ",				tracker.blur,			0, 40  );
	gui.addSlider( "threshold_2 ",		tracker.threshold_2,	0, 255 );
	gui.addToggle( "bUpdateCameraPos",	tracker.bUpdateCameraPos );
	gui.addSlider( "cameraRadius",		tracker.cameraRadius,	0, 360 );
	
//	gui.loadFromXML();
	
	gui.show();
	gui.setPage( 1 );
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
}

void testApp :: updateBlobs ()
{
	blobs.clear();
	
	for( int i=0; i<rocks.blobs.size(); i++ )
	{
		blobs.push_back( rocks.blobs[ i ] );
	}
	
	for( int i=0; i<tracker.blobs.size(); i++ )
	{
		blobs.push_back( tracker.blobs[ i ] );
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
	
	tracker.draw();
}

void testApp :: drawBoids ()
{
	flock.draw();
}

void testApp :: drawRocks ()
{
	if( mode == MODE_ROCKS )
	{
		rocks.drawImage();
		rocks.drawDownsampledBlobs();
	}
	{
		rocks.drawDownsampledBlobs();
	}
}

void testApp :: drawPeeps ()
{
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
	if( mode == MODE_BOIDS )
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

