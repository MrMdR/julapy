#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	ofBackground( 127, 127, 127 );
	
	ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
	
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	
	lastTouchId = -1;
	
	splashScreen = NULL;
	splashScreen = new SplashScreen();		// comment out to remove.
	if( splashScreen != NULL )
	{
		splashScreen->setup();
		splashScreen->draw();
	}

	footer = NULL;
	footer = new FooterBar();				// comment out to remove.
	if( footer != NULL )
	{
		footer->setup();
	}
	
	infoScreen = NULL;
	infoScreen = new InfoScreen();			// comment out to remove.
	if( infoScreen != NULL )
	{
		infoScreen->setup();
	}
	
	sounds = NULL;
	sounds = new ColorSound();				// comment out to remove.
	if( sounds != NULL )
	{
		sounds->setup();
	}
	
	cc = NULL;
	cc = new ColorCycle();					// comment out to remove.
	if( cc != NULL )
	{
		cc->setSounds( sounds );
		cc->setScreenSize( ofGetWidth(), ofGetHeight() );
		cc->setup();
	}
}

//--------------------------------------------------------------
void testApp::update()
{
	checkLastTouch();
	
	if( footer != NULL )
	{
		if( footer->isShuffleSelected() )
		{
			if( cc != NULL )
			{
				cc->shuffle();
			}
		}
	}
	
	if( footer != NULL )
	{
		if( footer->isColorSelected() )
		{
			if( cc != NULL )
			{
				cc->colorSelectMode();
			}
		}
	}

	if( footer != NULL )
	{
		if( footer->isAddSelected() )
		{
			if( cc != NULL )
			{
				cc->addCircle();
			}
		}
	}
	
	if( footer != NULL )
	{
		if( footer->isRemoveSelected() )
		{
			if( cc != NULL )
			{
				cc->removeCircle();
			}
		}
	}
	
	if( footer != NULL )
	{
		if( footer->isInfoSelected() )
		{
			if( infoScreen != NULL )
			{
				infoScreen->show();
			}
		}
	}
	
	if( footer != NULL )
	{
		if( footer->isHideSelected() )				// doesn't work, touch needs to pass through EAGLView to work.
		{
			ofPoint p = footer->getHidePoint();
			
			if( cc != NULL )
			{
				cc->down( p.x, p.y, 0 );
			}
		}
	}
	
	float gx;
	gx = ofxAccelerometer.getForce().y;
	gx *= 2;									// increase the reaction to tilt.
	gx = MIN( 1.0, MAX( -1.0, gx ) );			// between -1 and 1.
	
	float gy;
	gy = ofxAccelerometer.getForce().x;
	gy *= 2;									// increase the reaction to tilt.
	gy = MIN( 1.0, MAX( -1.0, gy ) );			// between -1 and 1.

	if( cc != NULL )
	{
		cc->setGravity( gx, gy );
		cc->update();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	if( cc != NULL )
		cc->draw();
	
	if( splashScreen != NULL )
		splashScreen->draw();
	
	if( upsideDown )
	{
		int cx = (int)( ofGetScreenWidth()  * 0.5 );
		int cy = (int)( ofGetScreenHeight() * 0.5 );
		
		glPushMatrix();
		glTranslatef( cx, cy, 0 );
		glRotatef( 180, 0, 0, 1 );
		glTranslatef( -cx, -cy, 0 );
	}
	
	if( infoScreen != NULL )
		infoScreen->draw();
	
	ofSetColor( 0, 0, 0 );
	ofDrawBitmapString( ofToString( ofGetFrameRate(),  0 ), ofGetScreenWidth() - 30, 20 );
	
	if( upsideDown )
	{
		glPopMatrix();
	}
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch)
{
	if( infoScreen )
		infoScreen->hide();
	
	if( lastTouchId != touch.id )
	{
		lastTouchId			= touch.id;
		lastTouch.x			= touch.x;
		lastTouch.y			= touch.y;
		lastTouchMoved.x	= touch.x;
		lastTouchMoved.y	= touch.y;
		lastTouchCount		= 0;
	}
	
	if( cc != NULL )
		cc->down( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch)
{
	if( lastTouchId == touch.id )
	{
		lastTouchMoved.x	= touch.x;
		lastTouchMoved.y	= touch.y;
	}
	
	if( cc != NULL )
		cc->drag( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch)
{
	if( lastTouchId == touch.id )
	{
		lastTouchId = -1;
	}
	
	if( cc != NULL )
		cc->up( touch.x, touch.y, touch.id );
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
			if( footer != NULL )
			{
				if( !footer->isShowing() )
				{
					footer->show();
				}
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch)
{
	if( footer != NULL )
		footer->toggleShow();
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

