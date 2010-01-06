/*
 *  Sydfest_06b.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 6/01/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "Sydfest_06.h"


void Sydfest_06 :: popCircles ()
{
	Circle_06		*circle;
	ofxBox2dCircle	*box2dCircle;
	
	int t;
	t = MIN( circles.size(), box2dCircles.size() );
	
	for( int i=0; i<circlePopRate; i++ )
	{
		if( t == 0 )
			break;
		
		int j;
		j = ofRandom( 0, t - 1 );
		
		circle		= &circles[ j ];
		box2dCircle = &box2dCircles[ j ];
		
		if( !circle->pop )
			circle->pop = true;
	}
	
	for( int i=0; i<t; i++ )
	{
		circle		= &circles[ i ];
		box2dCircle = &box2dCircles[ i ];
		
		if( circle->pop )
		{
			if( circle->radius < 0.5 )
			{
				box2dCircle->destroyShape();
				
				box2dCircles.erase( box2dCircles.begin() + i );
				circles.erase( circles.begin() + i );
				
				continue;
			}
			
			circle->alpha	*= circle->decay;
			circle->radius	*= circle->decay * 0.98;	// slightly slower.

			box2dCircle->setRadius( circle->radius );
		}
	}
}

void Sydfest_06 :: inflateCircles ()
{
	for( int i=0; i<circles.size(); i++ )
	{
		ofxBox2dCircle *box2dCircle;
		box2dCircle = &box2dCircles.at( i );
		
		Circle_06 *circle;
		circle = &circles[ i ];
		
		if( circle->pop == false )
			continue;
		
		circle->bounce.update();
		circle->radius	= MAX( 0.1, circle->bounce.position() );
		
		circle->vel.x	= ofRandom( -0.1, 0.1 );
		circle->vel.y	= MAX( -10, circle->vel.y - 0.1 );
		
		box2dCircle->setRadius( circle->radius );
		box2dCircle->setVelocity( circle->vel.x, circle->vel.y );
		box2dCircle->enableGravity( false );
		box2dCircle->setPhysics( cMass, 0.5, cFriction );
	}
}