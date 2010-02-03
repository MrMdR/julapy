/*
 *  Sydfest_03.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 23/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sydfest_03.h"

Sydfest_03 :: Sydfest_03 ()
{
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofBackground( 0, 0, 0 );
	
	frameStart = ofGetFrameNum();
}

Sydfest_03 :: ~Sydfest_03()
{
	//
}

void Sydfest_03 :: setup ()
{
	circlePacker.loadFromFile( "circle_data_05_becks" );
	circles = circlePacker.getCircles();
	
	for( int i=0; i<circles->size(); i++ )
	{
		circleAnims.push_back( CircleAnim() );
		
		CircleAnim *circle;
		circle = &circleAnims.back();
		
		circle->color		= circles->at( i ).color;
		circle->loc.x		= circles->at( i ).loc.x;
		circle->loc.y		= circles->at( i ).loc.y;
		circle->radiusMax	= circles->at( i ).radius;
	}
	
	reset();
	
	//--
	
	box2d.init();
	box2d.setGravity( -10, 10 );
//	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	
	shapes.loadFromFile( "shapes_data" );
	
	for( int i=0; i<shapes.getShapesNum(); i++ )
	{
		box2dLineStrips.push_back( ofxBox2dLine() );
		
		ofxBox2dLine *lineStrip;
		lineStrip = &box2dLineStrips.back();
		
		ofPoint p;
		lineStrip->setWorld( box2d.getWorld() );
		
		vector<double*> poly = shapes.getShapeAt( i );
		
		bool bReversePoints;
		bReversePoints = false;
		
		if( i == 0 )
			bReversePoints = true;
		
		if( bReversePoints )
		{
			for( int j=poly.size()-1; j>=0; j-- )
			{
				p.x = poly[ j ][ 0 ];
				p.y = poly[ j ][ 1 ];
				lineStrip->addPoint( p.x, p.y );
			}
		}
		else
		{
			for( int j=0; j<poly.size(); j++ )
			{
				p.x = poly[ j ][ 0 ];
				p.y = poly[ j ][ 1 ];
				lineStrip->addPoint( p.x, p.y );
			}
		}
		
		lineStrip->createShape();
	}
}

void Sydfest_03 :: update ()
{
	int frameNum;
	frameNum = ofGetFrameNum() - frameStart;
	
	for( int i=0; i<circleAnims.size(); i++ )
	{
		if( circleAnims[ i ].frameDelay == frameNum )
		{
			circleAnims[ i ].playing = true;
		}

		if( circleAnims[ i ].playing  )
		{
			circleAnims[ i ].bounce.update();
			circleAnims[ i ].radius = circleAnims[ i ].bounce.position();
//			circleAnims[ i ].radius += ( circleAnims[ i ].radiusMax - circleAnims[ i ].radius ) * circleAnims[ i ].radiusEase;
		}
	}
}

void Sydfest_03 :: draw ()
{
	for( int i=0; i<circleAnims.size(); i++ )
	{
		ofSetColor( circleAnims[ i ].color );
		
		ofFill();
		ofCircle( circleAnims[ i ].loc.x, circleAnims[ i ].loc.y, circleAnims[ i ].radius );
		ofNoFill();
		ofCircle( circleAnims[ i ].loc.x, circleAnims[ i ].loc.y, circleAnims[ i ].radius );
	}
	
//	for( int i=0; i<box2dLineStrips.size(); i++ )
//	{
//		box2dLineStrips[ i ].draw();
//	}
}

void Sydfest_03 :: drawDebug ()
{
	
}

void Sydfest_03 :: reset ()
{
	frameStart = ofGetFrameNum();
	
	float radiusMax;
	radiusMax = 0;

	for( int i=0; i<circleAnims.size(); i++ )
	{
		if( circles->at( i ).radius > radiusMax )
			radiusMax = circles->at( i ).radius;
	}
	
	for( int i=0; i<circleAnims.size(); i++ )
	{
		float p;
		p = circles->at( i ).radius / radiusMax;
		
		circleAnims[ i ].radius			= 0;
		circleAnims[ i ].radiusEase		= 0.05;
		circleAnims[ i ].frameDelay		= (int)( ( 1 - p ) * ( 100 + ofRandom( 0, 500 ) ) );
		circleAnims[ i ].playing		= false;
		circleAnims[ i ].bounce.center( circles->at( i ).radius );
		circleAnims[ i ].bounce.position( 0 );
		circleAnims[ i ].bounce.inertia( 0.85 );
		circleAnims[ i ].bounce.springconst( 0.3 );
	}
}

void Sydfest_03 :: keyReleased ( int key )
{
	if( key == 'r' )
	{
		reset();
	}
}