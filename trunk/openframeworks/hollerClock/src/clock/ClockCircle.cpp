/*
 *  ClockCircle.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockCircle.h"

ClockCircle :: ClockCircle ( float radius, int color )
{
	set( radius, color );
}

ClockCircle :: ~ClockCircle ()
{
	//
}

void ClockCircle :: enableGravity ( bool b )
{
	ClockCircleB2 :: enableGravity( b );
	
//	if( body != NULL )	// same as for radius.
//	{
//		float rad = getRadius();
//		
//		for( b2Shape* s=body->GetShapeList(); s; s=s->GetNext() )
//		{
//			body->DestroyShape(s);
//		}
//		
//		circle.radius	    = rad / OFX_BOX2D_SCALE;
//		circle.density		= mass;
//		circle.restitution  = bounce;
//		circle.friction		= friction;
//		
//		//body = world->CreateBody(&bodyDef);
//		body->SetLinearVelocity(b2Vec2(0.0, 0.0));
//		body->CreateShape(&circle);
//		body->SetMassFromShapes();
//	}
}

void ClockCircle :: draw ()
{
	if( false )
	{
		ClockCircleB2 :: draw();
		
		return;
	}
	
	float radius = getRadius();
	
	glPushMatrix();
	glTranslatef( getPosition().x, getPosition().y, 0 );
	
	ofCircle( 0, 0, radius - 1 );
	
	glPopMatrix();
}