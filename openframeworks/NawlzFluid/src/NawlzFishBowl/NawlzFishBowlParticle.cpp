/*
 *  WanderBoid.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 13/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzFishBowlParticle.h"

NawlzFishBowlParticle :: NawlzFishBowlParticle()
{
	loc = ofPoint( 0, 0 );
	acc = ofPoint( 0, 0 );
	vel = ofPoint( 0, 0 );
	
	float r = ofRandom( 0.0, 1.0 );
	r = (int)( r * 2 );
	r *= 2;
	
	size = r + 5;
	
	maxspeed	= ofRandom( 0.8, 1.5 );
	ease		= 0.2;

	wanderTheta		= ofRandom( 0, TWO_PI );
	wanderRadius	= 16.0;
	wanderDistance	= 30.0;
	wanderChange	= 0.25;
	
	rotation	= ofRandom( -180, 180 );
	rotationVel	= ofRandom( 4, 8 );
	rotationVel *= ( ofRandom( 0, 1.0 ) > 0.5 ) ? 1 : -1;
	rotationVel	*= ( ofRandom( 0, 1.0 ) > 0.9 ) ? 3 : 1;
	
	friction = ofRandom( 0.5, 1.0 );
	
//	colors.push_back( 0xc7c7c7 );
	colors.push_back( 0x929292 );
//	colors.push_back( 0x777777 );
	color = colors[ (int)ofRandom( 0, colors.size() ) ];
	
	bUseImageForBounds		= false;
	bIsOutsideImageBounds	= false;
	
	tex = NULL;
}
	
NawlzFishBowlParticle :: ~NawlzFishBowlParticle ()
{
	//
}

void NawlzFishBowlParticle :: setLoc ( float x, float y )
{
	loc.x = x;
	loc.y = y;
}

void NawlzFishBowlParticle :: setVel ( float x, float y )
{
	vel.x = x;
	vel.y = y;
}

void NawlzFishBowlParticle :: setBounds ( const ofRectangle& rect )
{
	bounds = rect;
}

void NawlzFishBowlParticle :: setImageBounds ( const ofRectangle& rect, unsigned char* pixels, int pixelDepth )
{
	imageRect		= rect;
	imagePixels		= pixels;
	imagePixelDepth	= pixelDepth;
	bUseImageForBounds = true;
}

void NawlzFishBowlParticle :: setTexture ( ofTexture* tex )
{
	this->tex = tex;
}

void NawlzFishBowlParticle :: update( float forceScale )
{
	wander();
	
	vel += acc * forceScale;
	vel.limit( maxspeed );
	loc += vel;
	acc.set( 0, 0 );
	
	rotation += rotationVel;
}

void NawlzFishBowlParticle :: wander()
{
	wanderTheta += ofRandom( -wanderChange, wanderChange );     // Randomly change wander theta
	
	circle = vel;				// Start with velocity
	circle.normalize();			// Normalize to get heading
	circle *= wanderDistance;	// Multiply by distance
	circle += loc;				// Make it relative to boid's location
	
	bool isOutside;
	if( bUseImageForBounds )
	{
		isOutside = bIsOutsideImageBounds = constrainToImage( circle );
	}
	else
	{
		isOutside = constrainToBorders( circle );
	}
	
	ease = isOutside ? 1.0 : 0.2;
	
	circleOffSet	= ofxVec2f( wanderRadius * cos( wanderTheta ), wanderRadius * sin( wanderTheta ) );
	circleTarget	= circle + circleOffSet;
	acc				+= steer( circleTarget );
}

bool NawlzFishBowlParticle :: constrainToBorders ( const ofxVec2f& target )
{
	bool isOutside = false;
	
	bool l = target.x < bounds.x;
	bool t = target.y < bounds.y;
	bool r = target.x > bounds.x + bounds.width;
	bool b = target.y > bounds.y + bounds.height;
	
	isOutside = l || t || r || b;
	
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

bool NawlzFishBowlParticle :: constrainToImage ( const ofxVec2f& target )
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
	
	int p	= ( ( iy * iw ) + ix ) * imagePixelDepth;
	
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


ofxVec2f NawlzFishBowlParticle :: steer( const ofxVec2f& target )
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

void NawlzFishBowlParticle :: draw()
{
	glPushMatrix();
	glTranslatef( loc.x, loc.y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	
	float s = size;
	
	ofFill();
	ofSetColor( color );
	ofRect( -0, -(int)( s * 0.5 ), 1, s );
	ofRect( -(int)( s * 0.5 ), -0, s, 1 );
	
	glPopMatrix();
	
	return;
	
	//---
	
	float r		= size;
	float theta = -vel.angle( ofxVec2f( -1, 0 ) ) - 90;
	
	glPushMatrix();
	glTranslatef( loc.x, loc.y, 0 );
	glRotatef( theta, 0, 0, 1 );
	
	if( tex )
	{
		tex->draw( -tex->getWidth() * 0.5, -tex->getHeight() * 0.5 );
		
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

void NawlzFishBowlParticle :: drawDebug ()
{
	ofNoFill();
	ofEllipse( circle.x, circle.y, wanderRadius * 2, wanderRadius * 2 );
	ofEllipse( circleTarget.x, circleTarget.y, 4, 4 );
	ofLine( loc.x, loc.y, circle.x, circle.y );
	ofLine( circle.x, circle.y, circleTarget.x, circleTarget.y );
}
