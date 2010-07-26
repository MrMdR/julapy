/*
 *  ClockCircle.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockCircle.h"

ClockCircle :: ClockCircle ( float r, int c )
{
	radius		= r;
	colorHex	= c;
	
	colorTrgt.r	= colorCurr.r = ( c >> 16 ) & 0xff;
	colorTrgt.g	= colorCurr.g = ( c >> 8  ) & 0xff;
	colorTrgt.b	= colorCurr.b = ( c >> 0  ) & 0xff;
	
	spinDir = ( ofRandom( 0.0, 1.0 ) > 0.5 ) ? 1 : -1;
	spinFrc	= ofRandom( 0.2, 0.5 );
	
	active	= false;
}

ClockCircle :: ~ClockCircle ()
{
	//
}

void ClockCircle :: enableGravity ( bool b )
{
	ofxBox2dCircle :: enableGravity( b );
}

void ClockCircle :: update ()
{
	float angle			= getRotation();
	const b2XForm& xf	= body->GetXForm();
	b2Vec2	center		= body->GetPosition();
	b2Vec2	axis		= xf.R.col1;
	b2Vec2	p			= center + ( radius * 0.6 ) / OFX_BOX2D_SCALE * axis;
	
	eye.x = p.x * OFX_BOX2D_SCALE;
	eye.y = p.y * OFX_BOX2D_SCALE;
	
	colorCurr.r += ( colorTrgt.r - colorCurr.r ) * 0.1;
	colorCurr.g += ( colorTrgt.g - colorCurr.g ) * 0.1;
	colorCurr.b += ( colorTrgt.b - colorCurr.b ) * 0.1;
}

void ClockCircle :: draw ()
{
	if( false )
	{
		ofxBox2dCircle :: draw();
		
		return;
	}
	
	float radius = getRadius();
	
	glPushMatrix();
	glTranslatef( getPosition().x, getPosition().y, 0 );
	
	ofCircle( 0, 0, radius - 1 );
	
	glPopMatrix();

	//--
	
	glPushMatrix();
	glTranslatef( eye.x, eye.y, 0 );
	
	int col = 220;
	
	ofSetColor( col, col, col );
	ofCircle( 0, 0, radius * 0.1 );
	
	
	glPopMatrix();
}