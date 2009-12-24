/*
 *  CilindricoCollapse_01.cpp
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "CilindricoCollapse_01.h"

CilindricoCollapse_01 :: CilindricoCollapse_01()
{
	ofxTrigLUT :: createLUT ();
	
	glShadeModel( GL_SMOOTH ); 
	glEnable( GL_DEPTH_TEST );
}

CilindricoCollapse_01 :: ~CilindricoCollapse_01()
{
	reset();
}

void CilindricoCollapse_01 :: setup ()
{
	int   arcsTotal		= 1;
	float arcHeight		= 6;
	float arcLocZInc	= 50;
//	float arcLocZ 		= -(int)( ( arcsTotal * ( arcHeight + arcLocZInc ) ) * 0.5f );
	float arcLocZ 		= 500;
	float arcAngleMin	= 100;
	float arcAngleMax	= 270;
	
	float arcRadiusMin	= 50;
	float arcRadiusMax	= 200;
	float arcWidthMin	= 200;
	float arcWidthMax	= 400;
	float arcDispRange	= 100;
	float arcCosMult	= 0;
	
	glGenLists( arcsTotal * 2 );
	
	for( int i=0; i<arcsTotal; i++ )
	{
		arcBars.push_back( ArcBar() );
		
//		arcCosMult		= -cos( ( i / (float)( arcsTotal - 1 ) ) * PI + PI * 0.5 );
		arcCosMult		= 1.0;
		
		arcBars.back().id			= i * 2 + 1;
		arcBars.back().loc.z 		= arcLocZ;
		arcBars.back().height		= arcHeight;
		arcBars.back().radius		= ofRandom( arcRadiusMin, arcRadiusMax ) * arcCosMult;
		arcBars.back().width		= ofRandom( arcWidthMin, arcWidthMax ) * arcCosMult;
//		arcBars.back().angle		= ofRandom( arcAngleMin, arcAngleMax ) * ( 1 - arcCosMult );
		arcBars.back().angle		= ofRandom( arcAngleMin, arcAngleMax );
		arcBars.back().rot.z		= ofRandom( 0, 1 ) * 360;
		arcBars.back().rInc			= ofRandom( 0, 1 ) * 1.0 - 0.5;
		arcBars.back().dis.x		= ofRandom( 0, 1 ) * arcDispRange - arcDispRange * 0.5;
		arcBars.back().dis.y		= ofRandom( 0, 1 ) * arcDispRange - arcDispRange * 0.5;
		arcBars.back().dis.z		= ofRandom( 0, 1 ) * arcDispRange - arcDispRange * 0.5;
		arcBars.back().wireframePad	= ofRandom( 0, 1 ) * 20.0 + 10.0;
		
		arcBars.back().setup();
//		arcBars.back().createSolidModel();
		arcBars.back().createWireframe();
		
		arcLocZ += ( arcHeight + arcLocZInc );
	}
	
	//-- trackball.
	
	int cx, cy;
	cx = (int)( renderArea.width  * 0.5 );
	cy = (int)( renderArea.height * 0.5 );
	
	trackball.setCenter( cx, cy );
	trackball.setRadius( MAX( cx, cy ) );
}

void CilindricoCollapse_01 :: reset()
{
	trackball.reset();
	
	arcBars.clear();
}

void CilindricoCollapse_01 :: update()
{
	for( int i=0; i<arcBars.size(); i++ )
	{
		arcBars.at( i ).update();
	}
}

void CilindricoCollapse_01 :: draw()
{
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );			// smoothing.
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
//	drawBackground();
	
	ofxVec3f scene;
	scene.x = renderArea.width  * 0.5;
	scene.y = renderArea.height * 0.5;
	scene.z = renderArea.height * 0.5 / tan( PI * 60 / 360.0 );
	scene.z *= -1;
	
	glPushMatrix();
	glTranslatef( scene.x, scene.y, scene.z );
	
	trackball.rotate();
	
	for( int i=0; i<arcBars.size(); i++ )
	{
//		arcBars.at( i ).renderSolidModel();
		arcBars.at( i ).renderWireframe();
	}

	glPopMatrix();
}

void CilindricoCollapse_01 :: drawDebug ()
{
	//
}

void CilindricoCollapse_01 :: keyPressed( int key )
{
	if( key == 'r' )
	{
		reset();
		setup();
	}
}

void CilindricoCollapse_01 :: mouseDragged ( int x, int y, int button )
{
	//
}

void CilindricoCollapse_01 :: mousePressed ( int x, int y, int button )
{
	//
}

void CilindricoCollapse_01 :: mouseReleased ( int x, int y, int button )
{
	//
}