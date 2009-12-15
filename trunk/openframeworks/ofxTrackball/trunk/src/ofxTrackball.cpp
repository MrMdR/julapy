/*
 *  ofxTrackball.cpp
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 14/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxTrackball.h"

ofxTrackball :: ofxTrackball()
{
	reset();
	setCenter( 0, 0 );
	setRadius( 300 );
	setDamping( 0.95 );
	
	enableMouseEvents();
	enableAppEvents();
	
	tolerance = 0.001;
}

ofxTrackball :: ~ofxTrackball()
{
	disableMouseEvents();
	disableAppEvents();
}

void ofxTrackball :: enableMouseEvents ()
{
	ofAddListener( ofEvents.mousePressed,  this, &ofxTrackball :: mouseDown );
	ofAddListener( ofEvents.mouseDragged,  this, &ofxTrackball :: mouseMoved );
	ofAddListener( ofEvents.mouseReleased, this, &ofxTrackball :: mouseUp );
}

void ofxTrackball :: disableMouseEvents ()
{
	ofRemoveListener( ofEvents.mousePressed,  this, &ofxTrackball :: mouseDown );
	ofRemoveListener( ofEvents.mouseDragged,  this, &ofxTrackball :: mouseMoved );
	ofRemoveListener( ofEvents.mouseReleased, this, &ofxTrackball :: mouseUp );
}

void ofxTrackball :: enableAppEvents ()
{
	ofAddListener( ofEvents.update, this, &ofxTrackball :: update );
}

void ofxTrackball :: disableAppEvents()
{
	ofRemoveListener( ofEvents.update, this, &ofxTrackball :: update );
}

void ofxTrackball :: mouseDown ( ofMouseEventArgs &e )
{
	isMouseDown = true;
	
	mouseCurr.x = e.x;
	mouseCurr.y = e.y;
}

void ofxTrackball :: mouseUp ( ofMouseEventArgs &e )
{
	isMouseDown = false;
}

void ofxTrackball :: mouseMoved ( ofMouseEventArgs &e )
{
	mousePrev.x = mouseCurr.x;
	mousePrev.y = mouseCurr.y;
	
	mouseCurr.x = e.x;
	mouseCurr.y = e.y;
	
	mouseNew.x	= mouseCurr.x;
	mouseNew.y	= mouseCurr.y;
	
	if( isMouseDown )
	{
		mouseVel = mouseCurr - mousePrev;
		
		rollToTrackball();
	}
}

void ofxTrackball :: update ( ofEventArgs &e )
{
	if( isMouseDown )
		return;
	
	float mouseVelLength;
	mouseVelLength = (float)sqrt( mouseVel.x * mouseVel.x + mouseVel.y * mouseVel.y );
	
	if( mouseVelLength == 0 )
		return;
	
	mouseVel *= damping;
	
	mouseVelLength = (float)sqrt( mouseVel.x * mouseVel.x + mouseVel.y * mouseVel.y );
	
	if( mouseVelLength < 0.01 )
	{
		mouseVel.set( 0, 0 );
		return;
	}

	mousePrev.x = mouseCurr.x;
	mousePrev.y = mouseCurr.y;
	
	mouseNew.x	= mouseCurr.x + mouseVel.x;
	mouseNew.y	= mouseCurr.y + mouseVel.y;
	
	rollToTrackball();	
}

void ofxTrackball :: rotate ()
{
	glRotatef( rotation[ 0 ], rotation[ 1 ], rotation[ 2 ], rotation[ 3 ] );
}

void ofxTrackball :: reset ()
{
	rotation[ 0 ] = 0;
	rotation[ 1 ] = 0;
	rotation[ 2 ] = 0;
	rotation[ 3 ] = 0;
	
	mouseCurr.set( 0, 0 );
	mousePrev.set( 0, 0 );
	mouseVel.set(  0, 0 );
	mouseNew.set(  0, 0 );
	
	isMouseDown	= false;
}

//-- setters.

void ofxTrackball :: setCenter ( int x, int y )
{
	center.x = x;
	center.y = y;
}

void ofxTrackball :: setRadius ( float r )
{
	radius = r;
}

void ofxTrackball :: setDamping	( float d )
{
	damping = d;
}

/////////////////////////////////////////////////////////////////////////
//	
//	QUATERNION ROTATION.
//
//---- Original code from ::
//
//  trackball.m
//
//  Created by daves on Wed May 03 2000.
//	Modified by ggs Dec 6 2002
//	Modified by julapy Dec 15 2009
//  Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
//
//	http://developer.apple.com/mac/library/samplecode/CocoaGL/listing13.html
//
/////////////////////////////////////////////////////////////////////////

void ofxTrackball :: rollToTrackball ()
{
	float sPoint[ 3 ];	// start point.
	float ePoint[ 3 ];	// end point.
	
	sPoint[ 0 ] = mousePrev.x - center.x;
	sPoint[ 1 ] = mousePrev.y - center.y;
	sPoint[ 2 ] = radius * radius - ( sPoint[ 0 ] * sPoint[ 0 ] + sPoint[ 1 ] * sPoint[ 1 ] );
	sPoint[ 2 ] = MAX( sPoint[ 2 ], 0.0 );
	sPoint[ 2 ] = sqrt( sPoint[ 2 ] );
	
	ePoint[ 0 ] = mouseNew.x - center.x;
	ePoint[ 1 ] = mouseNew.y - center.y;
	ePoint[ 2 ] = radius * radius - ( ePoint[ 0 ] * ePoint[ 0 ] + ePoint[ 1 ] * ePoint[ 1 ] );
	ePoint[ 2 ] = MAX( ePoint[ 2 ], 0.0 );
	ePoint[ 2 ] = sqrt( ePoint[ 2 ] );
	
    if
	(
		fabs( ePoint[ 0 ] - sPoint[ 0 ]) < tolerance && 
		fabs( ePoint[ 1 ] - sPoint[ 1 ]) < tolerance
	)
	return;		// Not enough change in the vectors to have an action.
	
	float rot[ 4 ];
    float cosAng, sinAng;
    float ls, le, lr;
	
    rot[ 1 ] =  sPoint[ 1 ] * ePoint[ 2 ] - sPoint[ 2 ] * ePoint[ 1 ];	// cross product.  r = s X e
    rot[ 2 ] = -sPoint[ 0 ] * ePoint[ 2 ] + sPoint[ 2 ] * ePoint[ 0 ];
    rot[ 3 ] =  sPoint[ 0 ] * ePoint[ 1 ] - sPoint[ 1 ] * ePoint[ 0 ];
    
    cosAng	= sPoint[ 0 ] * ePoint[ 0 ] + sPoint[ 1 ] * ePoint[ 1 ] + sPoint[ 2 ] * ePoint[ 2 ];			// (s . e) dot product.
    ls		= sqrt( sPoint[ 0 ] * sPoint[ 0 ] + sPoint[ 1 ] * sPoint[ 1 ] + sPoint[ 2 ] * sPoint[ 2 ] );	// length of starting point from center.
    ls		= 1.0 / ls;																						// inverse of starting point length.
    le		= sqrt( ePoint[ 0 ] * ePoint[ 0 ] + ePoint[ 1 ] * ePoint[ 1 ] + ePoint[ 2 ] * ePoint[ 2 ] );	// length of end point from center.
    le		= 1.0 / le;																						// inverse of end point length.
    cosAng	= cosAng * ls * le;
    
	lr		 = sqrt( rot[ 1 ] * rot[ 1 ] + rot[ 2 ] * rot[ 2 ] + rot[ 3 ] * rot[ 3 ] ); // length of the cross product of the starting point and end point. || r = s X e ||
    sinAng	 = lr * ls * le;
    rot[ 0 ] = (float)atan2( sinAng, cosAng ) * RAD_TO_DEG; 
    
    lr		 = 1.0 / lr;	// normalize the rotation axis.
    rot[ 1 ] *= lr;
	rot[ 2 ] *= lr;
	rot[ 3 ] *= lr;
	
	if( rot[ 0 ] != 0.0 )
	{
		addToRotationTrackball( rot, rotation );
	}
}

void ofxTrackball :: addToRotationTrackball ( float * dA, float * A )
{
    float q0[ 4 ], q1[ 4 ], q2[ 4 ];
    float theta2, sinTheta2;
    
    // Figure out A' = A . dA
    // In quaternions: let q0 <- A, and q1 <- dA.
    // Figure out q2 = q1 + q0 (note the order reversal!).
    // A' <- q3.
    
    rotation2Quat( A,  q0 );
    rotation2Quat( dA, q1 );
    
    // q2 = q1 + q0;
    q2[ 0 ] = q1[ 1 ] * q0[ 2 ] - q1[ 2 ] * q0[ 1 ] + q1[ 3 ] * q0[ 0 ] + q1[ 0 ] * q0[ 3 ];
    q2[ 1 ] = q1[ 2 ] * q0[ 0 ] - q1[ 0 ] * q0[ 2 ] + q1[ 3 ] * q0[ 1 ] + q1[ 1 ] * q0[ 3 ];
    q2[ 2 ] = q1[ 0 ] * q0[ 1 ] - q1[ 1 ] * q0[ 0 ] + q1[ 3 ] * q0[ 2 ] + q1[ 2 ] * q0[ 3 ];
    q2[ 3 ] = q1[ 3 ] * q0[ 3 ] - q1[ 0 ] * q0[ 0 ] - q1[ 1 ] * q0[ 1 ] - q1[ 2 ] * q0[ 2 ];
    // Here's an excersize for the reader: it's a good idea to re-normalize your quaternions
    // every so often.  Experiment with different frequencies.
    
    // An identity rotation is expressed as rotation by 0 about any axis.
    // The "angle" term in a quaternion is really the cosine of the half-angle.
    // So, if the cosine of the half-angle is one (or, 1.0 within our tolerance),
    // then you have an identity rotation.
    if( fabs( fabs( q2[ 3 ] - 1.0 ) ) < 1.0e-7 )	// Identity rotation.
	{
        A[ 0 ] = 0.0;
        A[ 1 ] = 1.0;
        A[ 2 ] = A[ 3 ] = 0.0;
        return;
    }
    
    // If you get here, then you have a non-identity rotation.  In non-identity rotations,
    // the cosine of the half-angle is non-0, which means the sine of the angle is also
    // non-0.  So we can safely divide by sin(theta2).
    
    // Turn the quaternion back into an {angle, {axis}} rotation.
    theta2		= acos( q2[ 3 ] );
    sinTheta2	= 1.0 / sin( theta2 );
    A[ 0 ] = theta2 * 2.0 * RAD_TO_DEG;
    A[ 1 ] = q2[ 0 ] * sinTheta2;
    A[ 2 ] = q2[ 1 ] * sinTheta2;
    A[ 3 ] = q2[ 2 ] * sinTheta2;
}

void ofxTrackball :: rotation2Quat ( float *A, float *q )
{
    float ang2;		// The half-angle
    float sinAng2;	// sin(half-angle)
    
    // Convert a GL-style rotation to a quaternion.  The GL rotation looks like this:
    // {angle, x, y, z}, the corresponding quaternion looks like this:
    // {{v}, cos(angle/2)}, where {v} is {x, y, z} / sin(angle/2).
    
    ang2	= A[ 0 ] * DEG_TO_RAD * 0.5;  // Convert from degrees ot radians, get the half-angle.
    sinAng2 = sin( ang2 );
    q[ 0 ] = A[ 1 ] * sinAng2;
	q[ 1 ] = A[ 2 ] * sinAng2;
	q[ 2 ] = A[ 3 ] * sinAng2;
    q[ 3 ] = cos( ang2 );
}