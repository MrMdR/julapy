/*
 *  Sydfest_02.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 23/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sydfest_02.h"

Sydfest_02 :: Sydfest_02 ()
{
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofBackground( 0, 0, 0 );
}

Sydfest_02 :: ~Sydfest_02()
{
	//
}

void Sydfest_02 :: setup ()
{
	circlePacker.loadFromFile( "circle_data_04_becks" );
	circles = circlePacker.getCircles();
	
	box2d.init();
	box2d.setGravity( -10, 10 );
//	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	
	for( int i=0; i<circles->size(); i++ )
	{
		box2dCircles.push_back( ofxBox2dCircle() );
		
		ofxBox2dCircle *circle;
		circle = &box2dCircles.back();
		
		float m;
		m = circles->at( i ).radius / 100.0;
		
		circle->setPhysics
		(
			0.0001,		// mass.
			0.4,		// bounce.
			0.01		// friction.
		);
		
		circle->setup
		(
			box2d.getWorld(),
			circles->at( i ).loc.x,
			circles->at( i ).loc.y,
			circles->at( i ).radius
		);
	}
	
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

void Sydfest_02 :: update ()
{
	box2d.update();
}

void Sydfest_02 :: draw ()
{
	for( int i=0; i<circles->size(); i++ )
	{
		ofSetColor( circles->at( i ).color );
		
		ofxBox2dCircle *circle;
		circle = &box2dCircles[ i ];
		
		ofPoint p = circle->getPosition();
		float	r = circle->getRadius();
		
		ofFill();
		ofCircle( p.x, p.y, r );
		ofNoFill();
		ofCircle( p.x, p.y, r );
	}
	
	for( int i=0; i<box2dLineStrips.size(); i++ )
	{
		box2dLineStrips[ i ].draw();
	}
}

void Sydfest_02 :: drawDebug ()
{
	
}
