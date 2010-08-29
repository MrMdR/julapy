#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	
	ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
	
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	
	initSounds();	// has to be before ColorCycle setup is called.
	
	//--
	
	lastTouchId = -1;
	
	//--
	
	cc.setScreenSize( ofGetWidth(), ofGetHeight() );
	cc.setup();
	
	//--
	
	footer.setup();
	
	//-- splash screen fade out.
	
	splashScreen = new ofImage();
	splashScreen->loadImage( "Default.png" );
	drawSplashScreen();
	
	//--
	
	infoScreen.setup();
}

void testApp :: initSounds ()
{
	vector<string> soundNames;
	soundNames.push_back( "sounds/background/background_1.caf" );
	soundNames.push_back( "sounds/background/background_2.caf" );
	soundNames.push_back( "sounds/background/background_3.caf" );
	createSounds( soundNames, spBackground, true );
	
	soundNames.clear();
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_1.caf" );
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_2.caf" );
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_3.caf" );
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_4.caf" );
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_5.caf" );
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_6.caf" );
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_7.caf" );
	soundNames.push_back( "sounds/mesh_drag/mesh_drag_8.caf" );
	createSounds( soundNames, spMeshDrag );
	
	soundNames.clear();
	soundNames.push_back( "sounds/point_add/point_add_1.caf" );
	createSounds( soundNames, spPointAdd );
	
	soundNames.clear();
	soundNames.push_back( "sounds/point_collide/point_collide_1.caf" );
	soundNames.push_back( "sounds/point_collide/point_collide_2.caf" );
	soundNames.push_back( "sounds/point_collide/point_collide_3.caf" );
	soundNames.push_back( "sounds/point_collide/point_collide_4.caf" );
	soundNames.push_back( "sounds/point_collide/point_collide_5.caf" );
	soundNames.push_back( "sounds/point_collide/point_collide_6.caf" );
	soundNames.push_back( "sounds/point_collide/point_collide_7.caf" );
	soundNames.push_back( "sounds/point_collide/point_collide_8.caf" );
	createNumOfSounds( soundNames, spPointCollide );
	
	soundNames.clear();
	soundNames.push_back( "sounds/point_remove/point_remove_1.caf" );
	createSounds( soundNames, spPointRemove );
	
	soundNames.clear();
	soundNames.push_back( "sounds/point_shuffle/point_shuffle.caf" );
	createSounds( soundNames, spPointShuffle );
	
	cc.spBackground		= spBackground;
	cc.spMeshDrag		= spMeshDrag;
	cc.spPointAdd		= spPointAdd;
	cc.spPointCollide	= spPointCollide;
	cc.spPointRemove	= spPointRemove;
	cc.spPointShuffle	= spPointShuffle;
}

void testApp :: createSounds ( const vector<string>& fileNames, vector<ofxALSoundPlayer*>& sounds, bool loop )
{
	for( int i=0; i<fileNames.size(); i++ )
	{
		ofxALSoundPlayer* sp	= new ofxALSoundPlayer();
		string fileName			= fileNames[ i ];
		
		if( loop )
			sp->loadLoopingSound( fileName );
		else 
			sp->loadSound( fileName );
		
		sounds.push_back( sp );
	}
}

void testApp :: createNumOfSounds ( const vector<string>& fileNames, vector<ofxALSoundPlayer*>& sounds, bool loop, int numOfSounds )
{
	for( int i=0; i<numOfSounds; i++ )
	{
		int j = ofRandom( 0, fileNames.size() - 1 );
		
		ofxALSoundPlayer* sp	= new ofxALSoundPlayer();
		string fileName			= fileNames[ j ];
		
		if( loop )
			sp->loadLoopingSound( fileName );
		else 
			sp->loadSound( fileName );
		
		sounds.push_back( sp );
	}
}

//--------------------------------------------------------------
void testApp::update()
{
	checkLastTouch();
	
	if( footer.isShuffleSelected() )
		cc.shuffle();
	
	if( footer.isColorSelected() )
		cc.colorSelectMode();
	
	if( footer.isAddSelected() )
		cc.addCircle();
	
	if( footer.isRemoveSelected() )
		cc.removeCircle();
	
	if( footer.isInfoSelected() )
		infoScreen.show();
	
	if( footer.isHideSelected() )				// doesn't work, touch needs to pass through EAGLView to work.
	{
		ofPoint p = footer.getHidePoint();
		cc.down( p.x, p.y, 0 );
	}
	
	float gx;
	gx = ofxAccelerometer.getForce().y;
	gx *= 2;									// increase the reaction to tilt.
	gx = MIN( 1.0, MAX( -1.0, gx ) );			// between -1 and 1.
	
	float gy;
	gy = ofxAccelerometer.getForce().x;
	gy *= 2;									// increase the reaction to tilt.
	gy = MIN( 1.0, MAX( -1.0, gy ) );			// between -1 and 1.
	
	cc.setGravity( gx, gy );
	
	cc.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	cc.draw();
	
	drawSplashScreen();
	
	if( upsideDown )
	{
		int cx = (int)( ofGetScreenWidth()  * 0.5 );
		int cy = (int)( ofGetScreenHeight() * 0.5 );
		
		glPushMatrix();
		glTranslatef( cx, cy, 0 );
		glRotatef( 180, 0, 0, 1 );
		glTranslatef( -cx, -cy, 0 );
	}
	
	infoScreen.draw();
	
	ofSetColor( 0, 0, 0 );
	ofDrawBitmapString( ofToString( ofGetFrameRate(),  0 ), ofGetScreenWidth() - 30, 20 );
	
	if( upsideDown )
	{
		glPopMatrix();
	}
}

void testApp :: drawSplashScreen ()
{
	if( splashScreen == NULL )
		return;
	
	int i = ofGetFrameNum();
	int t = 30;
	
	if( i > t )
	{
		splashScreen->clear();
		delete splashScreen;
		splashScreen = NULL;
		return;
	}
	
	float a = 1 - Quad :: easeOut( i, 0, 1.0, t );
	
	int cx = (int)( ofGetScreenWidth()  * 0.5 );
	int cy = (int)( ofGetScreenHeight() * 0.5 );

	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, a * 255 );
	
	glPushMatrix();
	glTranslatef( cx, cy, 0 );
	glRotatef( 90, 0, 0, 1 );
	glTranslatef( -cy, -cx, 0 );
	splashScreen->draw( 0, 0 );
	glPopMatrix();
	
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch)
{
	infoScreen.hide();
	
	if( lastTouchId != touch.id )
	{
		lastTouchId			= touch.id;
		lastTouch.x			= touch.x;
		lastTouch.y			= touch.y;
		lastTouchMoved.x	= touch.x;
		lastTouchMoved.y	= touch.y;
		lastTouchCount		= 0;
	}
	
	cc.down( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch)
{
	if( lastTouchId == touch.id )
	{
		lastTouchMoved.x	= touch.x;
		lastTouchMoved.y	= touch.y;
	}
	
	cc.drag( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch)
{
	if( lastTouchId == touch.id )
	{
		lastTouchId = -1;
	}
	
	cc.up( touch.x, touch.y, touch.id );
}

void testApp :: checkLastTouch ()
{
	if( lastTouchId == -1 )
		return;
	
	if( ++lastTouchCount > 30 )
	{
		lastTouchId = -1;
		
		float d = ofDist( lastTouch.x, lastTouch.y, lastTouchMoved.x, lastTouchMoved.y );
		
		if( d < 20 )
		{
			if( !footer.isShowing() )
			{
				footer.show();
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch)
{
	footer.toggleShow();
}

//--------------------------------------------------------------
void testApp::lostFocus()
{
	//
}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation)
{
	if( newOrientation == 3 )
		upsideDown = true;
	
	if( newOrientation == 4 )
		upsideDown = false;
}

