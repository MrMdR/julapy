#include "testApp.h"

///////////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////////

void testApp :: setup ()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	bDebug			= true;
	
	initRenderArea();
	initBoids();
	initGui();
	
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
	gui.addTitle( "contour analysis" );
	
//	gui.loadFromXML();
	
	gui.show();
	gui.setPage( 1 );
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp :: update()
{
	updateBoids();
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
		renderArea.x		= renderAreaWindow.x;
		renderArea.y		= renderAreaWindow.y;
		renderArea.width	= renderAreaWindow.width;
		renderArea.height	= renderAreaWindow.height;
	}
	
	if( ofGetWindowMode() == OF_FULLSCREEN )
	{
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

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp :: draw()
{
	ofBackground( 0, 0, 0 );
	
	glPushMatrix();
	glTranslatef( renderArea.x, renderArea.y, 0 );
	
	drawBoids();
	drawBlobs();
	
	glPopMatrix();
	
	if( bDebug )
		return;
	
	gui.draw();
}

void testApp :: drawBoids ()
{
	flock.draw();
}

void testApp :: drawBlobs ()
{
	for( int i=0; i<blobs.size(); i++ )
	{
		ofxCvBlob &blob = blobs[ i ];
		
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

///////////////////////////////////////////////////
//	DATA.
///////////////////////////////////////////////////

void testApp :: loadRockData ( string fileName )
{
	blobs.clear();
	
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
				
				blobs.push_back( ofxCvBlob() );
			}
			
			if( line == "" )
			{
				bNewBlob = true;
				
				continue;
			}
			
			vector<string> values = ofSplitString( line, " " );
			
			ofxCvBlob &blob = blobs.back();
			
			blob.pts.push_back( ofPoint() );
			ofPoint &p = blob.pts.back();
			p.x = atof( values[ 0 ].c_str() );
			p.y = atof( values[ 1 ].c_str() );
		}
	}
	myFile.close();
}

///////////////////////////////////////////////////
//	EVENTS.
///////////////////////////////////////////////////

void testApp :: keyPressed(int key)
{
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
	
	if( key == 'o' )
	{
		flock.addObstacle( ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ) );
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
	
	if( key == 'l' )
	{
		loadRockData();
	}
}

void testApp::keyReleased(int key)
{
	
}

void testApp::mouseMoved(int x, int y )
{
	flock.addMouse( x, y );
}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{
	flock.addFood( x, y );
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

