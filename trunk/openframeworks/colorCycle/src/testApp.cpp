#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );

	//-- sounds.
	
	loadSounds( "sounds/background",	spBackground, true );
	loadSounds( "sounds/mesh_drag",		spMeshDrag );
	loadSounds( "sounds/point_add",		spPointAdd );
	loadNumOfSounds( "sounds/point_collide", spPointCollide );
	loadSounds( "sounds/point_remove",	spPointRemove );
	loadSounds( "sounds/point_shuffle",	spPointShuffle );

	cc.spBackground		= spBackground;
	cc.spMeshDrag		= spMeshDrag;
	cc.spPointAdd		= spPointAdd;
	cc.spPointRemove	= spPointRemove;
	cc.spPointCollide	= spPointCollide;
	cc.spPointShuffle	= spPointShuffle;
	
	cc.setScreenSize( ofGetWidth(), ofGetHeight() );
	cc.setup();
	cc.bColorSelectModeOnBgClick = true;
}

void testApp :: loadSounds( string dirPath, vector<ofSoundPlayer*>& soundVector, bool loop )
{
	int t = 0;
    t = DIR.listDir( dirPath );
    for( int i=0; i<t; i++ )
	{
		ofSoundPlayer* sp;
		sp = new ofSoundPlayer();
		sp->loadSound( DIR.getPath(i) );
		sp->setLoop( loop );
		sp->setMultiPlay( true );
		
		soundVector.push_back( sp );
    }
}

void testApp :: loadNumOfSounds ( string dirPath, vector<ofSoundPlayer*>& soundVector, bool loop, int numOfSounds )
{
	int t = 0;
    t = DIR.listDir( dirPath );
    for( int i=0; i<numOfSounds; i++ )
	{
		int j = ofRandom( 0, t - 1 );
		
		ofSoundPlayer* sp;
		sp = new ofSoundPlayer();
		sp->loadSound( DIR.getPath( j ) );
		sp->setLoop( loop );
		
		soundVector.push_back( sp );
    }
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	cc.update();
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	cc.draw();
	
	ofSetColor( 0, 0, 0 );
	ofDrawBitmapString( ofToString( ofGetFrameRate(),  0 ), ofGetWidth() - 30, 20 );
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{
	if( key == 's' )
	{
		cc.shuffle();
	}
	
	if( key == '=' )
	{
		cc.addCircle();
	}
	
	if( key == '-' )
	{
		cc.removeCircle();
	}
}

void testApp :: keyReleased( int key )
{

}

void testApp :: mouseMoved( int x, int y )
{
	float gx = x / (float)ofGetWidth()  * 2 - 1;
	float gy = y / (float)ofGetHeight() * 2 - 1;
	
//	cc.setGravity( gx, gy );
}

void testApp :: mouseDragged( int x, int y, int button )
{
	cc.drag( x, y );
}

void testApp :: mousePressed( int x, int y, int button )
{
	cc.down( x, y );
}

void testApp :: mouseReleased( int x, int y, int button )
{
	cc.up( x, y );
}

void testApp :: windowResized( int w, int h )
{

}

