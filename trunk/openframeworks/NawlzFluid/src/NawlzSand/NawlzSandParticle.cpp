/*
 *  NawlzSandParticle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzSandParticle.h"

NawlzSandParticle :: NawlzSandParticle()
{
	loc = ofPoint( 0, 0 );
	acc = ofPoint( 0, 0 );
	vel = ofPoint( 0, 0 );
	
	sizeRadius		= 6.0;
	maxspeed		= 1.4;
	ease			= 0.2;
	
	wanderTheta		= 0.0;
	wanderRadius	= 16.0;
	wanderDistance	= 60.0;
	wanderChange	= 0.25;
	
	bUseImageForBounds = false;
	
	tex = NULL;
}

NawlzSandParticle :: ~NawlzSandParticle ()
{
	//
}

void NawlzSandParticle :: setLoc ( float x, float y )
{
	loc.x = x;
	loc.y = y;
}

void NawlzSandParticle :: setVel ( float x, float y )
{
	vel.x = x;
	vel.y = y;
}

void NawlzSandParticle :: setBounds ( const ofRectangle& rect )
{
	bounds = rect;
	
	boundsCenter.x = bounds.x + bounds.width  * 0.5;
	boundsCenter.y = bounds.y + bounds.height * 0.5;
}

void NawlzSandParticle :: setImageBounds ( const ofRectangle& rect, unsigned char* pixels )
{
	imageRect	= rect;
	imagePixels	= pixels;
	bUseImageForBounds = true;
}

void NawlzSandParticle :: setTexture ( ofTexture* tex )
{
	this->tex = tex;
}

void NawlzSandParticle :: update()
{
	wander();
	
	vel += acc;
	vel.limit( maxspeed );
	loc += vel;
	acc.set( 0, 0 );
}

void NawlzSandParticle :: wander()
{
	wanderTheta += ofRandom( -wanderChange, wanderChange );     // Randomly change wander theta
	
	circle = vel;				// Start with velocity
	circle.normalize();			// Normalize to get heading
	circle *= wanderDistance;	// Multiply by distance
	circle += loc;				// Make it relative to boid's location
	
	bool isOutside;
	isOutside = constrainToBorders( circle );
	
	ease = isOutside ? 1.0 : 0.2;
	
	circleOffSet	= ofxVec2f( wanderRadius * cos( wanderTheta ), wanderRadius * sin( wanderTheta ) );
	circleTarget	= circle + circleOffSet;
	acc				+= steer( circleTarget );
}

bool NawlzSandParticle :: constrainToBorders ( const ofxVec2f& target )
{
	bool isOutside = false;
	
	float d = ofDist( target.x, target.y, boundsCenter.x, boundsCenter.y );
	isOutside = d > bounds.width * 0.5;

	bool l, t ,r, b;
//	l = target.x < bounds.x;
//	t = target.y < bounds.y;
//	r = target.x > bounds.x + bounds.width;
//	b = target.y > bounds.y + bounds.height;
//	
//	isOutside = l || t || r || b;
	
	//--- target back to center of bounds.
	
	if( isOutside )
	{
		float cx = bounds.x + bounds.width  * 0.5;
		float cy = bounds.y + bounds.height * 0.5;
		
		float px = target.x - cx;
		float py = target.y - cy;
		
		ofxVec2f vec = ofxVec2f( px, py );
		float ang = vec.angle( ofxVec2f( 0, -1 ) );							// return an angle between -180 and 180.
		
		wanderTheta = ( ( ang + 180 ) / 360.0 ) * TWO_PI + PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
		wanderTheta *= -1;
	}
	
	return isOutside;
	
	//--- avoid off wall. this approach did not work.
	
	if( l )	wanderTheta = TWO_PI * 0;
	if( t ) wanderTheta = TWO_PI * 0.25;
	if( r ) wanderTheta = TWO_PI * 0.5;
	if( b ) wanderTheta = TWO_PI * 0.75;
	
	if( t && r ) wanderTheta = TWO_PI * 0.125;
	if( r && b ) wanderTheta = TWO_PI * 0.375;
	if( b && l ) wanderTheta = TWO_PI * 0.625;
	if( l && t ) wanderTheta = TWO_PI * 0.875;
	
	return isOutside;
}

bool NawlzSandParticle :: constrainToImage ( const ofxVec2f& target )
{
	bool isOutside = false;
	
	float px = ( target.x - imageRect.x ) / (float)imageRect.width;
	float py = ( target.y - imageRect.y ) / (float)imageRect.height;
	px = ofClamp( px, 0, 1 );
	py = ofClamp( py, 0, 1 );
	
	int iw	= imageRect.width;
	int ih	= imageRect.height;
	int ix	= px * iw;
	int iy	= py * ih;
	
	int p	= ( ( iy * iw ) + ix ) * 3;
	
	isOutside = imagePixels[ p ] == 255;
	
	if( isOutside )
	{
		float cx = imageRect.x + imageRect.width  * 0.5;
		float cy = imageRect.y + imageRect.height * 0.5;
		
		float px = target.x - cx;
		float py = target.y - cy;
		
		ofxVec2f vec = ofxVec2f( px, py );
		float ang = vec.angle( ofxVec2f( 0, -1 ) );							// return an angle between -180 and 180.
		
		wanderTheta = ( ( ang + 180 ) / 360.0 ) * TWO_PI + PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
		wanderTheta *= -1;
	}
	
	return isOutside;
}


ofxVec2f NawlzSandParticle :: steer( const ofxVec2f& target )
{
    ofxVec2f steer;
    ofxVec2f desired = target - loc;	// A vector pointing from the location to the target
    
	float d = desired.length();
    if( d > 0 )
	{
		desired.normalize();
		desired *= maxspeed;
		
		steer = desired - vel;
		steer.limit( ease );
	}
	else
	{
		steer.set( 0, 0 );
    }
	
    return steer;
}

void NawlzSandParticle :: draw()
{
	ofFill();
	ofSetColor( 0x6bada5 );
//	ofRect( (int)loc.x, (int)loc.y, 2, 2 );
	ofRect( loc.x, loc.y, 2, 2 );
	
	return;
	
	//---
	
	float r		= sizeRadius;
	float theta = -vel.angle( ofxVec2f( -1, 0 ) ) - 90;
	
	glPushMatrix();
	glTranslatef( loc.x, loc.y, 0 );
//	glRotatef( theta, 0, 0, 1 );
	
	if( tex )
	{
		tex->draw( 0, 0 );
		
		glPopMatrix();
		
		return;
	}
	
	ofFill();
	ofSetColor( 175, 175, 175 );
	
	ofBeginShape();
	ofVertex(  0, -r * 2 );
	ofVertex( -r,  r * 2 );
	ofVertex(  r,  r * 2 );
	ofEndShape( true );
	
	glPopMatrix();
}

void NawlzSandParticle :: drawDebug ()
{
	ofNoFill();
	ofEllipse( circle.x, circle.y, wanderRadius * 2, wanderRadius * 2 );
	ofEllipse( circleTarget.x, circleTarget.y, 4, 4 );
	ofLine( loc.x, loc.y, circle.x, circle.y );
	ofLine( circle.x, circle.y, circleTarget.x, circleTarget.y );
}
