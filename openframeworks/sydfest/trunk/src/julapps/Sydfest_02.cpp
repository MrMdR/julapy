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
	ofBackground( 60, 60, 60 );
}

Sydfest_02 :: ~Sydfest_02()
{
	//
}

void Sydfest_02 :: setup ()
{
	circlePacker.loadFromFile();
	circles = circlePacker.getCircles();
	
	box2d.init();
	box2d.setGravity( 0, 10 );
	box2d.checkBounds( true );
	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	
	for( int i=0; i<circles->size(); i++ )
	{
		box2dCircles.push_back( ofxBox2dCircle() );
		
		ofxBox2dCircle *circle;
		circle = &box2dCircles.back();
		
		float s;
		s = circles->at( i ).radius / 100.0;
		
		circle->setPhysics
		(
			s * 10,
			s,
			0.0
		);
		
		circle->setup
		(
			box2d.getWorld(),
			circles->at( i ).loc.x,
			circles->at( i ).loc.y,
			circles->at( i ).radius
		);
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
}

void Sydfest_02 :: drawDebug ()
{
	
}
