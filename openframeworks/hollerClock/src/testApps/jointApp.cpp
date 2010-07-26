#include "jointApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void jointApp :: setup()
{
	ofSetFrameRate( 60 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.createBounds();
	box2d.registerGrabbing();
	
	float mass		= 3.0;
	float bounce	= 0.53;
	float friction	= 0.1;
	float radius	= 40.0;
	
	c1.setPhysics( mass, bounce, friction );
	c1.setup( box2d.getWorld(), ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ), radius, false );
	c1.setRotationFriction( 1.0 );
	c1.setDamping( 1.0 );
	
	c2.setPhysics( mass, bounce, friction );
	c2.setup( box2d.getWorld(), ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ), radius, false );
	c2.setRotationFriction( 1.0 );
	c2.setDamping( 1.0 );
	
	b2BodyDef bd;
	bd.position.Set( ( ofGetWidth() * 0.5 ) /OFX_BOX2D_SCALE, ( ofGetHeight() * 0.5 ) / OFX_BOX2D_SCALE);
	point = box2d.getWorld()->CreateBody( &bd );
	
	createJoint();
}

void jointApp :: createJoint ()
{
	joint.setWorld( box2d.getWorld() );
	
	b2DistanceJointDef jd;
	b2Vec2 p1, p2, d;
	
	jd.frequencyHz  = 1.0;
	jd.dampingRatio = 0.2;
	
	jd.body1 = c1.body;
	jd.body2 = point;
	
	jd.localAnchor1.Set( 0, 0 );
	jd.localAnchor2.Set( 0, 0 );
	jd.collideConnected = false;
	
	p1	= jd.body1->GetWorldPoint( jd.localAnchor1 );
	p2	= jd.body2->GetWorldPoint( jd.localAnchor2 );
	d	= p2 - p1;
	
	jd.length = 0;
	
	joint.joint = box2d.getWorld()->CreateJoint( &jd );
	
	c1.body->WakeUp();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void jointApp::update()
{
	box2d.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void jointApp::draw()
{
	ofBackground( 0, 0, 0 );
	
	box2d.draw();
	
	c1.draw();
	c2.draw();
	
	joint.draw();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void jointApp::keyPressed(int key)
{
	if( key == ' ' && !joint.alive )
		createJoint();
}

void jointApp::keyReleased(int key)
{

}

void jointApp::mouseMoved(int x, int y )
{
	if( joint.alive )
	{
		b2DistanceJoint* jd = (b2DistanceJoint*)joint.joint;
	}
}

void jointApp::mouseDragged(int x, int y, int button)
{

}

void jointApp::mousePressed(int x, int y, int button)
{

}

void jointApp::mouseReleased(int x, int y, int button)
{

}

void jointApp::windowResized(int w, int h)
{

}

