#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofBackground( 30, 30, 30 );
	ofSetFrameRate( 60 );
	ofSetVerticalSync( true );
	ofEnableSmoothing();
	
	bMouseDown = false;
	
	scale1	= 0;
	scale2	= 0;
	scale3	= 0;
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	float mx;
	mx = mouseX / (float)ofGetWidth();
	mx = MAX( 0.0, MIN( mx, 1.0 ) );
	scale1	= mx;
	scale2	= mx * 140;
	scale3	= mx * 0.1;
	
	cu.smooth( curve, curveSmooth, scale1 );
	cu.simplify( curve, curveSimplify, scale2 );
//	cu.simplify( curveSmooth, curveSimplify, simplifyValue );
	cs.simplify( curve, curveCvSimplify, scale3 );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	int thick = 4;
	
	ofFill();
	ofSetColor( 255, 255, 255 );
	ofRect( 0, (int)( ( ofGetHeight() - thick ) * 0.5 ), ofGetWidth(), thick );
	ofRect( (int)( ( ofGetWidth() - thick ) * 0.5 ), 0, thick, ofGetHeight() );
	
	//-- curve points.
	
	ofSetColor( 255, 255, 255 );
	ofDrawBitmapString( "1) curve - draw in this window - press space to reset", 10, 20 );
	drawCurve( curve );
	
	//-- curve smooth.
	
	glPushMatrix();
	glTranslatef( (int)( ofGetWidth() * 0.5 ), 0, 0 );

	ofSetColor( 255, 255, 255 );
	ofDrawBitmapString( "2) curve smooth :: " + ofToString( scale1, 2 ) + " - pretty much useless.", 10, 20 );
	drawCurve( curveSmooth );
	
	glPopMatrix();
	
	//-- curve simplify.
	
	glPushMatrix();
	glTranslatef( 0, (int)( ofGetHeight() * 0.5 ), 0 );
	
	ofSetColor( 255, 255, 255 );
	ofDrawBitmapString( "3) curve simplify :: " + ofToString( scale2, 2 ), 10, 20 );
	drawCurve( curveSimplify );
	
	glPopMatrix();
	
	//-- curve fitted.
	
	glPushMatrix();
	glTranslatef( (int)( ofGetWidth() * 0.5 ), (int)( ofGetHeight() * 0.5 ), 0 );
	
//	ofSetColor( 255, 255, 255 );
//	ofDrawBitmapString( "4) curve fitted using cutmull-rom", 10, 20 );
//	drawFittedCurve( curveSimplify );

	ofSetColor( 255, 255, 255 );
	ofDrawBitmapString( "3) curve simplify opencv :: " + ofToString( scale3, 2 ), 10, 20 );
	drawCurve( curveCvSimplify );
	
	glPopMatrix();
}

void testApp :: drawCurve ( const vector<ofPoint>& points )
{
	for( int i=0; i<points.size(); i++ )
	{
		ofSetColor( 0, 255, 255 );
		ofCircle( points[ i ].x, points[ i ].y, 2 );
		
		int j = i + 1;

		if( j < points.size() )
		{
			ofSetColor( 255, 0, 255 );
			ofLine( points[ i ].x, points[ i ].y, points[ j ].x, points[ j ].y );
		}
	}
}

void testApp :: drawFittedCurve ( const vector<ofPoint>& points )
{
	ofFill();
	ofSetColor( 0, 255, 255 );
	
	int t = points.size();
	
	for( int i=0; i<t; i++ )
	{
		ofCircle( points[ i ].x, points[ i ].y, 2 );
	}

	if( t < 2 )
		return;
	
	ofNoFill();
	ofSetColor( 255, 0, 255 );
	
	ofBeginShape();
	
	ofCurveVertex( points[ 0 ].x, points[ 0 ].y );
	
	for( int i=0; i<t; i++ )
	{
		ofCurveVertex( points[ i ].x, points[ i ].y );
	}
	
	ofCurveVertex( points[ t - 1 ].x, points[ t - 1 ].y );
	
	ofEndShape( false );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == ' ' )
	{
		curve.clear();
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{
	//
}

void testApp::mouseDragged(int x, int y, int button)
{
	bool b1 = x <= ofGetWidth()  * 0.5;
	bool b2 = y <= ofGetHeight() * 0.5;
	bool b3 = x >= 0;
	bool b4 = y >= 0;
	
	if( b1 && b2 && b3 && b4 )
	{
		curve.push_back( ofPoint( x, y ) );
	}
}

void testApp::mousePressed(int x, int y, int button)
{
	bool b1 = x <= ofGetWidth()  * 0.5;
	bool b2 = y <= ofGetHeight() * 0.5;
	bool b3 = x >= 0;
	bool b4 = y >= 0;
	
	if( b1 && b2 && b3 && b4 )
	{
		curve.push_back( ofPoint( x, y ) );
	}
	
	bMouseDown = true;
}

void testApp::mouseReleased(int x, int y, int button)
{
	bMouseDown = false;
}

void testApp::windowResized(int w, int h)
{

}

