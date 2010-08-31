/*
 *  ColorPhysics.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorPhysics.h"

ColorPhysics :: ColorPhysics ()
{
	box2d = NULL;
	
	setGravity( 0, 0 );
}

ColorPhysics :: ~ColorPhysics ()
{

}

///////////////////////////////////////////////////////
//	CONTACT LISTENER METHODS.
///////////////////////////////////////////////////////

void ColorPhysics :: Add( const b2ContactPoint* point )
{
	for( int i=0; i<circles.size(); i++ )
	{
		ColorCircle& circle = *circles[ i ];
		
		for( b2Shape* s=circle.body->GetShapeList(); s; s=s->GetNext() )
		{
			if( point->shape1 == s || point->shape2 == s )
			{
				float vel;								// weight colour change by velocity.
				vel = point->velocity.Length();
				
				if( vel > 5 )
				{
					circle.collision = 1.0;
//					circle.collision = vel / 30;
//					circle.collision = ofClamp( circle.collision, 0, 1 );
				}
				
				float vol;
				vol = vel / 100;
				vol = ofClamp( vol, 0, 1 );
				
//				sounds->playRandomPointCollisionSound( vol );
				sounds->playPointCollisionSoundAtIndex( i, vol );
			}
		}
	}
}

void ColorPhysics :: Remove(const b2ContactPoint* point)
{
	//
}

///////////////////////////////////////////////////////
//	SETTERS.
///////////////////////////////////////////////////////

void ColorPhysics :: setScreen ( ofxScreen screen )
{
	this->screen = screen;
}

void ColorPhysics :: setGravity	( float gx, float gy )
{
	gravity.x = MIN( 1.0, MAX( -1.0, gx ) );
	gravity.y = MIN( 1.0, MAX( -1.0, gy ) );
	
	gravity.x *= 20;
	gravity.y *= 20;
	
	if( box2d != NULL )
	{
		box2d->setGravity( gravity.x, gravity.y );
	}
}

void ColorPhysics :: setSounds ( ColorSound* sounds )
{
	this->sounds = sounds;
}

///////////////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////////////

void ColorPhysics :: setup ()
{
	box2d = new ofxBox2d();
	box2d->init();
	box2d->setGravity( 0, 0 );
	box2d->setFPS( 30.0 );
//	box2d->registerGrabbing();
	box2d->getWorld()->SetContactListener( this );		// register contact class.
	
	circleRadius = areaToRadius( 0.001 );
	
	createBounds();
	createCircles();
	createJoints();
}

void ColorPhysics :: createBounds ()
{
	b2BodyDef bd;
	bd.position.Set( 0, 0 );
	ground = box2d->ground = box2d->world->CreateBody( &bd );
	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	sd.density		= 0.0f;
	sd.restitution	= 0.0f;
	sd.friction		= 0.6;
	
	float thick		= 0.02 * screen.screenHeight;
	
	int w = screen.screenWidth;
	int h = screen.screenHeight;
	
	sd.SetAsBox		//-- right		( float32 hx, float32 hy, const b2Vec2& center, float32 angle )
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2,
		b2Vec2( ( w + thick + circleRadius ) / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape( &sd );
	
	sd.SetAsBox		//-- left
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2, 
		b2Vec2( -( thick + circleRadius ) / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- top
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, -( thick + circleRadius ) /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- bottom
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, ( h + thick + circleRadius ) /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
}

void ColorPhysics :: createCircles ()
{
	int numOfCircle;
	numOfCircle = 25;
#ifdef TARGET_OF_IPHONE
	numOfCircle = 25;
#endif
	
	int t = numOfCircle;
	for( int i=0; i<t; i++ )
	{
		addCircle();
	}
}

void ColorPhysics :: addCircle ()
{
	ColorCircle* circle;
	circle = new ColorCircle();
	
	//-- define line up point.
	
	circle->setScreen( screen );
	circle->init();		// do things here before creating the circle in box2d.
	
	//-- define circles physics.
	
	float mass		= 3.0;
	float bounce	= 0.53;
	float friction	= 0.1;
	
	bool bEnableGravity;
	bEnableGravity = ofRandom( 0.0, 1.0 ) > 0.5;
	
	circle->enableGravity( bEnableGravity );
	circle->setPhysics( mass, bounce, friction );
	circle->setup( box2d->getWorld(), ofRandom( 0, screen.screenWidth ), ofRandom( 0, screen.screenHeight ), circleRadius, false );
	circle->setRotationFriction( 1.0 );
	circle->setDamping( 1.0 );
	circle->body->AllowSleeping( false );
	
	//-- add to vectors.
	
	circles.push_back( circle );
}

bool ColorPhysics :: addSingleCircle ()
{
	if( circles.size() <= CIRCLES_MAX )
	{
		addCircle();
		createJointsForCircle( circles.back() );
		
		return true;
	}
	
	return false;
}

bool ColorPhysics :: removeCircle ()
{
	if( circles.size() > CIRCLES_MIN )
	{
		int i;
		
		//-- remove last joint.
		
//		i = joints.size() - 1;
//		
//		if( i > 0 )
//		{
//			box2d->getWorld()->DestroyJoint( joints[ i ] );
//			joints.erase( joints.begin() + i );
//		}
		
		destroyJoints();
		
		//-- remove last circle.
		
		i = circles.size() - 1;

		if( i > 0 )
		{
			circles[ i ]->destroyShape();
			circles.erase( circles.begin() + i );
		}
		
		createJoints();
		
		return true;
	}
	
	return false;
}

float ColorPhysics :: areaToRadius ( float area )
{
	float r;
	r = sqrt( area * screen.screenArea );
	r *= 0.5;
	
	return r;
}

bool ColorPhysics :: checkHit ( float x, float y )
{
	b2Vec2 p( x / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE );
	
	for( int i=0; i<circles.size(); i++ )
	{
		ColorCircle* circle = circles[ i ];
		
		for( b2Shape* s=circle->body->GetShapeList(); s; s=s->GetNext() )
		{
			b2Body* shapeBody = s->GetBody();
			
			bool inside;
			inside = s->TestPoint( shapeBody->GetXForm(), p );
			
			if( inside )
			{
				return true;
			}
		}
	}
	
	return false;
}

void ColorPhysics :: showCircles ()
{
	
}

void ColorPhysics :: hideCircles ()
{
	
}

///////////////////////////////////////////////////////
//	JOINTS.
///////////////////////////////////////////////////////

void ColorPhysics :: createJoints ()
{
	for( int i=0; i<circles.size(); i++ )
	{
		createJointsForCircle( circles[ i ] );
	}
}

void ColorPhysics :: createJointsForCircle	( ColorCircle* circle )
{
	int r = (int)ofRandom( 0, circles.size () - 1 );
	
	ColorCircle& c1 = *circle;
	ColorCircle& c2 = *circles[ r ];
	
	b2DistanceJointDef jd;
	b2Vec2 p1, p2, d;
	
	jd.frequencyHz  = 10.0;
	jd.dampingRatio = 0.05;
	
	jd.body1 = c1.body;
	jd.body2 = c2.body;
	
	jd.localAnchor1.Set( 0, 0 );
	jd.localAnchor2.Set( 0, 0 );
	jd.collideConnected = false;
	
	p1	= jd.body1->GetWorldPoint( jd.localAnchor1 );
	p2	= jd.body2->GetWorldPoint( jd.localAnchor2 );
	d	= p2 - p1;
	
	float length;
	length = ofRandom( 0.3, 0.5 ) * screen.screenMin;
	
	jd.length = length / OFX_BOX2D_SCALE;
	
	b2DistanceJoint* joint;
	joint = (b2DistanceJoint*)box2d->getWorld()->CreateJoint( &jd );
	
	joints.push_back( joint );
}

void ColorPhysics :: destroyJoints ()
{
	for( int i=0; i<joints.size(); i++ )
	{
		box2d->getWorld()->DestroyJoint( joints[ i ] );
		joints[ i ] = NULL;
	}
	
	joints.clear();
}

void ColorPhysics :: resetJoints ()
{
	destroyJoints();
	createJoints();
}

///////////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////////

void ColorPhysics :: update ()
{
	for( int i=0; i<circles.size(); i++ )
	{
		ColorCircle& circle = *circles[ i ];
		
		circle.update();
	}
	
	box2d->update();
}

///////////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////////

void ColorPhysics :: draw ()
{
	for( int i=0; i<circles.size(); i++ )
	{
		ColorCircle& circle = *circles[ i ];
		
		circle.draw();
	}
}